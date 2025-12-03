#include "renderer.hpp"
#include "SDL_video.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imgui.h"
#include "pointers.hpp"


namespace gm
{
	renderer::renderer() :
		m_window(nullptr)
	{
		g_renderer = this;
	}

	renderer::~renderer()
	{
		g_renderer = nullptr;

		ImGui_ImplSDL2_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();

		ImGui::DestroyContext();
	}

	void renderer::init(SDL_Window* window)
	{
		m_window = window;

		m_context = SDL_GL_CreateContext(window);
		ImGuiContext* ctx = ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;     // Use in game cursor

		ImGui_ImplSDL2_InitForOpenGL(window, m_context);
		ImGui_ImplOpenGL3_Init();

		Msg("Render initialized yey.\n");
	}

	void renderer::swap_window(SDL_Window* window)
	{
		if (m_window == nullptr)
		{
			init(window);
		}

		SDL_GLContext og_context = SDL_GL_GetCurrentContext();
		SDL_GL_MakeCurrent(window, m_context);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_MakeCurrent(window, og_context);
	}

	void renderer::poll_event(SDL_Event* event)
	{
		if (m_window != nullptr)
		{
			ImGui_ImplSDL2_ProcessEvent(event);
		}
	}
}
