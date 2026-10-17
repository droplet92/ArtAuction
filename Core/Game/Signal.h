#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <utility>
#include <vector>


namespace lhs::game
{
	/**
	* A subscription to a Signal
	*
	* Destroying it stops the calls, so a listener never outlives its owner.
	* It is safe to destroy it after the Signal is gone.
	*/
	class Subscription
	{
	public:
		Subscription() = default;

		explicit Subscription(std::function<void()> cancel) noexcept
			: cancel(std::move(cancel))
		{
		}

		Subscription(Subscription&& other) noexcept
			: cancel(std::exchange(other.cancel, {}))
		{
		}

		Subscription& operator=(Subscription&& other) noexcept
		{
			if (this != &other)
			{
				Reset();
				cancel = std::exchange(other.cancel, {});
			}
			return *this;
		}

		Subscription(const Subscription&) = delete;
		Subscription& operator=(const Subscription&) = delete;

		~Subscription()
		{
			Reset();
		}

		// Stop the calls now.
		void Reset() noexcept
		{
			if (cancel)
				std::exchange(cancel, {})();
		}

	private:
		std::function<void()> cancel;
	};

	/**
	* A list of listeners to call with an event
	*/
	template <typename Event>
	class Signal
	{
	public:
		using Listener = std::function<void(const Event&)>;

		[[nodiscard]] Subscription Subscribe(Listener listener)
		{
			auto id = state->nextId++;
			state->listeners.emplace(id, std::move(listener));

			return Subscription{ [weak = std::weak_ptr<State>(state), id]()
				{
					if (auto state = weak.lock())
						state->listeners.erase(id);
				} };
		}

		void Emit(const Event& event) const
		{
			// A listener may subscribe or unsubscribe others while it runs.
			std::vector<uint64_t> ids;
			for (const auto& [id, _] : state->listeners)
				ids.push_back(id);

			for (auto id : ids)
			{
				auto listener = state->listeners.find(id);
				if (listener != state->listeners.end())
					std::invoke(Listener{ listener->second }, event);
			}
		}

	private:
		struct State
		{
			uint64_t nextId = 0;
			std::map<uint64_t, Listener> listeners;
		};
		std::shared_ptr<State> state = std::make_shared<State>();
	};
}
