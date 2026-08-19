cmake_minimum_required(VERSION 3.16)

project(icloud-for-linux)

set(CMAKE_CXX_STANDARD 17)

include_directories(subprojects/choc/choc)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/app.cpp
)

add_executable(
    ${PROJECT_NAME}-tests
    tests/test_app.cpp
    src/app.cpp
)

target_include_directories(
    ${PROJECT_NAME}-tests
    PRIVATE
    src
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules (gtk3 REQUIRED gtk+-3.0 IMPORTED_TARGET)
    pkg_check_modules (webkit2 REQUIRED webkit2gtk-4.0 IMPORTED_TARGET)
    target_link_libraries (${PROJECT_NAME} PUBLIC pthread PkgConfig::gtk3 PkgConfig::webkit2)
else()
    target_link_libraries(${PROJECT_NAME} "-framework WebKit")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(PROGRAMS scripts/apple-maps scripts/apple-tv DESTINATION bin)

enable_testing()
add_test(NAME app COMMAND ${PROJECT_NAME}-tests)
