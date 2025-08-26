cmake_minimum_required(VERSION 3.20)
project(EasyBotDll)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019/lib/cmake")

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Qml)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)


add_library(EasyBot SHARED EasyBot_dlmain.cpp
        General/main_window_tab.cpp
        General/main_window_tab.h
        Walker/walker_tab.cpp
        Walker/walker_tab.h
        Loot/loot_tab.cpp
        Loot/loot_tab.h
        Loot/loot_thread.cpp
        Loot/loot_thread.h
        Functions/Game.cpp
        Functions/Game.h
        General/select_client_tab.cpp
        General/select_client_tab.h
        Walker/walker_thread.cpp
        Walker/walker_thread.h
        Structs/medivia_struct.h
        Target/target_tab.cpp
        Target/target_tab.h
        Target/target_thread.cpp
        Target/target_thread.h
        Status/status_tab.cpp
        Status/status_tab.h
        PatternScans/pattern_scan.cpp
        PatternScans/pattern_scan.h
        safe_queue.h
        Heal/heal_tab.cpp
        Heal/heal_tab.h
        Heal/heal_thread.cpp
        Heal/heal_thread.h
        BridgeAPI/EasyApi.cpp
        BridgeAPI/EasyApi.h
        BridgeAPI/ScriptRuntime.cpp
        BridgeAPI/ScriptRuntime.h
)

target_link_libraries(EasyBot PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Qml
        libMinHook.x64.lib
)

add_executable(EasyBotLoader WIN32 Injector/EasyBotLoader.cpp)

set_target_properties(EasyBot PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bot"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bot"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bot"
)
set_target_properties(EasyBotLoader PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bot"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bot"
)

target_link_libraries(EasyBotLoader PRIVATE
        Qt5::Widgets
)
