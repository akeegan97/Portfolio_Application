# Portfolio Application

The Portfolio Application is a tailor-made desktop tool designed for fund managers operating across Windows and MacOS platforms. It addresses the critical needs of calculating management and promote fees, as well as centralizing various fund management documents in a single, convenient location. At its core, the application leverages the robust GUI capabilities of wxWidgets for its interface, enriched with advanced charting functionalities provided by wxFreeChart. This combination not only enhances user experience but also facilitates insightful data visualization.

Beyond its sleek interface, the Portfolio Application offers comprehensive functionality to manage investors, positions, positional types, and assets. Users can easily add distributions for assets, alongside valuations, and the application proficiently calculates the Internal Rate of Return (IRR) for assets on a gross level. More so, it provides a nuanced investor view by automatically calculating IRR per asset invested, amalgamating any positions held in the same asset for a consolidated display. At the heart of its operations, the application employs JSON for data storage, ensuring seamless serialization and deserialization of data upon startup and closure, thus streamlining the management process and enhancing the overall efficiency of fund management.
## Features

The Portfolio Application encapsulates a robust set of features designed for efficiency and ease of use in fund management:

- **Cross-Platform Compatibility:** Seamlessly runs on both Windows and MacOS, ensuring accessibility for a wide range of users.
- **Advanced GUI:** Utilizes wxWidgets for a responsive and intuitive user interface, catering to the needs of modern fund management.
- **Chart Visualization:** Incorporates wxFreeChart for dynamic charting capabilities, offering visual insights into fund performance and metrics.
- **Comprehensive Fund Management:** Enables management of investors, positions, positional types, and assets all in one place.
- **Automatic Calculations:** Automatically calculates management and promote fees, as well as the Internal Rate of Return (IRR) for assets on both a gross level and per individual investor, streamlining the evaluation process.
- **JSON for Data Persistence:** Employs JSON for efficient data storage, facilitating quick serialization and deserialization for reliable data handling on startup and closure.

These features collective
## Prerequisites

Before you begin, ensure you have met the following requirements:

- Operating System: Windows 11 / MacOS Sonoma 14.4.1.
- A C++17 compatible compiler. This project uses GCC via Mingw on Windows for static builds.
- CMake version 3.10 or higher for building the project.
- wxWidgets 3.2, as wxFreeChart is not compatible with wxWidgets 3.3.
- wxFreeChart, configured for a static build alongside wxWidgets.

## Installing and Running

### Windows Specific

1. Install Mingw and ensure it is correctly configured in your system's PATH.
2. Verify GCC availability via `gcc --version`.
3. Clone and build wxWidgets 3.2 and wxFreeChart within `YourProjectDirectory/staticlibs`, configuring both for static builds using Mingw.
4. Adjust your `CMakeLists.txt` as necessary to link against these static libraries.
5. For your project, reference the `staticlibs/(wxWidgets),(wxFreeChart)` directories for includes and libraries in your build configuration.

### MacOS Specific

1. Within the project directory, create a `staticlibs` folder.
2. **Clone wxWidgets 3.2**: Navigate to `YourProjectDirectory/staticlibs` and clone the wxWidgets 3.2 repository. Configure and build wxWidgets for a static build, but do not install it globally.
3. **Clone and Build wxFreeChart**: Similarly, clone wxFreeChart into `YourProjectDirectory/staticlibs`. Ensure wxFreeChart is configured for a static build. During the build phase, provide the `wx-config` script from the wxWidgets build to wxFreeChart.
4. For your project, reference the `staticlibs/(wxWidgets),(wxFreeChart)` directories for includes and libraries in your build configuration.

### Linux Ubuntu Specific

## Building the Project

Directory Structure for Static Libraries
To facilitate the build process, it's recommended to structure your project with a staticlibs directory. This directory will contain all necessary static libraries and their include files, specifically for wxWidgets and wxFreeChart. Here's how your project directory might look:

project_directory/
|-- staticlibs/
    |-- wxWidgets/
    |-- wxFreeChart/
This structure is advised for both Windows and MacOS to keep your project organized and ensure that all required libraries are easily accessible.

#Building on MacOS
Prepare the Static Libraries:

Within your project's staticlibs directory, clone the wxWidgets 3.2 repository. It's crucial to use version 3.2 due to compatibility issues with wxFreeChart and newer versions of wxWidgets.
Configure wxWidgets for a static build. Do not install after building; the project will reference these libraries directly.
Similarly, clone and prepare wxFreeChart for static linking, ensuring it is set to use the wxWidgets static build you've just compiled.
Build Configuration:

#Building on Windows
Static Libraries Preparation:

Even though the directory structure and setup process for static libraries are similar on Windows, the specific path to your staticlibs might differ. Ensure you clone wxWidgets and wxFreeChart into this directory and configure both for static linking.
Statically Link System Libraries:

#General Build Instructions
After preparing your environment according to the platform-specific instructions outlined above:

Run CMake to configure your project, pointing it to the root of your project directory where your main CMakeLists.txt is located.
Build the project using your chosen build system, following the commands specific to that system.


## License

This project is licensed under the wxWindows Library Licence, Version 3.1 - see the [LICENSE.md](LICENSE.md) file for details.


## CMAKELISTS.TXT Win and MacOS

##Windows
```cmake
cmake_minimum_required(VERSION 3.10)
project(Portfolio VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

include(CTest)
enable_testing()

# Update include directories to match the static build
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE INCLUDE_SUBDIRS LIST_DIRECTORIES true ${PROJECT_SOURCE_DIR}/include/*)
foreach(SUBDIR ${INCLUDE_SUBDIRS})
    if(IS_DIRECTORY ${SUBDIR})
        include_directories(${SUBDIR})
    endif()
endforeach()
# Include directories for wxWidgets
include_directories("path/to/include/wxWidgets_3.2.3/lib/wx/include/msw-unicode-static-3.2")#needed for setup.h include file
include_directories("path/to/include/wxWidgets_3.2.3/include")
include_directories("path/to/include/wxFreeChart-master/include")

# Link directories for wxWidgets
link_directories("path/to/lib/wxWidgets_3.2.3/lib")

# Add the lib directory for wxFreeChart
link_directories("path/to/lib/wxFreeChart/wxFreeChart-master/lib")

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 
    "path/to/wxFreeChart-master/lib/libwxcode_mswu_freechart-3.2.a"
    wx_mswu_aui-3.2
    wx_mswu_core-3.2
    wx_baseu-3.2
    wxpng-3.2
    "C:/msys64/mingw64/lib/libz.a"
    wxjpeg-3.2
    version
    uxtheme
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comctl32
    advapi32
    windowscodecs
    shlwapi
    user32
    gdi32
    oleacc
    comdlg32
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
```

##MacOS
```cmake
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Portfolio LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
# Determine the full path to wx-config (make sure this is the static version)
set(WX_CONFIG_EXECUTABLE path/to/staticlibs/wxWidgets/wx-config)
 
find_library(IOKIT_LIBRARY IOKit)
message(STATUS "IOKit Library: ${IOKIT_LIBRARY}")
 
# Execute wx-config to get compiler and linker flags
execute_process(COMMAND ${WX_CONFIG_EXECUTABLE} --cxxflags
                RESULT_VARIABLE result
                OUTPUT_VARIABLE WX_CXX_FLAGS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "WX_CXX_FLAGS: ${WX_CXX_FLAGS}")
 
execute_process(COMMAND ${WX_CONFIG_EXECUTABLE} --libs
                RESULT_VARIABLE result
                OUTPUT_VARIABLE WX_LIBS
                OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "WX_LIBS: ${WX_LIBS}")
 
if(NOT result EQUAL "0")
    message(FATAL_ERROR "Failed to execute wx-config")
endif()

set(WXWIDGETS_ROOT "${PROJECT_SOURCE_DIR}/staticlibs/wxWidgets")
set(WXFREECHART_ROOT "${PROJECT_SOURCE_DIR}/staticlibs/wxFreeChart")
 
add_definitions(-DWXUSINGDLL)
 
# Add the build-specific include directory for wxWidgets where 'wx/setup.h' is located
# Make sure this path matches where your actual setup.h is located after building wxWidgets
set(WXWIDGETS_BUILD_INCLUDE_DIR "${WXWIDGETS_ROOT}/lib/wx/include/osx_cocoa-unicode-static-3.2")
 
# Include directories for wxWidgets and wxFreeChart
include_directories(
    "${WXWIDGETS_ROOT}/include"  # General wxWidgets include directory
    "${WXWIDGETS_BUILD_INCLUDE_DIR}"  # Build-specific wxWidgets include directory
    "${WXFREECHART_ROOT}/include"
    "${PROJECT_SOURCE_DIR}/include"
)
 
# Find source files
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE JSON_FILES "${PROJECT_SOURCE_DIR}/resources/date/*.json")
 
# Create macOS bundle
add_executable(Portfolio MACOSX_BUNDLE ${SRC_FILES} ${JSON_FILES})
 
# Set properties for the macOS bundle
set_target_properties(Portfolio PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist"
)
 
# Apply compile options obtained from wx-config
# Ensure WX_CXX_FLAGS is treated as a list before using it
separate_arguments(WX_CXX_FLAGS_LIST UNIX_COMMAND "${WX_CXX_FLAGS}")
target_compile_options(Portfolio PRIVATE ${WX_CXX_FLAGS_LIST})
 
# Link against wxWidgets and wxFreeChart libraries
# Ensure WX_LIBS is treated as a list before using it
separate_arguments(WX_LIBS_LIST UNIX_COMMAND "${WX_LIBS}")
 
target_link_libraries(Portfolio PRIVATE
    ${WX_LIBS}
    "/Users/andrewkeegan/Desktop/Code_Things/cpp/portfolio_application/staticlibs/wxWidgets/lib/libwx_osx_cocoau_aui-3.2.a"
    "${WXFREECHART_ROOT}/lib/libwxcode_osx_cocoau_freechart-3.2.a"
    ${IOKIT_LIBRARY} # Add this line to include the IOKit framework
)
add_custom_command(TARGET Portfolio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/data"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/icons"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources/data" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/data"
    COMMENT "Copying data resources to the app bundle"
)
add_custom_command(TARGET Portfolio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources/icons" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/icons"
    COMMENT "Copying icon resources to the app bundle"
)
add_custom_command(TARGET Portfolio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/resources/icons/NewAppIcon.icns" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/NewAppIcon.icns"
    COMMENT "Copying the app icon to the app bundle"
)
```
##Linux Ubuntu CMake...

