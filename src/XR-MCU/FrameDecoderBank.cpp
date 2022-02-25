#include "FrameDecoderBank.h"
#include "CwipcFrameDecoder.h"
#include "IMCUProfiler.h"
#include "AcquisitionModuleProfiler.h"
#include "MCUManagerDefs.h"

namespace mcu
{

FrameDecoderBank::FrameDecoderBank(const MCUDecoderConfigProvider& i_configProvider, idecoded_frame_receiver_lent_ref i_frameReceiver)
: m_threadPool(ddk::thread_pool::GrowsOnDemand,0)
, m_config(i_configProvider.getConfig())
, m_frameReceiver(i_frameReceiver)
, m_currmaxFpsId(k_invalidFrameSourceId)
{
	pthread_mutex_init(&m_decoderMutex,nullptr);
}
FrameDecoderBank::~FrameDecoderBank()
{
	std::map<frame_source_id,iraw_frame_decoder_unique_ref>::iterator itDecoder = m_decoders.begin();
	for (;itDecoder!=m_decoders.end();++itDecoder)
	{
		itDecoder->second->stop();
	}

	pthread_mutex_destroy(&m_decoderMutex);
}
void FrameDecoderBank::init()
{
	pfr::iprofiler_lent_ref mcuProfiler = pfr::IProfiler::get_instance();

	m_measuremntConn.push_back(mcuProfiler->listen_to<DecodedFrameRateMeasurable>(k_acquisition_profile_module,ddk::make_function(this,&FrameDecoderBank::checkDecoderFrameRate)));
}
FrameDecoderBank::register_result FrameDecoderBank::register_frame_source(frame_source_id i_id, const decoder_context& i_context, iraw_frame_source_const_lent_ref i_frameSource)
{
	pthread_mutex_lock(&m_decoderMutex);

	std::map<frame_source_id,iraw_frame_decoder_unique_ref>::iterator itDecoder = m_decoders.lower_bound(i_id);

	if (itDecoder == m_decoders.end() || itDecoder->first != i_id)
	{
		switch (i_context.m_type)
		{
			case DecodecType::Cwipc:
			{
				ddk::thread_pool::acquire_result<ddk::thread_sheaf> acqRes = m_threadPool.acquire_sheaf(m_config.getNumDecoderThreads());

				if (acqRes)
				{
					iraw_frame_decoder_unique_ref newFrameDecoder = ddk::make_unique_reference<CwipcFrameDecoder>(i_id,m_frameReceiver,std::move(acqRes).extract(),m_config.getMaxNumPendingFrames());

					i_frameSource->sig_onNewFrame.connect(ddk::make_function(newFrameDecoder.get(),&IRawFrameDecoder::enqueue_raw_frame));

					newFrameDecoder->start();

					m_decoders.insert(itDecoder,std::make_pair(i_id,std::move(newFrameDecoder)));

					pthread_mutex_unlock(&m_decoderMutex);
				}
				else
				{
					pthread_mutex_unlock(&m_decoderMutex);

					return ddk::make_error<register_result>(RegisterErrorCode::RegisterLackOfResources);
				}

				break;
			}
			default:
				pthread_mutex_unlock(&m_decoderMutex);

				return ddk::make_error<register_result>(RegisterErrorCode::RegisterUnsupportedDecoder);
		}

		return ddk::success;
	}
	else
	{
		pthread_mutex_unlock(&m_decoderMutex);

		return ddk::make_error<register_result>(RegisterErrorCode::RegisterSourceALreadyExist);
	}
}
FrameDecoderBank::unregister_result FrameDecoderBank::unregister_frame_source(frame_source_id i_id)
{
	std::map<frame_source_id,iraw_frame_decoder_unique_ref>::iterator itDecoder = m_decoders.find(i_id);

	if (itDecoder != m_decoders.end())
	{
		itDecoder->second->stop();

		m_decoders.erase(itDecoder);

		return ddk::success;
	}
	else
	{
		return ddk::make_error<unregister_result>(UnregisterErrorCode::UnregisterSourceNonExist);
	}
}
size_t FrameDecoderBank::needed_resources() const
{
	return k_numCpusForDecoding;
	//this computation is attached to the number of points (now hardcoded to 30k)
	return static_cast<size_t>(m_decoders.size() * 1.5f);
}
void FrameDecoderBank::set_affinity(const cpu_set_t& i_cpuSet)
{
	pthread_mutex_lock(&m_decoderMutex);

	std::map<frame_source_id,iraw_frame_decoder_unique_ref>::iterator itDecoder = m_decoders.begin();
	for(; itDecoder != m_decoders.end(); ++itDecoder)
	{
		itDecoder->second->set_affinity(i_cpuSet);
	}

	pthread_mutex_unlock(&m_decoderMutex);
}
void FrameDecoderBank::checkDecoderFrameRate(const frame_decoder_fps& i_measure)
{
	typedef typename std::set<frame_decoder_fps>::iterator iterator;

	//this code must be reviewed
	return;

	pthread_mutex_lock(&m_decoderMutex);

	iterator itMaxDecoder = m_decoderFpsSet.find(m_currmaxFpsId);
	if (itMaxDecoder != m_decoderFpsSet.end())
	{
		if (i_measure.get_fps() > itMaxDecoder->get_fps())
		{
			m_currmaxFpsId = i_measure.get_id();
		}
	}
	else
	{
		m_currmaxFpsId = i_measure.get_id();
		std::pair<iterator,bool> insertRes = m_decoderFpsSet.insert(i_measure);
		if (insertRes.second)
		{
			itMaxDecoder = insertRes.first;
		}
		else
		{
			pthread_mutex_unlock(&m_decoderMutex);

			return;
		}
	}

	//this 20 is arbitary, select a proper way of defining it
	if (i_measure.get_fps() < 20.f)
	{
		std::map<frame_source_id,iraw_frame_decoder_unique_ref>::iterator itDecoder = m_decoders.find(i_measure.get_id());
		if (itDecoder != m_decoders.end())
		{
			//first of all, check if there are some threads left in the thread pool
			ddk::thread_pool::acquire_result<ddk::thread> acquireRes = m_threadPool.aquire_thread();
			if (acquireRes)
			{
				itDecoder->second->lend_thread(std::move(acquireRes).extract());
			}
			else if(m_currmaxFpsId != itMaxDecoder->get_id() && itMaxDecoder->get_fps() > 25.f)
			{
				//otherwise, try to borrow some threads from another decoder
				if (ddk::optional<ddk::thread> borrowedThreadOpt = m_decoders.at(m_currmaxFpsId)->borrow_thread())
				{
					itDecoder->second->lend_thread(std::move(*borrowedThreadOpt));
				}
			}
		}
	}

	pthread_mutex_unlock(&m_decoderMutex);
}

}