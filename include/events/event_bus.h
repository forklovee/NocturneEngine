#pragma once

#include <concepts>
#include <functional>
#include <typeindex>

namespace NocEngine {
	struct IEvent {};

	template<typename T>
	concept TEvent = std::derived_from<T, IEvent>;

	class EventBus
	{
		template<TEvent T>
		using CallbackFunction = std::function<void(const T&)>;

	public:
		template<TEvent T>
		void Subscribe(CallbackFunction<T> callback) {
			m_observers[typeid(T)].push_back(
				[callback](const IEvent& e) { // Type-erased callback in lambda, so it can be stored in one non-static map.
					callback(static_cast<const T&>(e));
				}
			);
		}

		template<typename TEvent>
		void Notify(const TEvent& event) {
			std::type_index type = typeid(TEvent);
			auto it = m_observers.find(type);
			if (it == m_observers.end()) {
				return;
			}
			for (auto& callback : m_observers.at(type)) {
				callback(event);
			}
		}

	private:
		std::unordered_map<std::type_index, std::vector<CallbackFunction<IEvent>>> m_observers;

	};
}