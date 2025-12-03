#include "hooking/hooking.hpp"
#include "renderer.hpp"

namespace gm
{
	int hooks::SDL_PollEvent(SDL_Event* event)
	{
		if (g_renderer)
		{
			g_renderer->poll_event(event);
		}

		return g_hooking->get_original<hooks::SDL_PollEvent>()(event);
	}

	void hooks::SDL_GL_SwapWindow(SDL_Window* window)
	{
		if (g_renderer)
		{
			g_renderer->swap_window(window);
		}

		g_hooking->get_original<hooks::SDL_GL_SwapWindow>()(window);
	}
}
