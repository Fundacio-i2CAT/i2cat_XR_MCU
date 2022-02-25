#pragma once

#include "ddk_signal_functor.h"
#include "ddk_connection_base.h"
#include <cassert>

namespace ddk
{

class connection
{

public:
	connection();
    connection(detail::connection_base& i_conn);
    connection(const connection& i_conn) = delete;
	connection(connection&& i_conn);
	~connection();

    connection& operator=(const connection& other) = delete;
	connection& operator=(connection&& other);
    connection& operator=(detail::connection_base& i_conn);

    bool is_enabled() const;
	bool is_connected() const;
    void enable();
    void disable();
    void disconnect();

private:
    detail::connection_base* m_referredConnection;
};

}