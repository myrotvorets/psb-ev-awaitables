#ifndef DD97EA05_3EA2_4637_AA42_F9AB0AF8AE33
#define DD97EA05_3EA2_4637_AA42_F9AB0AF8AE33

#include <coroutine>
#include <memory>
#include <ev++.h>

#include "psb_ev_awaitables_export.h"

namespace psb {

class async_awaitable_private;
class PSB_EV_AWAITABLES_EXPORT async_awaitable {
public:
    explicit async_awaitable(ev::loop_ref loop, bool one_shot);
    ~async_awaitable();

    async_awaitable(const async_awaitable&)            = default;
    async_awaitable(async_awaitable&&)                 = default;
    async_awaitable& operator=(const async_awaitable&) = default;
    async_awaitable& operator=(async_awaitable&&)      = default;

    void trigger() noexcept;

    [[nodiscard]] bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    void await_resume() const noexcept;

private:
    std::shared_ptr<async_awaitable_private> d_ptr;
};

}  // namespace psb

#endif /* DD97EA05_3EA2_4637_AA42_F9AB0AF8AE33 */
