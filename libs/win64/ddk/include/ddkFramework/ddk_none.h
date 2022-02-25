#pragma once

namespace ddk
{
namespace detail
{

struct none_t
{
};

}
}

const ddk::detail::none_t none = ddk::detail::none_t();
