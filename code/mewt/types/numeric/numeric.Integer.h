
#pragma once

#include "mewt/types/native/native.Bit.h"
#include "mewt/types/numeric/numeric.Count.h"
#include "mewt/types/native/native.Integer.decl.h"
#include "mewt/types/native/native.Value.h"

namespace mewt::types::numeric
{

	/*struct IntegerLayout
	{
		friend constexpr auto getNumericSpaceGranularity(traits::TypeId<IntegerLayout>)
		{
			return space::Granularity::Discrete;
		}
	};

	using IntegerSize = Count<native::Bit, IntegerLayout>;*/

	template <value::Relative NBitCount, bool NIsSigned>
		requires isCountOf<decltype(NBitCount), native::Bit>
	struct Integer
	{
		//static_assert(isCountOf<decltype(NBitCount), native::Bit>);
		using NativeUnsignedValueType = std::tuple_element_t<
			 (int)(NBitCount > native::kBitsPerByte) + (int)(NBitCount > native::kBitsPerWord) +
				  (int)(NBitCount > native::kBitsPerDWord) + (int)(NBitCount > native::kBitsPerQWord),
			 std::tuple<std::uint_fast8_t, std::uint_fast16_t, std::uint_fast32_t, std::uint_fast64_t,
							void>>;

		using NativeSignedValueType = std::tuple_element_t<
			 (int)(NBitCount > native::kBitsPerByte) + (int)(NBitCount > native::kBitsPerWord) +
				  (int)(NBitCount > native::kBitsPerDWord) + (int)(NBitCount > native::kBitsPerQWord),
			 std::tuple<std::int_fast8_t, std::int_fast16_t, std::int_fast32_t, std::int_fast64_t,
							void>>;

		using InnerValueType =
			 std::conditional_t<NIsSigned, NativeSignedValueType, NativeUnsignedValueType>;

		InnerValueType inner_value : native::valueOf(NBitCount);

		friend constexpr auto innerValue(Integer value) { return value.inner_value; }

		constexpr friend auto operator<=>(Integer lhs, Integer rhs)
		{
			return lhs.inner_value <=> rhs.inner_value;
		}

		constexpr friend auto operator==(Integer lhs, Integer rhs) -> bool
		{
			return lhs.inner_value == rhs.inner_value;
		}

		constexpr friend auto operator+=(Integer& lhs, Integer rhs) -> Integer&
		{
			lhs.inner_value += rhs.inner_value;
			return lhs;
		}

		constexpr friend auto operator+(Integer lhs, Integer rhs)
		{
			Integer ret(lhs);
			return ret += rhs;
		}

		//template <typename TTarget>
		//	requires(std::is_convertible_v<InnerValueType, TTarget>)
		//constexpr operator TTarget() const
		//{
		//	return innerValue;
		//}

		friend constexpr auto operator++(Integer& value) -> Integer&
		{
			++value.inner_value;
			return value;
		}

		friend constexpr auto operator--(Integer& value) -> Integer&
		{
			--value.inner_value;
			return value;
		}

		constexpr friend auto operator&=(Integer& lhs, Integer rhs) -> Integer&
		{
			lhs.inner_value &= rhs.inner_value;
			return lhs;
		}

		friend constexpr auto operator&(Integer lhs, Integer rhs)
		{
			Integer ret(lhs);
			return ret &= rhs;
		}

		constexpr friend auto operator|=(Integer& lhs, Integer rhs) -> Integer&
		{
			lhs.inner_value |= rhs.inner_value;
			return lhs;
		}

		friend constexpr auto operator|(Integer lhs, Integer rhs)
		{
			Integer ret(lhs);
			return ret |= rhs;
		}

		friend constexpr auto operator~(Integer value)
		{
			Integer ret;
			ret.inner_value = ~innerValue(value);
			return ret;
		}

		friend constexpr auto operator<<(Integer value, isCountOf<native::Bit> auto shift) -> Integer
		{
			return { static_cast<InnerValueType>(innerValue(value) << native::valueOf(shift)) };
		}

	};

}