cmake_minimum_required(VERSION 3.25)

project(Mime2Rdf4C VERSION 0.1.0 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#find_package(CInterfaceOxJsonld REQUIRED)
include(FetchContent)
FetchContent_Declare(
	CInterfaceOxJsonld
	GIT_REPOSITORY https://github.com/WhiteGobo/CInterfaceOxJsonld.git
	#GIT_TAG 6ae08cf54ec74aa5f8a9d0ca8547155af53be03a
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(CInterfaceOxJsonld)

FetchContent_Declare(
	NQuadsRDF
	GIT_REPOSITORY https://github.com/WhiteGobo/NQuadsRDF.git
	#GIT_TAG 7f424c76698a0df4b500d30a5917eaa2e038e65a
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(NQuadsRDF)

add_subdirectory(src)

enable_testing()

add_subdirectory(test)
