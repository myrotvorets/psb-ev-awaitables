#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <cstring>
#include <functional>

#include <sys/socket.h>
#include <unistd.h>

#include <ev++.h>
#include <wwa/utils/scope_action.h>

#include "eager_coroutine.h"
#include "io_awaitable.h"
#include "utils.h"

TEST(IOAwaitable, OneShot)
{
    auto loop = ev::dynamic_loop();
    bool done = false;

    std::array<int, 2> fds{};
    ASSERT_NE(socketpair(AF_LOCAL, SOCK_STREAM, 0, fds.data()), -1);

    const wwa::utils::exit_action close_guard([&fds] {
        close(fds[0]);
        close(fds[1]);
    });

    const psb::io_awaitable awaitable(loop, fds[0], ev::WRITE, true);
    [](psb::io_awaitable aw, std::reference_wrapper<bool> flag) -> eager_coroutine {
        auto what = co_await aw;
        EXPECT_EQ(what & make_unsigned(ev::WRITE), make_unsigned(ev::WRITE));
        flag.get() = true;
    }(awaitable, done);

    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}

TEST(IOAwaitable, Continuous)
{
    auto loop = ev::dynamic_loop();
    bool done = false;

    std::array<int, 2> fds{};
    ASSERT_NE(socketpair(AF_LOCAL, SOCK_STREAM, 0, fds.data()), -1);

    const wwa::utils::exit_action close_guard([&fds] {
        close(fds[0]);
        close(fds[1]);
    });

    const psb::io_awaitable awaitable(loop, fds[0], make_unsigned(ev::READ) | make_unsigned(ev::WRITE), false);
    const auto make_coro = [](psb::io_awaitable aw, std::reference_wrapper<bool> flag,
                              unsigned int v) -> eager_coroutine {
        auto what = co_await aw;
        EXPECT_EQ(what & (make_unsigned(ev::READ) | make_unsigned(ev::WRITE)), v);
        flag.get() = true;
    };

    make_coro(awaitable, done, make_unsigned(ev::WRITE));
    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);

    done = false;
    EXPECT_EQ(write(fds[1], "hello", std::strlen("hello")), std::strlen("hello"));

    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_FALSE(done);

    make_coro(awaitable, done, make_unsigned(ev::READ) | make_unsigned(ev::WRITE));
    EXPECT_TRUE(done);

    done = false;

    constexpr std::size_t buf_size = 16U;
    std::array<char, buf_size> buffer{};
    ASSERT_GT(read(fds[0], buffer.data(), buffer.size()), 0);

    make_coro(awaitable, done, make_unsigned(ev::WRITE));
    EXPECT_FALSE(done);
    loop.run(ev::ONCE);
    EXPECT_TRUE(done);
}
