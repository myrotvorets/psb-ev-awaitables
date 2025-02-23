#include "timeout_awaitable.h"
#include "timeout_awaitable_p.h"

namespace psb {

timeout_awaitable::timeout_awaitable(ev::loop_ref loop, ev::tstamp timeout)
    : d_ptr(std::make_shared<timeout_awaitable_private>(loop, timeout))
{}

timeout_awaitable::~timeout_awaitable() = default;

void timeout_awaitable::await_suspend(std::coroutine_handle<> handle) noexcept
{
    this->d_ptr->start_watcher(handle);
}

}  // namespace psb
