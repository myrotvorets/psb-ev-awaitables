#ifndef B276B036_981D_4B7D_868D_5A923EB0AD72
#define B276B036_981D_4B7D_868D_5A923EB0AD72

#include <coroutine>
#include <memory>
#include <ev++.h>

#include "psb_ev_awaitables_export.h"

namespace psb {

class signal_awaitable_private;
class PSB_EV_AWAITABLES_EXPORT signal_awaitable {
public:
    signal_awaitable(ev::loop_ref loop, int num, bool one_shot);
    ~signal_awaitable();

    signal_awaitable(const signal_awaitable&)            = default;
    signal_awaitable(signal_awaitable&&)                 = default;
    signal_awaitable& operator=(const signal_awaitable&) = default;
    signal_awaitable& operator=(signal_awaitable&&)      = default;

    [[nodiscard]] bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    void await_resume() const noexcept;

private:
    std::shared_ptr<signal_awaitable_private> d_ptr;
};

}  // namespace psb

#endif /* B276B036_981D_4B7D_868D_5A923EB0AD72 */
