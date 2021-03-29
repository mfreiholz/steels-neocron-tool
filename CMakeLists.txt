cmake_minimum_required(VERSION 3.14)
project(NeocronLogParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Manuel's environment
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.1/msvc2019_64")
set(QT_VERSION_MAJOR 5)

add_subdirectory(ncloglib)
add_subdirectory(apps/damage-hit-logger)
add_subdirectory(apps/infinite-damage-log-writer)
add_subdirectory(apps/damage-log-viewer)
#add_subdirectory(test)