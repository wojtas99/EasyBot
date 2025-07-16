cmake_minimum_required(VERSION 3.20)
project(EasyBotDll)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019/lib/cmake")

find_package(Qt5 REQUIRED COMPONENTS Widgets)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)


add_library(EasyBot SHARED EasyBot_dlmain.cpp
        General/main_window_tab.cpp
        General/main_window_tab.h
        Walker/walker_tab.cpp
        Walker/walker_tab.h
        Functions/memory_functions.cpp
        Functions/memory_functions.h
        General/select_client_tab.cpp
        General/select_client_tab.h
        Walker/walker_thread.cpp
        Walker/walker_thread.h
        Structs/medivia_struct.h
        Target/target_tab.cpp
        Target/target_tab.h
        Target/target_thread.cpp
        Target/target_thread.h
        PatternScans/pattern_scan.cpp
        PatternScans/pattern_scan.h
        safe_queue.h
)

set_target_properties(EasyBot PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(EasyBot PRIVATE
        Qt5::Widgets
        libMinHook.x64.lib
)

