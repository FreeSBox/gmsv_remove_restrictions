include(FetchContent)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(SOURCE_SDK_TAG b2c2a520ed2f7c73d716682c9295294ffd26c172)
else()
	set(SOURCE_SDK_TAG 58938eaf9954fb5e8ee4a1780ea260405d25556b)
endif()

FetchContent_Declare(
	sourcesdk_minimal
	GIT_REPOSITORY https://github.com/danielga/sourcesdk-minimal.git
	GIT_TAG        ${SOURCE_SDK_TAG}
	GIT_PROGRESS TRUE
)
message("sourcesdk-minimal")
FetchContent_MakeAvailable(sourcesdk_minimal)

file(GLOB SOURCE_INCLUDES
	"${sourcesdk_minimal_SOURCE_DIR}/common"
	"${sourcesdk_minimal_SOURCE_DIR}/public"
	"${sourcesdk_minimal_SOURCE_DIR}/public/tier0"
	"${sourcesdk_minimal_SOURCE_DIR}/public/tier1"
	"${sourcesdk_minimal_SOURCE_DIR}/game/shared"
	"${sourcesdk_minimal_SOURCE_DIR}/game/server"
	"${sourcesdk_minimal_SOURCE_DIR}/engine"
)

add_library(common STATIC
	"${sourcesdk_minimal_SOURCE_DIR}/common/netmessages.cpp"
	"${sourcesdk_minimal_SOURCE_DIR}/game/server/gameinterface.cpp"
)


target_include_directories(common PRIVATE ${SOURCE_INCLUDES})
target_compile_definitions(common PRIVATE
	"RAD_TELEMETRY_DISABLED"
	"COMMON_STATIC_LIB"
	"GAME_DLL"
	"NO_MALLOC_OVERRIDE"
)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_library(tier1 STATIC
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/appinstance.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/bitbuf.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/newbitbuf.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/byteswap.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/characterset.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/checksum_crc.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/checksum_md5.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/checksum_sha1.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/circularbuffer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/commandbuffer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/convar.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/datamanager.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/diff.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/exprevaluator.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/generichash.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/interface.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/keyvalues.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/keyvaluesjson.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/kvpacker.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/lzmaDecoder.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/lzss.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/mempool.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/memstack.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/NetAdr.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/splitstring.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/rangecheckedvar.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/stringpool.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/strtools.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/strtools_unicode.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/tier1.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/tier1_logging.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/timeutils.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/uniqueid.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlbuffer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlbufferutil.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlsoacontainer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlstring.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlsymbol.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/miniprofiler_hash.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/sparsematrix.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/memoverride_dummy.cpp"
	)
else()
	add_library(tier1 STATIC
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/bitbuf.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/byteswap.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/characterset.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/checksum_crc.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/checksum_md5.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/checksum_sha1.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/commandbuffer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/convar.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/datamanager.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/diff.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/generichash.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/ilocalize.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/interface.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/keyvalues.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/kvpacker.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/lzmaDecoder.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/mempool.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/memstack.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/NetAdr.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/splitstring.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/rangecheckedvar.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/reliabletimer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/stringpool.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/strtools.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/strtools_unicode.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/tier1.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/tokenreader.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/sparsematrix.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/uniqueid.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlbuffer.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlbufferutil.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlstring.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlsymbol.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/utlbinaryblock.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/snappy.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/snappy-sinksource.cpp"
		"${sourcesdk_minimal_SOURCE_DIR}/tier1/snappy-stubs-internal.cpp"
	)
endif()

target_include_directories(tier1 PRIVATE ${SOURCE_INCLUDES})
target_compile_definitions(tier1 PRIVATE
	"RAD_TELEMETRY_DISABLED"
	"TIER1_STATIC_LIB"
)
if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	target_compile_definitions(tier1 PRIVATE "NO_MALLOC_OVERRIDE")
endif()

if (WIN32)

else()
	add_compile_definitions(
		"_DLL_EXT=.so"
		"COMPILER_GCC"
		"POSIX"
		"_POSIX"
		"LINUX"
		"_LINUX"
		"GNUC"
	)
endif()
