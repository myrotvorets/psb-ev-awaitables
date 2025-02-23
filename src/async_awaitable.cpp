#include "async_awaitable.h"
#include "async_awaitable_p.h"

namespace psb {

async_awaitable::async_awaitable(ev::loop_ref loop, bool one_shot)
    : d_ptr(std::make_shared<async_awaitable_private>(loop, one_shot))
{}

async_awaitable::~async_awaitable() = default;

void async_awaitable::trigger() noexcept
{
    this->d_ptr->trigger();
}

bool async_awaitable::await_ready() const noexcept
{
    return this->d_ptr->is_ready();
}

void async_awaitable::await_suspend(std::coroutine_handle<> handle) noexcept
{
    this->d_ptr->start_watcher(handle);
}

void async_awaitable::await_resume() const noexcept
{
    this->d_ptr->clear_pending();
}

}  // namespace psb
