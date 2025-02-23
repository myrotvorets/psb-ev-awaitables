#include "async_awaitable_p.h"

#include <utility>

namespace psb {

async_awaitable_private::async_awaitable_private(ev::loop_ref loop, bool one_shot) noexcept
    : m_async(loop), m_one_shot(one_shot)
{
    this->m_async.set<async_awaitable_private, &async_awaitable_private::on_event>(this);
}

void async_awaitable_private::start_watcher(std::coroutine_handle<> handle) noexcept
{
    this->m_handle = handle;
    if (!this->m_async.is_active()) {
        this->m_async.start();
    }
}

void async_awaitable_private::on_event(ev::async& async, int)
{
    if (this->m_one_shot) {
        async.stop();
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
