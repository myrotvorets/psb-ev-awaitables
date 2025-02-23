#include "timeout_awaitable_p.h"

namespace psb {

timeout_awaitable_private::timeout_awaitable_private(ev::loop_ref loop, ev::tstamp timeout) noexcept : m_timer(loop)
{
    this->m_timer.set<timeout_awaitable_private, &timeout_awaitable_private::on_event>(this);
    this->m_timer.set(timeout, 0.0);
}

void timeout_awaitable_private::start_watcher(std::coroutine_handle<> handle) noexcept
{
    this->m_handle = handle;
    this->m_timer.start();
}

void timeout_awaitable_private::on_event(ev::timer& timer, int)
{
    timer.stop();
    if (this->m_handle && !this->m_handle.done()) {
        this->m_handle.resume();
    }
}

}  // namespace psb
