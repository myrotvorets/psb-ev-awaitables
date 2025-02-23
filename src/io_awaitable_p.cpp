#include "io_awaitable_p.h"
#include <utility>

namespace psb {

io_awaitable_private::io_awaitable_private(ev::loop_ref loop, int fd, int events, bool one_shot) noexcept
    : m_io(loop), m_one_shot(one_shot)
{
    this->m_io.set(fd, events);
    this->m_io.set<io_awaitable_private, &io_awaitable_private::on_event>(this);
}

void io_awaitable_private::start_watcher(std::coroutine_handle<> handle) noexcept
{
    this->m_handle = handle;
    if (!this->m_io.is_active()) {
        this->m_io.start();
    }
}

void io_awaitable_private::on_event(ev::io& io, int revents)
{
    if (this->m_one_shot) {
        io.stop();
    }

    if (this->m_handle && !this->m_handle.done()) {
        this->m_result = static_cast<unsigned int>(revents);
        std::exchange(this->m_handle, nullptr).resume();
    }
    else {
        this->m_result |= static_cast<unsigned int>(revents);
    }
}

}  // namespace psb
