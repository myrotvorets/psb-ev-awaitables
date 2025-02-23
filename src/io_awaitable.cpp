#include "io_awaitable.h"
#include "io_awaitable_p.h"

namespace psb {

io_awaitable::io_awaitable(ev::loop_ref loop, int fd, int events, bool one_shot)
    : d_ptr(std::make_shared<io_awaitable_private>(loop, fd, events, one_shot))
{}

io_awaitable::~io_awaitable() = default;

bool io_awaitable::await_ready() const noexcept
{
    return this->d_ptr->is_ready();
}

void io_awaitable::await_suspend(std::coroutine_handle<> handle) noexcept
{
    this->d_ptr->start_watcher(handle);
}

unsigned int io_awaitable::await_resume() const noexcept
{
    return this->d_ptr->result();
}

}  // namespace psb
