#ifndef CA59B44E_1AF6_46AC_95E1_FB0324A9FA1A
#define CA59B44E_1AF6_46AC_95E1_FB0324A9FA1A

#include <coroutine>
#include <memory>
#include <ev++.h>

#include "psb_ev_awaitables_export.h"

namespace psb {

class timed_io_awaitable_private;
class PSB_EV_AWAITABLES_EXPORT timed_io_awaitable {
public:
    timed_io_awaitable(ev::loop_ref loop, int fd, int events, ev::tstamp timeout);
    ~timed_io_awaitable();

    timed_io_awaitable(const timed_io_awaitable&)            = default;
    timed_io_awaitable(timed_io_awaitable&&)                 = default;
    timed_io_awaitable& operator=(const timed_io_awaitable&) = default;
    timed_io_awaitable& operator=(timed_io_awaitable&&)      = default;

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    [[nodiscard]] unsigned int await_resume() const noexcept;

private:
    std::shared_ptr<timed_io_awaitable_private> d_ptr;
};

}  // namespace psb

#endif /* CA59B44E_1AF6_46AC_95E1_FB0324A9FA1A */
