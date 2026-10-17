#include <gtest/gtest.h>

#include <optional>
#include <vector>

#include <Game/Signal.h>
using namespace lhs::game;


TEST(SignalTest, CallsEverySubscriber)
{
	Signal<int> signal;
	std::vector<int> received;

	auto first = signal.Subscribe([&](int value) { received.push_back(value); });
	auto second = signal.Subscribe([&](int value) { received.push_back(value * 10); });
	signal.Emit(1);

	EXPECT_EQ(received, (std::vector<int>{ 1, 10 }));
}

TEST(SignalTest, StopsCallingAfterTheSubscriptionIsDestroyed)
{
	Signal<int> signal;
	int calls = 0;

	{
		auto subscription = signal.Subscribe([&](int) { ++calls; });
		signal.Emit(0);
	}
	signal.Emit(0);

	EXPECT_EQ(calls, 1);
}

TEST(SignalTest, MovingASubscriptionKeepsItActive)
{
	Signal<int> signal;
	int calls = 0;

	Subscription moved;
	{
		auto subscription = signal.Subscribe([&](int) { ++calls; });
		moved = std::move(subscription);
	}
	signal.Emit(0);

	EXPECT_EQ(calls, 1);
}

TEST(SignalTest, ASubscriptionMayOutliveItsSignal)
{
	Subscription subscription;
	{
		Signal<int> signal;
		subscription = signal.Subscribe([](int) {});
	}
	subscription.Reset();

	SUCCEED();
}

TEST(SignalTest, AListenerMayUnsubscribeAnotherWhileRunning)
{
	Signal<int> signal;
	int laterCalls = 0;

	std::optional<Subscription> later;
	auto first = signal.Subscribe([&](int) { later.reset(); });
	later = signal.Subscribe([&](int) { ++laterCalls; });
	signal.Emit(0);

	EXPECT_EQ(laterCalls, 0);
}
