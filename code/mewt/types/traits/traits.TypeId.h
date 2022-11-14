
#pragma once

namespace mewt::types::traits
{

	template <typename TType>
	struct TypeId
	{
		using Type = TType;
		static constexpr bool kIsConst = false;
		static constexpr bool kIsReference = false;
		static constexpr bool kIsPointer = false;
	};

	template <typename TType>
	struct TypeId<const TType> : public TypeId<TType>
	{
		static constexpr bool kIsConst = true;
	};

	template <typename TType>
	struct TypeId<TType&> : public TypeId<TType>
	{
		static constexpr bool kIsReference = true;
	};

	template <typename TType>
	struct TypeId<TType*> : public TypeId<TType>
	{
		static constexpr bool kIsPointer = true;
	};

	template <typename TType>
	constexpr auto kTypeId = TypeId<TType> {};

}
