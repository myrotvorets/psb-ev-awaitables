#include "interval_awaitable.h"
#include "interval_awaitable_p.h"

namespace psb {

interval_awaitable::interval_awaitable(ev::loop_ref loop, ev::tstamp after, ev::tstamp interval)
    : d_ptr(std::make_shared<interval_awaitable_private>(loop, after, interval))
{}

interval_awaitable::~interval_awaitable() = default;

void interval_awaitable::set_interval(ev::tstamp interval) noexcept
{
    this->d_ptr->set_interval(interval);
}

void interval_awaitable::stop() noexcept
{
    this->d_ptr->stop();
}

void interval_awaitable::again() noexcept
{
    this->d_ptr->again();
}

ev::tstamp interval_awaitable::remaining()
{
    return this->d_ptr->remaining();
}

bool interval_awaitable::await_ready() const noexcept
{
    return this->d_ptr->is_ready();
}

void interval_awaitable::await_suspend(std::coroutine_handle<> handle) noexcept
{
    this->d_ptr->start_watcher(handle);
}

void interval_awaitable::await_resume() const noexcept
{
    this->d_ptr->clear_pending();
}

}  // namespace psb
