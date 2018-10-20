cmake_minimum_required(VERSION 3.12)
project(fint_stack_generator_gui)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOUIC ON) # if you have any .ui files
set(CMAKE_AUTORCC ON) # if you have any .qrc files
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/Users/fsjovatsen/Qt/5.11.2/clang_64/lib/cmake")
if (APPLE)
    set(ICON fint.icns)
endif ()

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.app)


include_directories(.)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(yaml-cpp REQUIRED)


add_executable(fint_stack_generator_gui
        MACOSX_BUNDLE
        dialog.cpp
        dialog.h
        dialog.ui
        main.cpp
        StackConfig.cpp
        StackConfig.h
        StackFile.cpp
        StackFile.h
        main.qrc
        ${ICON}
        )

if (APPLE)
    set_source_files_properties(${ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

    set_target_properties(fint_stack_generator_gui PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "FINT Stack Generator")

    set_target_properties(fint_stack_generator_gui PROPERTIES
            RESOURCE fint.icns
            MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0.0
            MACOSX_BUNDLE_LONG_VERSION_STRING 1.0.0
            MACOSX_BUNDLE_ICON_FILE fint.icns
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/MacOSXBundleInfo.plist.in"
            )
endif ()


target_link_libraries(fint_stack_generator_gui Qt5::Core)
target_link_libraries(fint_stack_generator_gui Qt5::Widgets)
target_link_libraries(fint_stack_generator_gui yaml-cpp)


