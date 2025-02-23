#ifndef C6497306_B6AA_45A5_8353_568D979CB28F
#define C6497306_B6AA_45A5_8353_568D979CB28F

#include <coroutine>
#include <ev++.h>

namespace psb {

class async_awaitable_private {
public:
    async_awaitable_private(ev::loop_ref loop, bool one_shot) noexcept;

    void start_watcher(std::coroutine_handle<> handle) noexcept;
    void trigger() noexcept { this->m_async.send(); }
    [[nodiscard]] bool is_ready() const noexcept { return this->m_pending; }
    void clear_pending() noexcept { this->m_pending = false; }

private:
    ev::async m_async;
    std::coroutine_handle<> m_handle;
    bool m_one_shot;
    bool m_pending = false;

    void on_event(ev::async& async, int revents);
};

}  // namespace psb

#endif /* C6497306_B6AA_45A5_8353_568D979CB28F */
