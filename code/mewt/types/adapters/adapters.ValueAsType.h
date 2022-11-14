
#pragma once

namespace mewt::types::adapters
{

	template <auto ktValue, typename TValueType = decltype(ktValue)>
	struct ValueAsType
	{
		static constexpr auto kValue = ktValue;
	};

	template <auto ktValue>
	constexpr auto kValueAsType = ValueAsType<ktValue> {};

}