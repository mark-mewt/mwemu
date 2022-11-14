
#pragma once

#include <type_traits>

namespace mewt::types::numeric::value
{

	template <typename TInnerType, typename TDerived>
	struct Strong
	{
		using InnerType = TInnerType;
		using DerivedType = TDerived;
		TInnerType innerValue;

		friend constexpr auto innerValue(const Strong& value) -> const auto
		{
			return value.innerValue;
		}

		friend constexpr auto innerValue(Strong&& value) -> const auto
		{
			return value.innerValue;
		}

		friend constexpr auto innerValue(Strong& value) -> auto&
		{
			return value.innerValue;
		}

		friend constexpr auto isValueFn(Strong)
		{
			return true;
		}
	};

	template <typename TType>
	concept isStrongValue =
		 std::is_base_of_v<Strong<typename TType::InnerType, typename TType::DerivedType>, TType>;

	template <isStrongValue TValue>
	constexpr auto operator++(TValue& value) -> auto&
	{
		++innerValue(value);
		return value;
	}

	template <isStrongValue TValue>
	constexpr auto operator--(TValue& value) -> auto&
	{
		--innerValue(value);
		return value;
	}

}