#ifndef D84CD005_D4BC_456E_B20B_3CF8BCD0A884
#define D84CD005_D4BC_456E_B20B_3CF8BCD0A884

#include <coroutine>
#include <memory>
#include <ev++.h>

#include "psb_ev_awaitables_export.h"

namespace psb {

class io_awaitable_private;
class PSB_EV_AWAITABLES_EXPORT io_awaitable {
public:
    io_awaitable(ev::loop_ref loop, int fd, int events, bool one_shot);
    ~io_awaitable();

    io_awaitable(const io_awaitable&)            = default;
    io_awaitable(io_awaitable&&)                 = default;
    io_awaitable& operator=(const io_awaitable&) = default;
    io_awaitable& operator=(io_awaitable&&)      = default;

    [[nodiscard]] bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    [[nodiscard]] unsigned int await_resume() const noexcept;

private:
    std::shared_ptr<io_awaitable_private> d_ptr;
};

}  // namespace psb

#endif /* D84CD005_D4BC_456E_B20B_3CF8BCD0A884 */
