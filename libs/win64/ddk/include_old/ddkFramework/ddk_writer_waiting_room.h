#pragma once

#include "ddk_iwaiting_room.h"
#include "ddk_mutex.h"

namespace ddk
{

class writer_waiting_room : public iwaiting_room
{
public:
	writer_waiting_room(SharedState& i_sharedState);

private:
	virtual void _enter_area(Reentrancy i_reentrancy) override;
	virtual bool _try_to_enter_area(Reentrancy i_reentrancy) override;
	virtual void _leave_area() override;
	virtual const SharedState& getSharedState() const override;

	size_t				m_numParticipants;
	SharedState&		m_sharedState;
	mutex	m_stateRoomMutex;
};

}