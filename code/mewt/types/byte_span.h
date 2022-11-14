
#pragma once

#include "mewt/types/address_space.h"

#include <span>

namespace mewt::types
{

	template <typename TObject>
	class ByteSpan : public std::span<native::Byte, sizeof(TObject)>
	{
		using Super = std::span<native::Byte, sizeof(TObject)>;

	public:
		inline explicit ByteSpan(TObject& object)
			 : Super(static_cast<native::Byte*>(static_cast<void*>(std::addressof(object))), sizeof(TObject))
		{
		}

		/// using Address = types::Index<LocalByte<std::bit_width(NExtent - 1)>, TSpace>;
		using Address = numeric::Index<native::Byte, traits::Layout<TObject>>;

		inline auto operator[](Address address) const
		{
			return Super::operator[](nativeValue(address));
		}

		inline auto operator[](Address address) -> auto&
		{
			return Super::operator[](native::valueOf(address));
		}
	};

}
