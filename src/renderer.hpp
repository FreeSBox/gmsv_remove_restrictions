#pragma once

namespace gm
{
	class renderer
	{
	public:
		renderer();
		~renderer();

		void swap_window(SDL_Window* window);
		void poll_event(SDL_Event* event);
	
	private:
		void init(SDL_Window* window);

	private:
		SDL_GLContext m_context;
		SDL_Window* m_window;
	};

	inline renderer* g_renderer{};
}
