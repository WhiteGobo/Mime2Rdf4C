configure_file(Mime2Rdf4C.h.in "${PROJECT_BINARY_DIR}/Mime2Rdf4C.h")

add_library(Mime2Rdf4C
	Mime2Rdf4C.c
	Mime2Rdf4CSerializer.c
	"${PROJECT_BINARY_DIR}/Mime2Rdf4C.h"
)

target_link_libraries(Mime2Rdf4C PUBLIC
	CInterfaceOxJsonld
	NQuadsRDF
)

target_include_directories(Mime2Rdf4C PUBLIC
	"${PROJECT_BINARY_DIR}"
)
