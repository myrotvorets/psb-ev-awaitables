#include <gtest/gtest.h>

#include <csignal>
#include <functional>

#include <ev++.h>

#include "eager_coroutine.h"
#include "signal_awaitable.h"

TEST(SignalAwaitable, RealSignal)
{
    auto loop = ev::dynamic_loop();
    bool done = false;
    const psb::signal_awaitable awaitable(loop, SIGCHLD, true);

    [](psb::signal_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);
    ASSERT_EQ(std::raise(SIGCHLD), 0);

    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}

TEST(SignalAwaitable, FeedSignal)
{
    auto loop = ev::dynamic_loop();
    bool done = false;
    const psb::signal_awaitable awaitable(loop, SIGCHLD, true);

    [](psb::signal_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);

    loop.feed_signal_event(SIGCHLD);
    EXPECT_FALSE(done);

    loop.run(ev::ONCE | ev::NOWAIT);
    EXPECT_TRUE(done);
}

TEST(SignalAwaitable, Continuous)
{
    auto loop = ev::dynamic_loop();
    bool done = false;
    const psb::signal_awaitable awaitable(loop, SIGCHLD, false);

    const auto make_coro = [](psb::signal_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    };

    make_coro(awaitable, done);
    // Will suspend on co_await because there has been no signal
    EXPECT_FALSE(done);

    loop.feed_signal_event(SIGCHLD);
    // Will not resume because the loop is not running
    EXPECT_FALSE(done);

    loop.run(ev::ONCE | ev::NOWAIT);
    // Will resume because the signal has been fed
    EXPECT_TRUE(done);

    done = false;
    loop.feed_signal_event(SIGCHLD);
    // Will not be set because nobody's listening
    EXPECT_FALSE(done);

    loop.run(ev::ONCE | ev::NOWAIT);
    // Still nobody is listening
    EXPECT_FALSE(done);

    make_coro(awaitable, done);
    // Will not suspend because of the pending signal
    EXPECT_TRUE(done);

    done = false;
    make_coro(awaitable, done);
    // Will suspend because all pending signals have been cleared
    EXPECT_FALSE(done);

    loop.feed_signal_event(SIGCHLD);
    loop.run(ev::ONCE | ev::NOWAIT);
    EXPECT_TRUE(done);
}
