#pragma once

#include <stddef.h>

namespace ddk
{

class connection;

namespace detail
{

class signal_connector;

class connection_base
{
    friend class ::ddk::connection;

public:
    connection_base(const signal_connector& i_connector);
	connection_base(const connection_base&) = delete;
	connection_base(connection_base&& other) = delete;
	virtual ~connection_base() = default;

	connection_base& operator=(const connection_base&) = delete;
	connection_base& operator=(connection_base&&) = delete;

	void enable();
	void disable();
	void disconnect();
    bool is_enabled() const;
	size_t get_id() const;

private:
    bool m_isEnabled;
	const signal_connector& m_connector;
};

}
}
