cmake_minimum_required(VERSION 3.14)
project(SteelsNeocronTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Manuel's environment
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install")
set(CMAKE_PREFIX_PATH "D:/Development/C_Workspace/thirdparty/qt/6.8.0/msvc2022_64")
set(QT_DIR "D:/Development/C_Workspace/thirdparty/qt/6.8.0/msvc2022_64")
set(QT_VERSION_MAJOR 6)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Multimedia LinguistTools REQUIRED)

set(CMAKE_VS_DEBUGGER_ENVIRONMENT "PATH=$<TARGET_FILE_DIR:Qt6::Core>;%PATH%;")
message(var="$<TARGET_FILE_DIR:Qt6::Core>")

add_subdirectory(ncloglib)
add_subdirectory(apps/damage-hit-logger)
add_subdirectory(apps/infinite-damage-log-writer)
add_subdirectory(apps/damage-log-viewer)
add_subdirectory(test)
