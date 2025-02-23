#include "timed_io_awaitable_p.h"

#include <ev++.h>

namespace psb {

timed_io_awaitable_private::timed_io_awaitable_private(
    ev::loop_ref loop, int fd, int events, ev::tstamp timeout
) noexcept
    : m_loop(loop), m_timeout(timeout), m_fd(fd), m_events(events)
{}

void timed_io_awaitable_private::start_watcher(std::coroutine_handle<> handle) noexcept
{
    this->m_handle = handle;
    this->m_loop.once<timed_io_awaitable_private, &timed_io_awaitable_private::on_event>(
        this->m_fd, this->m_events, this->m_timeout, this
    );
}

void timed_io_awaitable_private::on_event(int revents)
{
    this->m_result = static_cast<unsigned int>(revents);
    this->m_handle.resume();
}

}  // namespace psb
