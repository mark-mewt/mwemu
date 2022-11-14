
#pragma once

#include "mewt/types/native/native.Integer.decl.h"
#include "mewt/types/numeric/numeric.space.h"
#include "mewt/types/traits/traits.TypeId.h"

namespace mewt::types::traits
{

	template <typename TType>
	struct Layout;

	template <typename TType>
	constexpr auto getNumericSpaceGranularity(TypeId<Layout<TType>>)
	{
		return numeric::space::Granularity::Discrete;
	}

	template <typename TType>
	constexpr auto getNativeTypeForUnit(TypeId<native::Byte>, TypeId<Layout<TType>>)
		 -> TypeId<std::size_t>; // mwToDo: Change this to depend on size of type.

	template <typename TType>
	constexpr auto getNativeTypeForUnit(TypeId<native::Bit>, TypeId<Layout<TType>>)
		 -> TypeId<std::uint8_t>; // mwToDo: Change this to depend on size of type.

	template <typename TType>
	concept isLayoutSpace = declareTypeAsLayoutSpace(kTypeId<TType>);

	template<typename TType>
	using LayoutOf = std::conditional_t<isLayoutSpace<TType>, TType, Layout<TType>>;

	//template <typename TType>
	//??constexpr auto kSizeOf = numeric::Count<Layout<native::Byte>, Layout<TType>> { sizeof(TType) };
	template <typename TType>
	constexpr auto kSizeOf = numeric::Count<native::Byte, LayoutOf<TType>> { sizeof(TType) };

	template <typename TType>
	struct Layout
	{
		//static constexpr auto kSize = kSizeOf<TType>;

		friend constexpr auto declareTypeAsLayoutSpace(TypeId<Layout>) { return true; }
	};

}
