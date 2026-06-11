include(CTest)
include(manifest.cmake)

add_executable(testdriver_emptyserializer emptyserializer.c)
target_link_libraries(testdriver_emptyserializer Mime2Rdf4C)
add_test(NAME "Mime2Rdf4C::emptyserializer"
	COMMAND testdriver_emptyserializer
)

add_executable(testdriver_roundtrip
	roundtriptest.c
)

target_link_libraries(testdriver_roundtrip
	Mime2Rdf4C
	BasicRDFGraphComparator
	cwalk
)

set(tmpbase "${CMAKE_CURRENT_SOURCE_DIR}/owntestsuite")
set(roundtrip_manifest "${tmpbase}/manifest.json")
extract_info_from_jsonld_manifest(
	${roundtrip_manifest}
	baseIri testlist testlist_length)
math(EXPR range "${testlist_length} - 1")
foreach(x RANGE 0 ${range})
	string(JSON data GET ${testlist} ${x})
	configure_roundtripTest(${tmpbase} ${baseIri} ${data} "Mime2Rdf4C::roundtrip::" )
endforeach()
