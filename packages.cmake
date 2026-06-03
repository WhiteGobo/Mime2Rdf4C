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

FetchContent_Declare(
	CInterfaceOxTTL
	GIT_REPOSITORY https://github.com/WhiteGobo/CInterfaceOxTTL.git
	#GIT_TAG 7f424c76698a0df4b500d30a5917eaa2e038e65a
	FIND_PACKAGE_ARGS
)
FetchContent_MakeAvailable(CInterfaceOxTTL)
