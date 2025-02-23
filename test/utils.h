#ifndef F9AEB197_4D43_43AC_AFFC_7A8D7FCB486C
#define F9AEB197_4D43_43AC_AFFC_7A8D7FCB486C

#include <type_traits>

template<typename T>
constexpr auto make_unsigned(T value) noexcept
{
    return static_cast<std::make_unsigned_t<T>>(value);
}

#endif /* F9AEB197_4D43_43AC_AFFC_7A8D7FCB486C */
