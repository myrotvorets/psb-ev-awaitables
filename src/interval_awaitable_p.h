#ifndef A0CF4CBB_DDB8_43EE_99CE_84549F790605
#define A0CF4CBB_DDB8_43EE_99CE_84549F790605

#include <coroutine>
#include <ev++.h>

namespace psb {

class interval_awaitable_private {
public:
    interval_awaitable_private(ev::loop_ref loop, ev::tstamp after, ev::tstamp interval) noexcept;

    void start_watcher(std::coroutine_handle<> handle) noexcept;
    void set_interval(ev::tstamp interval) noexcept { this->m_timer.repeat = interval; }
    [[nodiscard]] bool is_ready() const noexcept { return this->m_pending; }
    void clear_pending() noexcept { this->m_pending = false; }
    void stop() noexcept { this->m_timer.stop(); }
    void again() noexcept { this->m_timer.again(); }
    [[nodiscard]] ev::tstamp remaining() { return this->m_timer.remaining(); }

private:
    ev::timer m_timer;
    std::coroutine_handle<> m_handle;
    bool m_pending = false;

    void on_event(ev::timer& timer, int revents);
};

}  // namespace psb

#endif /* A0CF4CBB_DDB8_43EE_99CE_84549F790605 */
