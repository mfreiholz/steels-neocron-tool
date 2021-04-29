execute_process(
	COMMAND windeployqt.exe --release ${BINARY_PATH}
	WORKING_DIRECTORY ${QT_DIR}/bin
)