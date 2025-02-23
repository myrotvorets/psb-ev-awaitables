#include "timed_io_awaitable.h"
#include "timed_io_awaitable_p.h"

namespace psb {

timed_io_awaitable::timed_io_awaitable(ev::loop_ref loop, int fd, int events, ev::tstamp timeout)
    : d_ptr(std::make_shared<timed_io_awaitable_private>(loop, fd, events, timeout))
{}

timed_io_awaitable::~timed_io_awaitable() = default;

void timed_io_awaitable::await_suspend(std::coroutine_handle<> handle) noexcept
{
    this->d_ptr->start_watcher(handle);
}

unsigned int timed_io_awaitable::await_resume() const noexcept
{
    return this->d_ptr->result();
}

}  // namespace psb
