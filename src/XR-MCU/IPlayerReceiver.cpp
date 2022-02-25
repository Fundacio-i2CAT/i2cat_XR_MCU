#include "IPlayerReceiver.h"

namespace mcu
{

ddk::lent_pointer_wrapper<IPlayerReceiver> IPlayerReceiver::s_self = nullptr;

ddk::lent_pointer_wrapper<IPlayerReceiver> IPlayerReceiver::get_instance()
{
	return s_self;
}

}