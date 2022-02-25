#include "SocketIoConnection.h"
#include "connection_exceptions.h"

namespace mcu
{
namespace
{

std::string&& acquire_sio_binary(sio::message::ptr i_data);
std::string acquire_sio_string(sio::message::ptr i_data);
std::string acquire_sio_int(sio::message::ptr i_data);
std::string acquire_sio_double(sio::message::ptr i_data);
std::map<std::string,std::string> acquire_sio_object(sio::message::ptr i_data);

std::map<std::string,std::string> acquire_response_fields(sio::message::ptr i_data)
{
	std::map<std::string,std::string> payload;

	switch(i_data->get_flag())
	{
	case sio::message::flag_binary:
	{
		payload.insert(std::make_pair("",acquire_sio_binary(i_data)));

		break;
	}
	case sio::message::flag_string:
	{
		payload.insert(std::make_pair("",acquire_sio_string(i_data)));

		break;
	}
	case sio::message::flag_integer:
	{
		payload.insert(std::make_pair("",acquire_sio_int(i_data)));

		break;
	}
	case sio::message::flag_double:
	{
		payload.insert(std::make_pair("",acquire_sio_double(i_data)));

		break;
	}
	case sio::message::flag_object:
	{
		payload = acquire_sio_object(i_data);

		break;
	}
	}

	return std::move(payload);
}
std::string&& acquire_sio_binary(sio::message::ptr i_data)
{
	return std::move(const_cast<std::string&>(*(i_data->get_binary())));
}
std::string acquire_sio_string(sio::message::ptr i_data)
{
	return i_data->get_string();
}
std::string acquire_sio_int(sio::message::ptr i_data)
{
	return std::to_string(i_data->get_int());
}
std::string acquire_sio_double(sio::message::ptr i_data)
{
	return std::to_string(i_data->get_double());
}
std::map<std::string,std::string> acquire_sio_object(sio::message::ptr i_data)
{
	std::map<std::string,std::string> res;

	for(const auto& itBody : i_data->get_map())
	{
		std::map<std::string,std::string> respFields = acquire_response_fields(itBody.second);

		for(const auto& itResp : respFields)
		{
			res.insert(std::make_pair(itBody.first + itResp.first,itResp.second));
		}
	}

	return std::move(res);
}

}

SocketIoConnection::SocketIoConnection(const std::string& i_url)
: m_url(i_url)
{
}
SocketIoConnection::~SocketIoConnection()
{
}
void SocketIoConnection::start()
{
	if(m_state == Idle)
	{
		m_client.set_open_listener([&](){
			if(m_client.opened())
			{
				if(m_state == Connecting)
				{
					m_socket = m_client.socket();

					m_state = Connected;

					sig_onStateChanged.execute(Connecting,Connected);
				}
			}
		});

		m_client.set_close_listener([this](sio::client::close_reason const& reason) 
		{
			if(m_client.opened() == false)
			{
				if(m_state == Connected)
				{
					m_state = Idle;

					sig_onStateChanged.execute(Connected,Idle);
				}
			}
		});

		m_client.set_socket_close_listener([](const std::string& nsp)
		{
		});

		m_client.set_reconnect_listener([](unsigned int i_lhs, unsigned int i_rhs)
		{
		});

		m_client.set_fail_listener([&](){
			if(m_client.opened())
			{
				m_state = Idle;

				sig_onStateChanged.execute(Connecting,Idle);
			}
		});

		m_client.set_reconnect_attempts(10);

		m_client.set_reconnect_delay(10);

		m_client.set_reconnect_delay_max(1000);

		m_client.connect(m_url);

		m_state = Connecting;

		sig_onStateChanged.execute(Idle,Connecting);
	}
}
void SocketIoConnection::stop()
{
	if(m_client.opened())
	{
		m_client.close();
	}
}
bool SocketIoConnection::listen(const std::string& i_eventName,const ddk::function<bool(const MessageResponse&)>& i_callback)
{
	if(m_socket)
	{
		if(m_eventBindings.find(i_eventName) == m_eventBindings.end())
		{
			m_socket->on(i_eventName,[this,i_eventName](sio::event& i_event)
			{
				std::list<MessageResponseEntry> keyValueResponse;

				const auto msgs = i_event.get_messages();
				for(size_t msgIndex=0;msgIndex<msgs.size();msgIndex++)
				{
					const auto& msg = msgs.at(msgIndex);

					keyValueResponse.push_back(acquire_response_fields(msgs.at(msgIndex)));
				}

				MessageType msgType;
				get_enum_from_name(msgType,i_eventName.c_str());

				for(const auto& binding : m_eventBindings[i_eventName])
				{
					if(ddk::eval(binding,MessageResponse(msgType,i_eventName,keyValueResponse)))
					{
						break;
					}
				}
			});
		}

		m_eventBindings[i_eventName].push_back(i_callback);
	
		return true;
	}
	else
	{
		return false;
	}
}
bool SocketIoConnection::is_connected() const
{
	return m_client.opened();
}
bool SocketIoConnection::send(const IOrchestratorMessage& i_msg,const ddk::function<void(msg_result)>& i_ackCallback)
{
	sio::message::list msgs;

	for(Message& msg : i_msg.construct_message())
	{
		if(msg.is_binary())
		{
			msgs.push(std::make_shared<std::string>(msg.extract_contents()));
		}
		else
		{
			msgs.push(msg.extract_contents());
		}
	}

	if(const ddk::optional<std::string> responseMsgOpt = i_msg.get_response_name())
	{
		m_socket->on(*responseMsgOpt,[this,messageType = i_msg.get_type(),messageId = i_msg.get_id(),responseMsg = *responseMsgOpt](const sio::event& i_event)
		{
			if(sio::object_message::ptr objMsg = static_cast<sio::object_message::ptr>(i_event.get_message()))
			{
				sig_onNewResponse.execute(MessageResponse(messageType,responseMsg,{acquire_response_fields(objMsg)}));
			}
		});
	}

	try
	{
		m_socket->emit(i_msg.get_name(),msgs,[msgName= i_msg.get_name(),msgType= i_msg.get_type(),i_ackCallback](const sio::message::list& i_msgs)
		{
			MessageResponse response(msgType,msgName);

			for(size_t msgIndex=0;msgIndex<i_msgs.size();msgIndex++)
			{
				for(const auto& itMsg : i_msgs[msgIndex]->get_map())
				{
					if(itMsg.first == "error")
					{
						const int errorCode = itMsg.second->get_int();

						if(errorCode != 0)
						{
							if(i_ackCallback != nullptr)
							{
								ddk::eval(i_ackCallback,ddk::make_error<msg_result>(errorCode));

								return;
							}
						}
					}
					else if(itMsg.first == "body")
					{
						response.add_contents(acquire_response_fields(itMsg.second));
					}
				}
			}

			if(i_ackCallback != nullptr)
			{
				ddk::eval(i_ackCallback,ddk::make_result<msg_result>(std::move(response)));
			}
		});
	}
	catch(const std::exception& i_excp)
	{
	}

	return true;
}

}