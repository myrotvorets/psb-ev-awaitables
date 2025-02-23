#ifndef A2CA5DE1_3F17_4F85_8FC4_45094078488B
#define A2CA5DE1_3F17_4F85_8FC4_45094078488B

#include <coroutine>
#include <memory>
#include <ev++.h>

#include "psb_ev_awaitables_export.h"

namespace psb {

class timeout_awaitable_private;
class PSB_EV_AWAITABLES_EXPORT timeout_awaitable {
public:
    explicit timeout_awaitable(ev::loop_ref loop, ev::tstamp timeout);
    ~timeout_awaitable();

    timeout_awaitable(const timeout_awaitable&)            = default;
    timeout_awaitable(timeout_awaitable&&)                 = default;
    timeout_awaitable& operator=(const timeout_awaitable&) = default;
    timeout_awaitable& operator=(timeout_awaitable&&)      = default;

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    [[nodiscard]] constexpr bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    void await_resume() const noexcept {}

private:
    std::shared_ptr<timeout_awaitable_private> d_ptr;
};

}  // namespace psb

#endif /* A2CA5DE1_3F17_4F85_8FC4_45094078488B */
