
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_template_helper.h"
#include "ddk_function.h"

namespace ddk
{
namespace iter
{

template<typename Functor>
auto transform_iterable_transform::operator()(Functor&& i_transform) const
{
	return impl<Functor>{i_transform};
}

template<typename Functor>
transform_iterable_transform::impl<Functor>::impl(Functor i_transform)
: m_transform(std::forward<Functor>(i_transform))
{
}

}
}

TEMPLATE(typename Sink,typename Iterable)
REQUIRED(IS_CONTAINER(Sink))
auto operator<<=(Sink& i_sink, const Iterable& i_iterable)
{
	return ddk::trans::detail::iterable_transformation_dump(i_sink,ddk::deduce_iterable(i_iterable));
}