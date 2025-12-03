cmake_minimum_required(VERSION 3.20)

set(IS_SERVERSIDE OFF)

project(remove_restrictions CXX)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

message("void* size is: ${CMAKE_SIZEOF_VOID_P}")

message("\nFetching modules")
include(cmake/sourcesdk_minimal.cmake)
include(cmake/minhook.cmake)

message("\nFetching custom modules")
include(cmake/imgui.cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

message(STATUS ${CMAKE_PROJECT_NAME})
file(GLOB_RECURSE SRC_MAIN
	"${SRC_DIR}/**.hpp"
	"${SRC_DIR}/**.h"
	"${SRC_DIR}/**.cpp"
	"${SRC_DIR}/**.cc"
	"${SRC_DIR}/**.cxx"
	"${SRC_DIR}/**.asm"
)
add_library(${CMAKE_PROJECT_NAME} MODULE "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
	"${SRC_DIR}"
	"${SRC_DIR}/gmod"
	"${sourcesdk_minimal_SOURCE_DIR}/public"
	"${sourcesdk_minimal_SOURCE_DIR}/common"
	"${sourcesdk_minimal_SOURCE_DIR}/game/shared"
	"${sourcesdk_minimal_SOURCE_DIR}/game/server"
	#"${sourcesdk_minimal_SOURCE_DIR}/game/client"
	${imgui_SOURCE_DIR}
)
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE "${SRC_DIR}/common.hpp")
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE minhook common SDL2 imgui)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	if (IS_SERVERSIDE)
		target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE minhook ${sourcesdk_minimal_SOURCE_DIR}/lib/public/linux64/libtier0.so)
	else()
		target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE minhook ${sourcesdk_minimal_SOURCE_DIR}/lib/public/linux64/libtier0_client.so)
	endif()
else()
	if (IS_SERVERSIDE)
		target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE minhook ${sourcesdk_minimal_SOURCE_DIR}/lib/public/linux32/libtier0_srv.so)
		else()
		target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE minhook ${sourcesdk_minimal_SOURCE_DIR}/lib/public/linux32/libtier0.so)
	endif()
endif()

if (IS_SERVERSIDE)
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "gmsv_")
else()
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "gmcl_")
endif()

if (WIN32)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX "_win64.dll")
	else()
		set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX "_win32.dll")
	endif()
else()
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX "_linux64.dll")
	else()
		set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX "_linux.dll")
	endif()
endif()

if (WIN32)
	add_compile_definitions(
		"_CRT_NONSTDC_NO_WARNINGS"
		"_CRT_SECURE_NO_WARNINGS"
		"STRICT"
	)
else()
	add_compile_definitions(
		"_GLIBCXX_USE_CXX11_ABI=0"
	)
endif()

