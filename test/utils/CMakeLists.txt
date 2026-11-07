add_executable(Mime2Rdf4C_test_jsonldcontext
	test_jsonldcontext.c
)

target_link_libraries(Mime2Rdf4C_test_jsonldcontext
	Mime2Rdf4C
)

add_test(NAME Mime2Rdf4C::test_jsonldcontext
	COMMAND Mime2Rdf4C_test_jsonldcontext
	"${CMAKE_CURRENT_SOURCE_DIR}/data00.jsonld" "${CMAKE_CURRENT_SOURCE_DIR}"
)
