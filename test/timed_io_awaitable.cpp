#include <gtest/gtest.h>

#include <array>
#include <cstring>
#include <functional>

#include <sys/socket.h>
#include <unistd.h>

#include <ev++.h>
#include <wwa/utils/scope_action.h>

#include "eager_coroutine.h"
#include "timed_io_awaitable.h"
#include "utils.h"

TEST(TimedIOAwaitable, IOEvent)
{
    auto loop = ev::dynamic_loop();
    bool done = false;

    std::array<int, 2> fds{};
    ASSERT_NE(socketpair(AF_LOCAL, SOCK_STREAM, 0, fds.data()), -1);

    const wwa::utils::exit_action close_guard([&fds] {
        close(fds[0]);
        close(fds[1]);
    });

    const psb::timed_io_awaitable awaitable(loop, fds[0], ev::WRITE, 1.0);
    [](psb::timed_io_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        auto what = co_await aw;
        EXPECT_EQ(
            what & (make_unsigned(ev::WRITE) | make_unsigned(ev::READ) | make_unsigned(ev::TIMER)),
            make_unsigned(ev::WRITE)
        );
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}

TEST(TimedIOAwaitable, Timeout)
{
    auto loop = ev::dynamic_loop();
    bool done = false;

    std::array<int, 2> fds{};
    ASSERT_NE(socketpair(AF_LOCAL, SOCK_STREAM, 0, fds.data()), -1);

    const wwa::utils::exit_action close_guard([&fds] {
        close(fds[0]);
        close(fds[1]);
    });

    const psb::timed_io_awaitable awaitable(loop, fds[0], ev::READ, 0.0);
    [](psb::timed_io_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        auto what = co_await aw;
        EXPECT_EQ(
            what & (make_unsigned(ev::WRITE) | make_unsigned(ev::READ) | make_unsigned(ev::TIMER)),
            make_unsigned(ev::TIMER)
        );
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}
