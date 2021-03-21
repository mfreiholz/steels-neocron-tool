cmake_minimum_required(VERSION 3.14)
project(NeocronLogParser)

add_subdirectory(ncloglib)
add_subdirectory(apps/damage-hit-logger)
add_subdirectory(apps/infinite-damage-log-writer)
add_subdirectory(test)