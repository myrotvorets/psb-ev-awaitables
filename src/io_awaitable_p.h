#ifndef D2A804AE_3573_4271_AFF6_6D8278CF15AA
#define D2A804AE_3573_4271_AFF6_6D8278CF15AA

#include <coroutine>
#include <utility>
#include <ev++.h>

namespace psb {

class io_awaitable_private {
public:
    io_awaitable_private(ev::loop_ref loop, int fd, int events, bool one_shot) noexcept;

    void start_watcher(std::coroutine_handle<> handle) noexcept;
    [[nodiscard]] bool is_ready() const noexcept { return this->m_result != 0; }
    [[nodiscard]] unsigned int result() noexcept { return std::exchange(this->m_result, 0U); }

private:
    ev::io m_io;
    std::coroutine_handle<> m_handle;
    unsigned int m_result = 0;
    bool m_one_shot;

    void on_event(ev::io& io, int revents);
};

}  // namespace psb

#endif /* D2A804AE_3573_4271_AFF6_6D8278CF15AA */
