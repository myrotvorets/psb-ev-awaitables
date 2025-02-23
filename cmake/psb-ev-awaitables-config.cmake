get_filename_component(PSB_EV_AWAITABLES_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

list(APPEND CMAKE_MODULE_PATH ${PSB_EV_AWAITABLES_CMAKE_DIR})

if(NOT TARGET psb-ev-awaitables)
    include("${PSB_EV_AWAITABLES_CMAKE_DIR}/psb-ev-awaitables-target.cmake")
    add_library(psb::ev::awaitables ALIAS psb-ev-awaitables)
endif()
