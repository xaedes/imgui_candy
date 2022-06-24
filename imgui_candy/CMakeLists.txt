cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME imgui_candy)
project( ${PROJECT_NAME} )


add_library(
    ${PROJECT_NAME} 
    INTERFACE
)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (NOT TARGET imgui AND NOT TARGET imgui::imgui)
    find_package(imgui REQUIRED)  # imgui::imgui
endif()

target_link_libraries(${PROJECT_NAME} INTERFACE imgui::imgui)
