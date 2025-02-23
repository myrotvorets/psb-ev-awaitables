#ifndef CDF2D584_273C_4EF7_8541_0B51C472D7B3
#define CDF2D584_273C_4EF7_8541_0B51C472D7B3

#include <coroutine>
#include <ev++.h>

namespace psb {

class signal_awaitable_private {
public:
    signal_awaitable_private(ev::loop_ref loop, int num, bool one_shot) noexcept;

    void start_watcher(std::coroutine_handle<> handle) noexcept;
    [[nodiscard]] bool is_ready() const noexcept { return this->m_pending; }
    void clear_pending() noexcept { this->m_pending = false; }

private:
    ev::sig m_sig;
    std::coroutine_handle<> m_handle;
    bool m_one_shot;
    bool m_pending = false;

    void on_event(ev::sig& sig, int revents);
};

}  // namespace psb

#endif /* CDF2D584_273C_4EF7_8541_0B51C472D7B3 */
