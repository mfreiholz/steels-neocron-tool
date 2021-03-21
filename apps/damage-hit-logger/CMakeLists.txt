add_executable(
	damage-hit-logger
	src/main.cpp
)

target_link_libraries(
	damage-hit-logger
	PUBLIC ncloglib
)