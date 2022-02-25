#pragma once

#include "ddk_tuple.h"
#include "ddk_iterable.h"
#include "ddk_iterable_value.h"
#include "ddk_iterable_algorithm_impl.h"
#include "ddk_concepts.h"
#include "ddk_container_concepts.h"
#include "ddk_function_ops.h"

#define FUNC_ITERABLE_TRANSFORM(_NAME,_FUNC) \
	struct _NAME##_iterable_transform : iterable_transform \
	{ \
		template<typename Iterable> \
		friend inline auto operator<<=(const _NAME##_iterable_transform& i_lhs,Iterable&& i_rhs) \
		{ \
			return i_lhs() <<= std::forward<Iterable>(i_rhs); \
		} \
		template<typename,typename...> \
		struct impl; \
		template<size_t ... Indexs,typename ... Args> \
		struct impl<mpl::sequence<Indexs...>,Args...> \
		{ \
			template<typename Iterable> \
			friend inline auto operator<<=(const impl& i_lhs, Iterable&& i_rhs) \
			{ \
				return i_lhs(std::forward<Iterable>(i_rhs)); \
			} \
			\
			constexpr impl(Args ... i_args) \
			: m_args(std::forward<Args>(i_args)...) \
			{ \
			} \
			template<typename Iterable> \
			inline auto operator()(Iterable&& i_iterable) const \
			{ \
				if constexpr (mpl::num_types<Args...> >= 1) \
				{ \
					return transform(_FUNC(m_args.template get<Indexs>()...)) <<= deduce_iterable(i_iterable); \
				} \
				else \
				{ \
					return transform(_FUNC) <<= deduce_iterable(i_iterable); \
				} \
			} \
			\
			tuple<Args...> m_args; \
		}; \
	public: \
		_NAME##_iterable_transform() = default; \
		template<typename ... Args> \
		inline auto operator()(Args&& ... i_args) const \
		{ \
			typedef impl<typename mpl::make_sequence<0,mpl::num_types<Args...>>::type,Args...> __iterable_impl; \
			\
			static const __iterable_impl _impl(std::forward<Args>(i_args)...); \
			\
			return _impl; \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

#define UNARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	struct _NAME##_iterable_transform : iterable_transform \
	{ \
		template<typename Iterable> \
		friend inline auto operator<<=(const _NAME##_iterable_transform& i_lhs,const Iterable& i_rhs) \
		{ \
			return i_lhs(i_rhs); \
		} \
		template<typename Iterable> \
		struct impl  : public detail::inherited_functor_impl<decltype(_OP std::declval<typename Iterable::const_reference>()),typename Iterable::const_reference> \
		{ \
			struct callable_tag; \
			constexpr impl() = default; \
			constexpr impl(const impl&) = default; \
			constexpr impl(impl&&) = default; \
			decltype(_OP std::declval<typename Iterable::const_reference>()) operator()(detail::forwarded_arg<typename Iterable::const_reference> i_value) const override \
			{ \
				return _OP i_value; \
			} \
		}; \
	public: \
		_NAME##_iterable_transform() = default; \
		template<typename Iterable> \
		inline auto operator()(Iterable&& i_iterable) const \
		{ \
			using ::operator<<=; \
			typedef impl<mpl::remove_qualifiers<Iterable>> __iterable_impl; \
			static const __iterable_impl _impl; \
			return transform(_impl) <<= deduce_iterable(i_iterable); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

#define BINARY_ITERABLE_TRANSFORM(_NAME,_OP) \
	struct _NAME##_iterable_transform : iterable_transform \
	{ \
	private: \
		template<typename Iterable> \
		friend inline auto operator<<=(const _NAME##_iterable_transform& i_lhs,const Iterable& i_rhs) \
		{ \
			return i_lhs(i_rhs); \
		} \
		template<typename ... Iterables> \
		struct impl : public detail::inherited_functor_impl<decltype((std::declval<typename Iterables::const_reference>() _OP ...)), typename Iterables::const_reference ...> \
		{ \
			struct callable_tag; \
			constexpr impl() = default; \
			constexpr impl(const impl&) = default; \
			constexpr impl(impl&&) = default; \
			decltype((std::declval<typename Iterables::const_reference>() _OP ...)) operator()(detail::forwarded_arg<typename Iterables::const_reference> ... i_values) const override \
			{ \
				return (i_values _OP ...); \
			} \
		}; \
	\
	public: \
		constexpr _NAME##_iterable_transform() = default; \
		template<typename Iterable> \
		inline auto operator()(Iterable&& i_iterable) const \
		{ \
			using ::operator<<=; \
			typedef mpl::remove_qualifiers<Iterable> iterable_t; \
			typedef mpl::remove_qualifiers<typename iterable_t::value_type> value_type; \
			typedef typename iterable_t::const_reference const_reference; \
			\
			typed_arena<value_type> acc; \
			\
			make_function([&acc](const_reference i_value){ acc.template set_value<value_type>(acc.empty() ? i_value : (acc.template get<value_type>() _OP i_value)); }) <<= i_iterable; \
			\
			const value_type res = (acc.empty()) ? 0 : acc.template get<value_type>(); \
			\
			acc.template destroy<value_type>(); \
			\
			return res; \
		} \
		template<typename Iterable, typename ... Iterables> \
		inline auto operator()(Iterable&& i_iterable, Iterables&& ... i_iterables) const \
		{ \
			using ::operator<<=; \
			typedef impl<mpl::remove_qualifiers<Iterable>,mpl::remove_qualifiers<Iterables>...> __iterable_impl; \
			static const __iterable_impl _impl; \
			return transform(_impl) <<= ddk::fusion(deduce_iterable(std::forward<Iterable>(i_iterable)),deduce_iterable(std::forward<Iterables>(i_iterables)) ...); \
		} \
	}; \
	const _NAME##_iterable_transform _NAME;

namespace ddk
{
namespace iter
{

struct iterable_transform
{
};

struct transform_iterable_transform : iterable_transform
{
private:
	template<typename Functor>
	struct impl
	{
		template<typename Iterable>
		friend inline auto operator<<=(const impl& i_lhs, Iterable&& i_rhs)
		{
			using ::operator<<=;
			return detail::iterable_transform<detail::resolved_callable<Functor>>(make_function(i_lhs.m_transform)) <<= deduce_iterable(i_rhs);
		}
		impl(Functor i_transform);

	private:

		const Functor m_transform;
	};

public:
	constexpr transform_iterable_transform() = default;
	template<typename Functor>
	inline auto operator()(Functor&& i_transform) const;
};
const transform_iterable_transform transform;

}
}

TEMPLATE(typename Sink,typename Iterable)
REQUIRES(IS_CONTAINER(Sink))
inline auto operator<<=(Sink& i_lhs, const Iterable& i_rhs);

#include "ddk_iterable_algorithm.inl"

namespace ddk
{
namespace iter
{

UNARY_ITERABLE_TRANSFORM(inv,-)

BINARY_ITERABLE_TRANSFORM(sum,+)
BINARY_ITERABLE_TRANSFORM(subs,-)
BINARY_ITERABLE_TRANSFORM(prod,*)
BINARY_ITERABLE_TRANSFORM(div,/ )

FUNC_ITERABLE_TRANSFORM(sqrt,ddk::sqrt)
FUNC_ITERABLE_TRANSFORM(pow,ddk::pow)
FUNC_ITERABLE_TRANSFORM(sin,ddk::sin)
FUNC_ITERABLE_TRANSFORM(cos,ddk::cos)

}
}