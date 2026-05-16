include(FindPackageHandleStandardArgs)

find_library(CInterfaceOxJsonld_LIBRARY
	NAMES CInterfaceOxJsonld
)
find_path(CInterfaceOxJsonld_INCLUDE_DIR CInterfaceOxJsonld.h
	PATH_SUFFIXES CInterfaceOxJsonld
)


find_package_handle_standard_args(CInterfaceOxJsonld
	REQUIRED_VARS
	CInterfaceOxJsonld_LIBRARY
	CInterfaceOxJsonld_INCLUDE_DIR
)

if(CInterfaceOxJsonld_LIBRARY AND CInterfaceOxJsonld_INCLUDE_DIR)
	mark_as_advanced(CInterfaceOxJsonld_LIBRARY)
	mark_as_advanced(CInterfaceOxJsonld_INCLUDE_DIR)

	add_library(CInterfaceOxJsonld SHARED IMPORTED)
	target_include_directories(CInterfaceOxJsonld INTERFACE
		${CInterfaceOxJsonld_INCLUDE_DIR}
		#$<BUILD_INTERFACE:${CInterfaceOxJsonld_INCLUDE_DIR}>
	)
	set_property(TARGET CInterfaceOxJsonld PROPERTY IMPORTED_LOCATION
		${CInterfaceOxJsonld_LIBRARY})
endif(CInterfaceOxJsonld_LIBRARY AND CInterfaceOxJsonld_INCLUDE_DIR)
