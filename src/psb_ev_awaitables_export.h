#ifndef B0F9416D_FFE6_4E5F_BFB3_6F7E06DBF703
#define B0F9416D_FFE6_4E5F_BFB3_6F7E06DBF703

#ifdef PSB_EV_AWAITABLES_STATIC_DEFINE
#    define PSB_EV_AWAITABLES_EXPORT
#    define PSB_EV_AWAITABLES_NO_EXPORT
#else
#    ifdef psb_ev_awaitables_EXPORTS
#        if defined _WIN32 || defined __CYGWIN__
#            define PSB_EV_AWAITABLES_EXPORT __declspec(dllexport)
#            define PSB_EV_AWAITABLES_NO_EXPORT
#        else
#            define PSB_EV_AWAITABLES_EXPORT    [[gnu::visibility("default")]]
#            define PSB_EV_AWAITABLES_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    else
#        if defined _WIN32 || defined __CYGWIN__
#            define PSB_EV_AWAITABLES_EXPORT __declspec(dllimport)
#            define PSB_EV_AWAITABLES_NO_EXPORT
#        else
#            define PSB_EV_AWAITABLES_EXPORT    [[gnu::visibility("default")]]
#            define PSB_EV_AWAITABLES_NO_EXPORT [[gnu::visibility("hidden")]]
#        endif
#    endif
#endif

#endif /* B0F9416D_FFE6_4E5F_BFB3_6F7E06DBF703 */
