file(GLOB_RECURSE SRC_FILES src/*)
file(GLOB_RECURSE INC_FILES include/*)

find_package(Vulkan)
if (NOT Vulkan_FOUND)
    message("vulkan sdk not found")
endif()

add_library(VulkanSdk INTERFACE IMPORTED GLOBAL)
target_link_libraries(VulkanSdk INTERFACE ${Vulkan_LIBRARIES})
target_include_directories(VulkanSdk INTERFACE ${Vulkan_INCLUDE_DIRS})

if (WIN32)
    file(GLOB_RECURSE PLATFORM_SRC platform/windows/*)
elseif(APPLE)
    file(GLOB_RECURSE PLATFORM_SRC platform/macos/*)
endif()

sky_add_library(TARGET Driver STATIC
    SOURCES
        ${SRC_FILES}
        ${INC_FILES}
        ${PLATFORM_SRC}
    PRIVATE_INC
        src
    PUBLIC_INC
        include
    LINK_LIBS
        Core
        VulkanSdk
        VMA
        ${PLATFORM_EXT_LIBS}
)