#ifndef FF6CDB17_C311_4D57_B76B_F3F8AC144A72
#define FF6CDB17_C311_4D57_B76B_F3F8AC144A72

#include <coroutine>
#include <exception>

struct eager_coroutine {
    struct promise_type {
        // NOLINTBEGIN(readability-convert-member-functions-to-static)
        [[nodiscard]] eager_coroutine get_return_object() const noexcept { return eager_coroutine{}; }
        [[nodiscard]] auto initial_suspend() const noexcept { return std::suspend_never{}; }
        [[nodiscard]] auto final_suspend() const noexcept { return std::suspend_never{}; }
        void return_void() const noexcept {}
        [[noreturn]] void unhandled_exception() const { std::terminate(); }
        // NOLINTEND(readability-convert-member-functions-to-static)
    };
};

#endif /* FF6CDB17_C311_4D57_B76B_F3F8AC144A72 */
