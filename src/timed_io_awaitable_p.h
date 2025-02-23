#ifndef A716D4F3_5FC0_4EAC_8B3F_FC7A63A4A12D
#define A716D4F3_5FC0_4EAC_8B3F_FC7A63A4A12D

#include <coroutine>
#include <ev++.h>

namespace psb {

class timed_io_awaitable_private {
public:
    timed_io_awaitable_private(ev::loop_ref loop, int fd, int events, ev::tstamp timeout) noexcept;

    void start_watcher(std::coroutine_handle<> handle) noexcept;
    [[nodiscard]] unsigned int result() const noexcept { return this->m_result; }

private:
    ev::loop_ref m_loop;
    std::coroutine_handle<> m_handle;
    ev::tstamp m_timeout;
    int m_fd;
    int m_events;
    unsigned int m_result = 0;

    void on_event(int revents);
};

}  // namespace psb

#endif /* A716D4F3_5FC0_4EAC_8B3F_FC7A63A4A12D */
