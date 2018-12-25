cmake_minimum_required(VERSION 3.2)
project(clcc 
				VERSION 1.0
				DESCRIPTION "A small utility that calls OpenCL compilers"
	      LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

find_package(OpenCL REQUIRED)

add_subdirectory(src)
