#include "signal_awaitable.h"
#include "signal_awaitable_p.h"

namespace psb {

signal_awaitable::signal_awaitable(ev::loop_ref loop, int num, bool one_shot)
    : d_ptr(std::make_shared<signal_awaitable_private>(loop, num, one_shot))
{}

signal_awaitable::~signal_awaitable() = default;

bool signal_awaitable::await_ready() const noexcept
{
    return this->d_ptr->is_ready();
}

void signal_awaitable::await_resume() const noexcept
{
    this->d_ptr->clear_pending();
}

void signal_awaitable::await_suspend(std::coroutine_handle<> handle) noexcept
{
    this->d_ptr->start_watcher(handle);
}

}  // namespace psb
