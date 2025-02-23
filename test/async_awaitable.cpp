#include <gtest/gtest.h>

#include <functional>

#include <ev++.h>

#include "async_awaitable.h"
#include "eager_coroutine.h"

TEST(AsyncAwaitable, OneShot)
{
    auto loop = ev::dynamic_loop();
    bool done = false;
    psb::async_awaitable awaitable(loop, true);

    [](psb::async_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);

    loop.run(ev::ONCE | ev::NOWAIT);
    EXPECT_FALSE(done);

    awaitable.trigger();
    EXPECT_FALSE(done);

    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}

TEST(AsyncAwaitable, Continuous)
{
    auto loop = ev::dynamic_loop();
    bool done = false;
    psb::async_awaitable awaitable(loop, false);

    const auto make_coro = [](psb::async_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    };

    make_coro(awaitable, done);
    // Will suspend on co_await
    EXPECT_FALSE(done);

    awaitable.trigger();
    // Will not resume because the loop is not running
    EXPECT_FALSE(done);

    loop.run(ev::ONCE);
    // Will resume because the async event has been sent
    EXPECT_TRUE(done);

    done = false;

    awaitable.trigger();
    loop.run(ev::ONCE);
    // Will not be set because nobody's listening
    EXPECT_FALSE(done);

    make_coro(awaitable, done);
    // Will not suspend because the event has already been triggered
    EXPECT_TRUE(done);

    done = false;
    make_coro(awaitable, done);
    // Will suspend because all pending events have been cleared
    EXPECT_FALSE(done);

    awaitable.trigger();
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}
