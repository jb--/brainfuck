cmake_minimum_required(VERSION 3.1)

project(html
    LANGUAGES C
    VERSION 2.7.3
)

include(CTest)

option(ENABLE_CLI "Enable the command line interface." ON)
option(ENABLE_EDITLINE "Enable GNU readline functionality provided by the editline library." ON)
option(ENABLE_EXTENSION_DEBUG "Enable the debug extension for html.")
option(INSTALL_EXAMPLES "Installs the examples.")

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# getopt shim for windows
add_library(getopt INTERFACE)
if (MSVC)
    target_include_directories(getopt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/deps/getopt)
endif()

add_library(html include/html.h src/html.c)
set_target_properties(html PROPERTIES
    PUBLIC_HEADER "include/html.h"
    C_STANDARD 90
)
target_include_directories(html PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    $<INSTALL_INTERFACE:include/html/>
)
target_compile_definitions(html PUBLIC
    "-DHTML_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
    "-DHTML_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
    "-DHTML_VERSION_PATCH=${PROJECT_VERSION_PATCH}"
)
install(TARGETS html
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/html
)

if(ENABLE_CLI)
    add_executable(html-cli src/main.c)
    set_target_properties(html-cli PROPERTIES
        OUTPUT_NAME "html"
        C_STANDARD 90
    )
    target_link_libraries(html-cli html getopt)
    install(TARGETS html-cli RUNTIME DESTINATION bin)
    install(FILES man/html.1 DESTINATION "share/man/man1")

    if(ENABLE_EDITLINE)
        target_compile_definitions(html-cli PRIVATE "-DHTML_EDITLINE_LIB")
        target_link_libraries(html-cli -ledit)
    endif()
endif()

if(ENABLE_EXTENSION_DEBUG)
    add_definitions("-DHTML_EXTENSION_DEBUG")
endif()

if(INSTALL_EXAMPLES)
    set(EXAMPLES_DIR "share/html")
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${EXAMPLES_DIR})
    install(DIRECTORY examples DESTINATION ${EXAMPLES_DIR})
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
