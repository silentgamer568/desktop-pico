cmake_minimum_required(VERSION 3.21)
project(DesktopPico LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(DesktopPico WIN32
    src/main.cpp
    src/pico.h
    src/pico.cpp
    src/speechbubble.h
    src/speechbubble.cpp
    src/colourpresets.h
    src/colourpresets.cpp
    src/dialogue.h
    src/dialogue.cpp
    src/spriterecolour.h
    src/spriterecolour.cpp
    src/resourcepath.h
)

target_link_libraries(DesktopPico PRIVATE Qt6::Widgets)

# Ship data/assets next to the exe so resourcepath.h can find them
# at runtime without embedding a Qt resource file (keeps hot-editing
# colours.json / dialogue.json possible post-build, which matters
# for the "friend edits dialogue" workflow this project cares about).
add_custom_command(TARGET DesktopPico POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:DesktopPico>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:DesktopPico>/data
)

if (WIN32)
    target_sources(DesktopPico PRIVATE assets/pico.rc)
endif()
