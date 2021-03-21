add_executable(
	ncloglib_dev_test
	test_dev.cpp
)

target_link_libraries(
	ncloglib_dev_test
	PUBLIC ncloglib
)