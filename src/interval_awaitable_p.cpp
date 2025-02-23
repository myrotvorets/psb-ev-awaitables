#include "interval_awaitable_p.h"

#include <utility>

namespace psb {

interval_awaitable_private::interval_awaitable_private(
    ev::loop_ref loop, ev::tstamp after, ev::tstamp interval
) noexcept
    : m_timer(loop)
{
    this->m_timer.set(after, interval);
    this->m_timer.set<interval_awaitable_private, &interval_awaitable_private::on_event>(this);
}

void interval_awaitable_private::start_watcher(std::coroutine_handle<> handle) noexcept
{
    this->m_handle = handle;
    if (!this->m_timer.is_active()) {
        this->m_timer.start();
    }
}

void interval_awaitable_private::on_event(ev::timer&, int)
{
    if (this->m_handle && !this->m_handle.done()) {
        this->m_pending = false;
        std::exchange(this->m_handle, nullptr).resume();
    }
    else {
        this->m_pending = true;
    }
}

}  // namespace psb
