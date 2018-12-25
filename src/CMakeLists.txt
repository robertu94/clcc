add_executable(clcc
	commandline.cc
	compiler.cc
	)

target_link_libraries(
	clcc
	OpenCL::OpenCL
	)

target_compile_features(
	clcc
	PRIVATE cxx_std_17
	)

set_target_properties(
	clcc
	PROPERTIES
	CXX_EXTENSIONS OFF
	)

install(TARGETS clcc RUNTIME DESTINATION bin)
