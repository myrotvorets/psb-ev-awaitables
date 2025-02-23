#include "signal_awaitable_p.h"

#include <utility>

namespace psb {

signal_awaitable_private::signal_awaitable_private(ev::loop_ref loop, int num, bool one_shot) noexcept
    : m_sig(loop), m_one_shot(one_shot)
{
    this->m_sig.set(num);
    this->m_sig.set<signal_awaitable_private, &signal_awaitable_private::on_event>(this);
}

void signal_awaitable_private::start_watcher(std::coroutine_handle<> handle) noexcept
{
    this->m_handle = handle;
    if (!this->m_sig.is_active()) {
        this->m_sig.start();
    }
}

void signal_awaitable_private::on_event(ev::sig& sig, int)
{
    if (this->m_one_shot) {
        sig.stop();
    }

    if (this->m_handle && !this->m_handle.done()) {
        this->m_pending = false;
        std::exchange(this->m_handle, nullptr).resume();
    }
    else {
        this->m_pending = true;
    }
}

}  // namespace psb
