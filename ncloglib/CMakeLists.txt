set(name "ncloglib")
add_library(${name} STATIC)
target_sources(
	${name}
	PUBLIC
	        DamageHit.hpp
		LineParser.hpp
		CharacterLogParser.hpp
		DamageLogParser.hpp
	PRIVATE
	        library.cpp
)
target_include_directories(ncloglib PUBLIC ../)
