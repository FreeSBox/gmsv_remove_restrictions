#include "GarrysMod/Lua/Interface.h"

#include "pointers.hpp"
#include "byte_patch_manager.hpp"
#include "hooking/hooking.hpp"
#include "renderer.hpp"

#include <memory>


std::unique_ptr<gm::pointers> pointers_instance;
std::unique_ptr<gm::byte_patch_manager> byte_patch_manager_instance;
std::unique_ptr<gm::hooking> hooking_instance;
std::unique_ptr<gm::renderer> renderer_instance;

GMOD_MODULE_OPEN()
{
	using namespace gm;

	pointers_instance = std::make_unique<pointers>();
	Msg("Pointers initialized.\n");

	byte_patch_manager_instance = std::make_unique<byte_patch_manager>();
	Msg("Byte Patch Manager initialized.\n");
	
	hooking_instance = std::make_unique<hooking>();
	Msg("Hooking initialized.\n");

	g_hooking->enable();
	Msg("Hooking enabled.\n");
	
	renderer_instance = std::make_unique<renderer>();
	Msg("Renderer initialized.\n");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	using namespace gm;

	renderer_instance.reset();
	Msg("Renderer uninitialized.\n");

	g_hooking->disable();
	Msg("Hooking disabled.\n");

	hooking_instance.reset();
	Msg("Hooking uninitialized.\n");

	byte_patch_manager_instance.reset();
	Msg("Byte Patch Manager uninitialized.\n");

	pointers_instance.reset();
	Msg("Pointers uninitialized.\n");

	return 0;
}
