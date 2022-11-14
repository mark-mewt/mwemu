
#pragma once

#include "mewt/types/numeric/numeric.space.h"

namespace mewt::types::numeric::unit
{

	template <typename TUnit, typename TSpace>
	using NativeTypeForUnitNoCatchAll = typename decltype(getNativeTypeForUnit(
		 traits::kTypeId<TUnit>, traits::kTypeId<TSpace>))::Type;

	namespace detail
	{

		template <typename... TArgs>
		constexpr auto staticError();
		#if 0
		template <typename TUnit, typename TSpace>
		constexpr auto getNativeTypeForUnit(traits::TypeId<TUnit> unit, traits::TypeId<TSpace> space)
		{
			struct NoNativeTypeForUnit;
			constexpr bool nativeTypeIsDefined = sizeof(traits::TypeId<TUnit>) == 0;
			static_assert(nativeTypeIsDefined, "Native type has not been declared for this unit/space.");
			return unit::NativeTypeForUnitNoCatchAll<TUnit, TSpace> {};
			//getNativeTypeForUnit(unit, space, false);
			//return staticError<NoNativeTypeForUnit, TUnit, TSpace>();
		}
#endif
		template <typename TUnit, typename TSpace>
		using NativeTypeForUnit = typename decltype(getNativeTypeForUnit(
			 traits::kTypeId<TUnit>, traits::kTypeId<TSpace>))::Type;

	}

	///template <typename TUnit, space::isDiscrete TSpace>
	// By default, units in discrete spaces are measured in ints.
	///constexpr auto getNativeTypeForUnit(traits::TypeId<TUnit>, traits::TypeId<TSpace>)
	///	 -> traits::TypeId<int>;

	template <typename TUnit, typename TSpace>
	using NativeTypeForUnit = detail::NativeTypeForUnit<TUnit, TSpace>;

	template <typename TLeftUnit, typename TRightUnit, typename TSpace>
	concept isUnitEquivalent = declareEquivalentUnits(traits::kTypeId<TLeftUnit>,
																	  traits::kTypeId<TRightUnit>,
																	  traits::kTypeId<TSpace>) ||
										declareEquivalentUnits(traits::kTypeId<TRightUnit>,
																	  traits::kTypeId<TLeftUnit>,
																	  traits::kTypeId<TSpace>);

}