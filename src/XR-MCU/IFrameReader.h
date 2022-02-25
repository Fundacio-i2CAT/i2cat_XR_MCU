#pragma once

#include "ddk_unique_reference_wrapper.h"
#include "ddk_lent_reference_wrapper.h"
#include "FrameReaderDefs.h"
#include "ddk_result.h"

namespace mcu
{

class IFrameReader
{
public:
	virtual ~IFrameReader() = default;

	enum ConnectErrorCode
	{
		ConnectInvalidHandleError,
		ConnectHandlePlayError,
		ConnectAlreadyConnected
	};
	typedef ddk::result<void,ConnectErrorCode> connect_result;
	enum ReadErrorCode
	{
		ReadNoFrameAvailableError,
		ReadBytesReadMismatchError,
		ReadInvalidHandle
	};
	typedef ddk::result<frame_data,ReadErrorCode> read_result;
	enum DestroyErrorCode
	{
		DestroyInvalidHandle
	};
	typedef ddk::result<void,ReadErrorCode> destroy_result;

	virtual connect_result connect() = 0;
	virtual read_result read() = 0;
	virtual destroy_result destroy() = 0;

	virtual bool is_connected() const = 0;
};

typedef ddk::unique_reference_wrapper<IFrameReader> frame_reader_unique_ref;
typedef ddk::unique_reference_wrapper<const IFrameReader> frame_reader_const_unique_ref;
typedef ddk::unique_pointer_wrapper<IFrameReader> frame_reader_unique_ptr;
typedef ddk::unique_pointer_wrapper<const IFrameReader> frame_reader_const_unique_ptr;
typedef ddk::lent_reference_wrapper<IFrameReader> frame_reader_lent_ref;
typedef ddk::lent_reference_wrapper<const IFrameReader> frame_reader_const_lent_ref;

}