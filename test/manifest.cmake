function(extract_info_from_jsonld_manifest manifest_path var_baseIri var_testarray_json var_testarray_length)

	set(toRdf_manifest ${manifest_path})
	file(READ "${toRdf_manifest}" TORDFMANIFEST_STRING)
	#cmakes json cant handle ';' so we just remove them.
	string(REPLACE ";" "" TORDFMANIFEST_STRING "${TORDFMANIFEST_STRING}")
	string(JSON ${var_baseIri} GET ${TORDFMANIFEST_STRING} "baseIri")
	string(JSON ${var_testarray_json}
		GET ${TORDFMANIFEST_STRING} sequence
	)
	string(JSON ${var_testarray_length}
		LENGTH ${testlist}
	)

	return(PROPAGATE
		${var_baseIri}
		${var_testarray_json}
		${var_testarray_length})
endfunction()



function(configure_roundtripTest basePath baseIri testdata_json testsuffix)
	set(extras "")
	string(JSON id GET ${testdata_json} "@id")
	string(JSON type GET ${testdata_json} "@type")
	string(JSON name GET ${testdata_json} "name")
	string(JSON purpose GET ${testdata_json} "purpose")
	string(JSON input GET ${testdata_json} "input")
	set(inputfile "${basePath}/${input}")
	string(CONCAT testuri ${baseIri} ${input})

	set(testname "${testsuffix}${id}")
	if (type MATCHES "DatasetTest")
		list(APPEND extras "--needs-context-awarness")
	endif()

	add_test(
		NAME "${testname}" COMMAND testdriver_roundtrip
		"--name" ${name}
		"--purpose" ${purpose}
		"--input" ${inputfile}
		"--base-uri" ${testuri}
		${extras}
	)
	set_property(TEST "${testname}" PROPERTY
		LABELS "RoundtripTest"
	)
endfunction()
