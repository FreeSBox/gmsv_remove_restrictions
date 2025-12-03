#include "module.hpp"

#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <elf.h>
#include <link.h>
#include <sys/mman.h>
#include <unistd.h>

namespace SymbolFinderPaste
{

#if defined SYSTEM_WINDOWS
	const std::vector<std::string> search_paths = {
#if defined ARCHITECTURE_X86_64
		"bin/win64/%s.dll",
#elif defined ARCHITECTURE_X86
		"garrysmod/bin/%s.dll", "bin/%s.dll"
#endif
	};

#elif defined SYSTEM_LINUX || defined SYSTEM_MACOSX
	const std::vector<std::string> search_paths = {
#if defined SYSTEM_LINUX
#if defined ARCHITECTURE_X86_64
		"bin/linux64/lib%s_client.so",
		"bin/linux64/%s_client.so",
		"bin/linux64/lib%s.so",
		"bin/linux64/%s.so",
#elif defined ARCHITECTURE_X86
		"bin/linux32/lib%s.so",
		"bin/linux32/%s.so",
		"garrysmod/bin/%s_srv.so",
		"garrysmod/bin/%s.so",
		"bin/lib%s_srv.so",
		"bin/%s_srv.so",
		"bin/lib%s.so",
		"bin/%s.so"
#endif
#elif defined SYSTEM_MACOSX
#if defined ARCHITECTURE_X86_64
		"GarrysMod_Signed.app/Contents/MacOS/lib%s.dylib",
		"GarrysMod_Signed.app/Contents/MacOS/%s.dylib"
#elif defined ARCHITECTURE_X86
		"garrysmod/bin/lib%s.dylib",
		"garrysmod/bin/%s.dylib",
		"bin/lib%s.dylib",
		"bin/%s.dylib"
#endif
#endif
	};
#endif

	#define PAGE_SIZE 4096
	#define PAGE_ALIGN_UP( x ) ( ( x + PAGE_SIZE - 1 ) & ~( PAGE_SIZE - 1 ) )

	struct DynLibInfo
	{
		void *baseAddress;
		size_t memorySize;
	};

	// The only useful part of SymbolFinder, and it's private... fuck.
	bool GetLibraryInfo( const void *handle, DynLibInfo &lib )
	{
		if( handle == nullptr )
			return false;

	#if defined ARCHITECTURE_X86
		
		typedef Elf32_Ehdr Elf_Ehdr;
		typedef Elf32_Phdr Elf_Phdr;
		const unsigned char ELFCLASS = ELFCLASS32;
		const uint16_t EM = EM_386;
		
	#elif defined ARCHITECTURE_X86_64
		
		typedef Elf64_Ehdr Elf_Ehdr;
		typedef Elf64_Phdr Elf_Phdr;
		const unsigned char ELFCLASS = ELFCLASS64;
		const uint16_t EM = EM_X86_64;
		
	#endif
		
		const struct link_map *map = static_cast<const struct link_map *>( handle );
		uintptr_t baseAddr = reinterpret_cast<uintptr_t>( map->l_addr );
		Elf_Ehdr *file = reinterpret_cast<Elf_Ehdr *>( baseAddr );
		if( memcmp( ELFMAG, file->e_ident, SELFMAG ) != 0 )
			return false;

		if( file->e_ident[EI_VERSION] != EV_CURRENT )
			return false;

		if( file->e_ident[EI_CLASS] != ELFCLASS || file->e_machine != EM || file->e_ident[EI_DATA] != ELFDATA2LSB )
			return false;

		if( file->e_type != ET_DYN )
			return false;

		uint16_t phdrCount = file->e_phnum;
		Elf_Phdr *phdr = reinterpret_cast<Elf_Phdr *>( baseAddr + file->e_phoff );
		for( uint16_t i = 0; i < phdrCount; ++i )
		{
			Elf_Phdr &hdr = phdr[i];
			if( hdr.p_type == PT_LOAD && hdr.p_flags == ( PF_X | PF_R ) )
			{
				lib.memorySize = PAGE_ALIGN_UP( hdr.p_filesz );
				break;
			}
		}

		lib.baseAddress = reinterpret_cast<void *>( baseAddr );
		return true;
	}
}

namespace memory
{
	module::module(const std::string_view name) :range(nullptr, 0), m_name(name), m_loaded(false)
	{
		try_get_module();
	}

	bool module::loaded() const
	{
		return m_loaded;
	}

	bool module::try_get_module()
	{
		if (m_loaded)
			return m_loaded;

		void *module_ = nullptr;
		for( const std::string &search_path : SymbolFinderPaste::search_paths )
		{
			char buffer[256] = { };
			if( snprintf( buffer, sizeof( buffer ), search_path.c_str( ), m_name.data() ) >= static_cast<int32_t>( sizeof( buffer ) ) )
			{
				// somehow truncated
				continue;
			}

			module_ = dlopen( buffer, RTLD_LAZY | RTLD_NOLOAD );
			if( module_ != nullptr )
			{
				break;
			}
		}
		if (module_ == nullptr)
		{
			return false;
		}
		SymbolFinderPaste::DynLibInfo lib;
		SymbolFinderPaste::GetLibraryInfo(module_, lib);

		m_base = lib.baseAddress;
		m_size = lib.memorySize;
		m_loaded = true;
		return m_loaded;
	}
}
