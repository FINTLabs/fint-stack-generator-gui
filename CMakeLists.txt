cmake_minimum_required(VERSION 3.12)
project(fint_stack_generator_gui)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/fsjovatsen/Qt/5.11.2/clang_64/lib/cmake")

include_directories(.)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(yaml-cpp REQUIRED)

add_executable(fint_stack_generator_gui
        dialog.cpp
        dialog.h
        dialog.ui
        main.cpp StackConfig.cpp StackConfig.h)

target_link_libraries(fint_stack_generator_gui Qt5::Core)
target_link_libraries(fint_stack_generator_gui Qt5::Widgets)
target_link_libraries(fint_stack_generator_gui yaml-cpp)


