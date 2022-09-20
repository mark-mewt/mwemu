
#pragma once

namespace mewt::types {

	template <typename _Type>
		requires(std::is_same_v<_Type, std::decay_t<_Type>>)
	struct DefaultTraits
	{
	};

	template <typename _Type>
		requires(std::is_same_v<_Type, std::decay_t<_Type>>)
	struct ClassId
	{
		using Type = _Type;
	};
	template <typename _Type>
	using ClassIdOf = ClassId<std::decay_t<_Type>>;

	template <typename _Type>
	constexpr auto getTypeTraits(ClassId<_Type>) -> ClassId<DefaultTraits<_Type>>;

	template <typename _Type>
	constexpr auto dummyArg() -> _Type;

	namespace adl {
		template <typename _Type>
		struct TraitDetector
		{
			using Traits = typename decltype(getTypeTraits(ClassIdOf<_Type>()))::Type;
		};
	}

	template <typename _Type>
	using Traits = typename adl::TraitDetector<_Type>::Traits;
	//struct traits : public default_traits<_Type> {
	//};

	template <typename _Type>
	constexpr _Type copyOf(const _Type& object) { return object; }
}
