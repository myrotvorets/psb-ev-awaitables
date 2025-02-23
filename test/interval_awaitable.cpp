#include <gtest/gtest.h>

#include <functional>

#include <ev++.h>

#include "eager_coroutine.h"
#include "interval_awaitable.h"

TEST(IntervalAwaitable, Basic)
{
    constexpr ev::tstamp interval = 0.01;
    auto loop                     = ev::dynamic_loop();
    int counter                   = 0;

    const psb::interval_awaitable awaitable(loop, interval, interval);

    [](psb::interval_awaitable aw, std::reference_wrapper<int> ctr) -> eager_coroutine {
        while (ctr.get() < 2) {
            co_await aw;
            ctr.get()++;
        }
    }(awaitable, counter);

    EXPECT_EQ(counter, 0);

    loop.run(ev::ONCE);
    EXPECT_EQ(counter, 1);

    loop.run(ev::ONCE);
    EXPECT_EQ(counter, 2);

    loop.run(ev::ONCE);
    EXPECT_EQ(counter, 2);
}

TEST(IntervalAwaitable, Restart)
{
    constexpr ev::tstamp interval     = 0.01;
    constexpr ev::tstamp new_interval = 0.02;

    auto loop = ev::dynamic_loop();
    bool done = false;

    const psb::interval_awaitable awaitable(loop, 0, interval);

    [](psb::interval_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        aw.set_interval(new_interval);
        aw.again();
        co_await aw;
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}

TEST(IntervalAwaitable, Stop)
{
    constexpr ev::tstamp interval = 1.0;

    auto loop = ev::dynamic_loop();
    bool done = false;

    psb::interval_awaitable awaitable(loop, interval, interval);

    [](psb::interval_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);
    loop.run(ev::ONCE | ev::NOWAIT);

    EXPECT_FALSE(done);
    EXPECT_LE(awaitable.remaining(), interval);

    awaitable.stop();
    loop.run(ev::ONCE);
    EXPECT_FALSE(done);

    awaitable.set_interval(0.01);  // NOLINT(readability-magic-numbers)
    awaitable.again();
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}
