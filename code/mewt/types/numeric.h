
#pragma once

#define USE_NEW_NUMERICS 1

#include "mewt/types/byte.h"
#include "mewt/types/traits.h"

#include <tuple>

namespace mewt::types
{

	// template <typename... TArgs>
	// constexpr auto staticError();

	/*template <typename TIntegral>
		requires(std::is_integral_v<TIntegral> || std::is_enum_v<TIntegral>)
	constexpr auto declareSpaceAsDiscrete(ClassId<TIntegral>)
	{
		return true;
	}*/

	template <typename TResult, typename... TArgs>
	constexpr auto sum(TArgs&&... args)
	{
		TResult init { 0 };
		return (init + ... + args);
	}

	// template <typename TSpace>
	// constexpr auto getNativeTypeForUnit(ClassId<Byte>, ClassId<TSpace>) -> ClassId<std::size_t>;

	// template <typename TType>
	// struct ObjectRepresentation;

	// template <typename TType>
	// constexpr auto declareSpaceAsDiscrete(ClassId<ObjectRepresentation<TType>> /*unused*/)
	//{
	//	return true;
	// }

	// template <typename TType>
	// constexpr auto getNativeTypeForUnit(ClassId<Byte>, ClassId<ObjectRepresentation<TType>>)
	//	 -> ClassId<std::size_t>;

	/*struct AnyDiscreteSpace;

	constexpr auto declareSpaceAsDiscrete(ClassId<AnyDiscreteSpace> )
	{
		return true;
	}

	template <isDiscreteSpace TSpace>
	constexpr auto declareEquivalentSpaces(ClassId<TSpace>, ClassId<AnyDiscreteSpace>)
	{
		return true;
	}

	using BitCount = Count<Bit, AnyDiscreteSpace>;
	*/

}
