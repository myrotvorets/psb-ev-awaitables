#ifndef E92CFF64_DE39_490B_92C4_F81ED7F10210
#define E92CFF64_DE39_490B_92C4_F81ED7F10210

#include <coroutine>
#include <ev++.h>

namespace psb {

class timeout_awaitable_private {
public:
    timeout_awaitable_private(ev::loop_ref loop, ev::tstamp timeout) noexcept;

    void start_watcher(std::coroutine_handle<> handle) noexcept;

private:
    ev::timer m_timer;
    std::coroutine_handle<> m_handle;

    void on_event(ev::timer& timer, int revents);
};

}  // namespace psb

#endif /* E92CFF64_DE39_490B_92C4_F81ED7F10210 */
