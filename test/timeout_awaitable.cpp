#include <gtest/gtest.h>

#include <ev++.h>

#include "eager_coroutine.h"
#include "timeout_awaitable.h"

TEST(OneShotTimerAwaitable, Basic)
{
    auto loop = ev::dynamic_loop();
    bool done = false;
    const psb::timeout_awaitable awaitable(loop, 0);

    [](psb::timeout_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        co_await aw;
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);

    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}
