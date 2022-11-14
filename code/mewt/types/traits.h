
#pragma once

#include <utility>

namespace mewt::types
{

	// template <typename TType>
	//	requires(std::is_same_v<TType, std::decay_t<TType>>)
	// struct DefaultTraits
	//{
	// };

	/*template <typename TType>
	struct ClassId
	{
		using Type = TType;
	};

	template <typename TType>
	struct ClassId<const TType> : public ClassId<TType>
	{
	};

	template <typename TType>
	struct ClassId<TType&> : public ClassId<TType>
	{
	};

	template <typename TType>
	struct ClassId<TType*> : public ClassId<TType>
	{
	};

	template <typename TType>
	constexpr auto kClassId = ClassId<TType> {};*/

	// template <typename TType>
	// constexpr auto getTypeTraits(ClassId<TType>) -> ClassId<DefaultTraits<TType>>;

	template <typename TType>
	constexpr auto dummyArg() -> TType;

	namespace adl
	{
		template <typename TType>
		struct TraitDetector
		{
			using Traits = typename decltype(getTypeTraits(ClassIdOf<TType>()))::Type;
		};
	}

	template <typename TType>
	using Traits = typename adl::TraitDetector<TType>::Traits;

	// struct traits : public default_traits<_Type> {
	// };

	template <typename TType>
	constexpr auto copyOf(const TType& object) -> TType
	{
		return object;
	}
}
