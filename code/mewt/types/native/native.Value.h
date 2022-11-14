
#pragma once

namespace mewt::types::native
{

	template<typename TType>
	concept isNativeValueType = std::is_integral_v<TType>;

	template<typename TType>
	concept isValueContainer = requires(TType value) { innerValue(value); };

	constexpr auto valueOf(isNativeValueType auto value) { return value; }

	template<typename TEnum>
		requires(std::is_enum_v<TEnum>)
	constexpr auto valueOf(TEnum value)
	{
		return static_cast<std::underlying_type_t<TEnum>>(value);
	}

	constexpr auto valueOf(isValueContainer auto value)
	{
		return valueOf(innerValue(value));
	}

}