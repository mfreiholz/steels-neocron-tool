# Qt Begin
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Multimedia LinguistTools REQUIRED)
# Qt End

if(WIN32)
	set(GUI_TYPE WIN32)
endif()
if(APPLE)
	set(GUI_TYPE MACOSX_BUNDLE)
endif()

add_executable(
	damage-log-viewer

	${GUI_TYPE}

	src/qml.qrc
	src/main.cpp

	src/App.hpp
	src/CsvWriter.hpp

	src/characterlog/DamageTypeInfo.hpp
	src/characterlog/HitZoneInfo.hpp
	src/characterlog/DamageHitInfo.hpp
	src/characterlog/LogReader.hpp
	src/characterlog/DamageHitInfoListModel.hpp
	src/characterlog/DamageHitInfoTableModel.hpp
	src/characterlog/CharacterLogPageBackend.hpp

	src/damagelog/DamageLogEntryInfo.hpp
	src/damagelog/DamageLogPageBackend.hpp
	src/damagelog/DamageLogFileReader.hpp
	src/damagelog/DamageLogListModel.hpp
	src/damagelog/DamageMeterModel.hpp
)

target_link_libraries(
	damage-log-viewer
	PUBLIC ncloglib
)

target_compile_definitions(damage-log-viewer PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(damage-log-viewer PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Multimedia)