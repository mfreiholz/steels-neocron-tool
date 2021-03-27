# Qt Begin
set(CMAKE_PREFIX_PATH "D:/Qt/5.15.1/msvc2019_64")
#LIST(APPEND CMAKE_PROGRAM_PATH  "D:/Qt/5.15.1/msvc2019_64/bin")
set(QT_VERSION_MAJOR 5)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick Multimedia LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Multimedia LinguistTools REQUIRED)
# Qt End

add_executable(
	damage-log-viewer
	src/qml.qrc
	src/main.cpp

	src/App.hpp
	src/DamageTypeInfo.hpp
	src/DamageHitInfo.hpp
	src/LogReader.hpp

	src/DamageHitInfoListModel.hpp
	src/DamageHitInfoTableModel.hpp
)

target_link_libraries(
	damage-log-viewer
	PUBLIC ncloglib
)

target_compile_definitions(damage-log-viewer PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(damage-log-viewer PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Multimedia)