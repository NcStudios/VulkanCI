cmake_minimum_required(VERSION 3.5)
project(VulkanCI-Test)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT DEFINED VKCI_API_VERSION)
    message(FATAL_ERROR "Must define VKCI_API_VERSION")
endif()

find_package(Vulkan REQUIRED)
add_executable(vulkan-ci-test test.cpp)
target_compile_definitions(vulkan-ci-test PRIVATE VKCI_API_VERSION="${VKCI_API_VERSION}")
target_link_libraries(vulkan-ci-test PRIVATE Vulkan::Vulkan)
