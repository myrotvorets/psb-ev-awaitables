#ifndef ADC1A41B_7EA3_408F_86D7_301358B91DDC
#define ADC1A41B_7EA3_408F_86D7_301358B91DDC

#include <coroutine>
#include <memory>
#include <ev++.h>

#include "psb_ev_awaitables_export.h"

namespace psb {

class interval_awaitable_private;
class PSB_EV_AWAITABLES_EXPORT interval_awaitable {
public:
    explicit interval_awaitable(ev::loop_ref loop, ev::tstamp after, ev::tstamp interval);
    ~interval_awaitable();

    interval_awaitable(const interval_awaitable&)            = default;
    interval_awaitable(interval_awaitable&&)                 = default;
    interval_awaitable& operator=(const interval_awaitable&) = default;
    interval_awaitable& operator=(interval_awaitable&&)      = default;

    void set_interval(ev::tstamp interval) noexcept;
    void stop() noexcept;
    void again() noexcept;
    [[nodiscard]] ev::tstamp remaining();

    [[nodiscard]] bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    void await_resume() const noexcept;

private:
    std::shared_ptr<interval_awaitable_private> d_ptr;
};

}  // namespace psb

#endif /* ADC1A41B_7EA3_408F_86D7_301358B91DDC */
