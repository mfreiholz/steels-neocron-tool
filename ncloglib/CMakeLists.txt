add_library(ncloglib INTERFACE)
target_sources(
	ncloglib
	INTERFACE
	ncloglib/DamageHit.hpp
	ncloglib/LineParser.hpp
	ncloglib/CharacterLogParser.hpp
)
target_include_directories(ncloglib INTERFACE .)