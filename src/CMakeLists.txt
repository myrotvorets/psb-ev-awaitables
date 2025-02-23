add_library("${PROJECT_NAME}")
target_sources(
    "${PROJECT_NAME}"
    PRIVATE
        async_awaitable.cpp
        async_awaitable_p.cpp
        interval_awaitable.cpp
        interval_awaitable_p.cpp
        io_awaitable.cpp
        io_awaitable_p.cpp
        signal_awaitable.cpp
        signal_awaitable_p.cpp
        timed_io_awaitable.cpp
        timed_io_awaitable_p.cpp
        timeout_awaitable.cpp
        timeout_awaitable_p.cpp
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
        FILES
            async_awaitable.h
            interval_awaitable.h
            io_awaitable.h
            psb_ev_awaitables_export.h
            signal_awaitable.h
            timed_io_awaitable.h
            timeout_awaitable.h
)

set_target_properties(
    "${PROJECT_NAME}"
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        POSITION_INDEPENDENT_CODE ON
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_link_libraries("${PROJECT_NAME}" PRIVATE unofficial::libev::libev)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PSB_EV_AWAITABLES_STATIC_DEFINE)
endif()

if(ENABLE_MAINTAINER_MODE)
    string(REPLACE " " ";" CXX_FLAGS_MM "${CMAKE_CXX_FLAGS_MM}")
    target_compile_options(${PROJECT_NAME} PRIVATE ${CXX_FLAGS_MM})
    unset(CXX_FLAGS_MM)
endif()

if(INSTALL_EV_AWAITABLES)
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs)

    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-target
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/psb/ev"
    )

    install(
        EXPORT ${PROJECT_NAME}-target
        FILE ${PROJECT_NAME}-target.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    write_basic_package_version_file(
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
        COPYONLY
    )

    install(
        FILES
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
            ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif()
