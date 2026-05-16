include(CTest)
include(manifest.cmake)

FetchContent_Declare(
        BasicRDFGraphComparator
        GIT_REPOSITORY https://github.com/WhiteGobo/BasicRDFGraphComparator.git
        GIT_TAG 0a7b9393534410536d35375d2b8017c9c807f3ab
	EXCLUDE_FROM_ALL #dont install, its just for testing
        FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(BasicRDFGraphComparator)

FetchContent_Declare(
        cwalk
        GIT_REPOSITORY https://github.com/likle/cwalk.git
	GIT_TAG v1.2.9
	EXCLUDE_FROM_ALL #dont install, its just for testing
	FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(cwalk)

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
	configure_roundtripTest(${tmpbase} ${baseIri} ${data} "roundtrip-" )
endforeach()
