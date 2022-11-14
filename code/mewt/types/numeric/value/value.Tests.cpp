
#include "mewt/types/adapters/adapters.ValueAsType.h"

#include <cstdint>
#include <type_traits>
#include <concepts>

namespace mewt::types::numeric
{

	enum class ValueBitWidth : std::uint8_t;

	template <typename TEnum>
		requires std::is_enum_v<TEnum>
	constexpr auto unwrapValue(TEnum value)
	{
		return static_cast<std::underlying_type_t<TEnum>>(value);
	}

	template <typename TType>
	constexpr auto kBitWidthOf = ValueBitWidth { 8 * sizeof(TType) };

	// enum ValueDomain
	//{
	//	Signed,
	//	Unsigned,
	//	FloatingPoint
	// };

	namespace config
	{

		enum class Signedness
		{
			Signed,
			Unsigned
		};

		enum class FloatPrecision
		{
			Single,
			Double,
			Quad
		};

		enum class VectorDimension : std::uint8_t;

		template <typename TUnit>
		struct Value
		{
			using UnitType = TUnit;
		};

		template<typename TType>
		concept isValueConfig = std::derived_from<TType, Value<typename TType::UnitType>>;

		template <auto kTConfig, typename TConfigType = decltype(kTConfig)>
			requires isValueConfig<TConfigType>
		constexpr auto determineInnerValueType(adapters::ValueAsType<kTConfig>)
		{
		}

		template <ValueBitWidth ktBitWidth, Signedness ktSignedness>
		struct SmallInt
		{
			using ValueType = std::conditional_t<ktSignedness == Signedness::Signed, std::int32_t, std::uint32_t>;
			ValueType inner_value : unwrapValue(ktBitWidth);

			constexpr operator ValueType() const { return inner_value; }
		};

		template <typename TUnit>
		struct Integer : public Value<TUnit>
		{
			Signedness signedness { Signedness::Unsigned };
			ValueBitWidth bit_width { 8 };
		};

		template <typename TType>
		concept isIntegerConfig = isValueConfig<TType> &&
		std::derived_from<TType, Integer<typename TType::UnitType>>;

		constexpr auto withBitWidth(isIntegerConfig auto config, ValueBitWidth bit_width)
		{
			config.bit_width = bit_width;
			return config;
		}

		template <auto kTConfig, typename TConfigType = decltype(kTConfig)>
			requires isIntegerConfig<TConfigType>
		constexpr auto determineInnerValueType(adapters::ValueAsType<kTConfig>)
		{
			if constexpr (kTConfig.signedness == Signedness::Signed)
			{
				if constexpr (kTConfig.bit_width == kBitWidthOf<std::int8_t>)
					return std::int8_t { 0 };
				else if constexpr (kTConfig.bit_width == kBitWidthOf<std::int16_t>)
					return std::int16_t { 0 };
				else if constexpr (kTConfig.bit_width == kBitWidthOf<std::int32_t>)
					return std::int32_t { 0 };
				else if constexpr (kTConfig.bit_width == kBitWidthOf<std::int64_t>)
					return std::int64_t { 0 };
				else
					return SmallInt<kTConfig.bit_width, kTConfig.signedness> { 0 };
			}
			else if constexpr (kTConfig.signedness == Signedness::Unsigned)
			{
				if constexpr (kTConfig.bit_width == kBitWidthOf<std::uint8_t>)
					return std::uint8_t { 0 };
				else if constexpr (kTConfig.bit_width == kBitWidthOf<std::uint16_t>)
					return std::uint16_t { 0 };
				else if constexpr (kTConfig.bit_width == kBitWidthOf<std::uint32_t>)
					return std::uint32_t { 0 };
				else if constexpr (kTConfig.bit_width == kBitWidthOf<std::uint64_t>)
					return std::uint64_t { 0 };
				else
					return SmallInt<kTConfig.bit_width, kTConfig.signedness> { 0 };
			}
		}

		template <typename TUnit>
		struct Float : public Value<TUnit>
		{
			FloatPrecision precision;
		};

		template <typename TType>
		concept isFloatConfig =
			 isValueConfig<TType> && std::derived_from<TType, Float<typename TType::UnitType>>;

		template <auto kTConfig, typename TConfigType = decltype(kTConfig)>
			requires isFloatConfig<TConfigType>
		constexpr auto determineInnerValueType(adapters::ValueAsType<kTConfig>)
		{
			return 0.0f;
		}

		template <isValueConfig TBase>
		struct Vector : public TBase
		{
			VectorDimension dimension;
		};

		template <typename TUnit>
		struct FloatVector : public Vector<Float<TUnit>>
		{};

		template <typename TUnit>
		struct IntegerVector : public Vector<Float<TUnit>>
		{};

		template <isValueConfig TBase>
		struct Absolute : public TBase
		{};

		template <isValueConfig TBase>
		struct Relative : public TBase
		{};

		template <typename TUnit>
		struct AbsoluteInteger : public Absolute<Integer<TUnit>>
		{
			constexpr AbsoluteInteger() { this->signedness = Signedness::Unsigned; }
		};

		template <typename TUnit>
		struct RelativeInteger : public Relative<Integer<TUnit>>
		{
			constexpr RelativeInteger() { this->signedness = Signedness::Signed; }
		};

	}

	namespace detail
	{
		/*
		template <ValueConfig kTConfig>
			requires(kTConfig.domain == ValueDomain::Signed)
		constexpr auto determineNativeValueType()
		{
			if constexpr (kTConfig.bit_width <= kBitWidthOf<std::int8_t>)
				return std::int_fast8_t { 0 };
			else if constexpr (kTConfig.bit_width <= kBitWidthOf<std::int16_t>)
				return std::int_fast16_t { 0 };
			else if constexpr (kTConfig.bit_width <= kBitWidthOf<std::int32_t>)
				return std::int_fast32_t { 0 };
			else if constexpr (kTConfig.bit_width <= kBitWidthOf<std::int64_t>)
				return std::int_fast64_t { 0 };
			else
				static_assert(kTConfig.bit_width <= kBitWidthOf<std::int64_t>,
								  "Type not supported - too many bits.");
		}

		template <ValueConfig kTConfig>
			requires(kTConfig.domain == ValueDomain::Unsigned)
		constexpr auto determineNativeValueType()
		{
			if constexpr (kTConfig.bit_width <= kBitWidthOf<std::uint8_t>)
				return std::uint_fast8_t { 0 };
			else if constexpr (kTConfig.bit_width <= kBitWidthOf<std::uint16_t>)
				return std::uint_fast16_t { 0 };
			else if constexpr (kTConfig.bit_width <= kBitWidthOf<std::uint32_t>)
				return std::uint_fast32_t { 0 };
			else if constexpr (kTConfig.bit_width <= kBitWidthOf<std::uint64_t>)
				return std::uint_fast64_t { 0 };
			else
				static_assert(kTConfig.bit_width <= kBitWidthOf<std::uint64_t>,
								  "Type not supported - too many bits.");
		}

		template <ValueConfig kTConfig>
			requires(kTConfig.domain == ValueDomain::FloatingPoint)
		constexpr auto determineNativeValueType()
		{
			if constexpr (kTConfig.bit_width == kBitWidthOf<float>)
				return float { 0 };
			else if constexpr (kTConfig.bit_width == kBitWidthOf<double>)
				return double { 0 };
			else if constexpr (kTConfig.bit_width == kBitWidthOf<long double>)
				return (long double) { 0 };
			else
				static_assert(kTConfig.bit_width == kBitWidthOf<float>,
								  "Type not supported - incorrect number of bits.");
		}*/
	}

	template <auto kTConfig, typename TConfigType = decltype(kTConfig)>
		requires config::isValueConfig<TConfigType>
	using InnerValueType = decltype(determineInnerValueType(adapters::kValueAsType<kTConfig>));

	template <auto kTConfig, typename TConfigType = decltype(kTConfig)>
		requires config::isValueConfig<TConfigType>
	struct Value
	{
		using ConfigType = decltype(kTConfig);
		using InnerValueType = InnerValueType<kTConfig>;
		static_assert(!std::is_void_v<InnerValueType>,
						  "determineInnerValueType() not defined for this value config.");
		InnerValueType inner_value;// : unwrapValue(kTConfig.bit_width);

		friend constexpr auto unwrapValue(Value value) { return value.inner_value; }
	};

	struct Name1;
	struct Name2;
	struct Name3;

	// struct Absolute;
	// struct Relative;

	constexpr auto kConfigIndex1 =
		 config::withBitWidth(config::AbsoluteInteger<Name1>(), ValueBitWidth { 16 });//) {
//		{ { .signedness = config::Signedness::Unsigned, .bit_width = ValueBitWidth { 16 } } }
//	};

	constexpr auto kConfigCount1 =
		 config::withBitWidth(config::RelativeInteger<Name1>(), ValueBitWidth { 16 }); //) {

	constexpr config::Float<Name1> kConfigScalar1 = {
		
	};

	constexpr auto kConfigSmallIndex1 =
		 config::withBitWidth(config::AbsoluteInteger<Name3>(), ValueBitWidth { 6 }); //) {

	using Index1 = Value<kConfigIndex1>;
	constexpr Index1 index1 { 1 };
	static_assert(unwrapValue(index1) == 1);

	using Count1 = Value<kConfigCount1>;
	constexpr Count1 count1 { 2 };
	static_assert(unwrapValue(count1) == 2);

	using Scalar1 = Value<kConfigScalar1>;
	constexpr Scalar1 scalar1 { 3 };
	static_assert(unwrapValue(scalar1) == 3);

	using Small1 = Value<kConfigSmallIndex1>;
	constexpr Small1 small1 { 4 };
	static_assert(unwrapValue(small1) == 4);

}
