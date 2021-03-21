add_executable(
	infinite-damage-log-writer
	src/main.cpp
)

target_link_libraries(
	infinite-damage-log-writer
	PUBLIC ncloglib
)