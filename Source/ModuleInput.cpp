#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "SDL/include/SDL.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;


}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;
    //LOG("filepath: %d", filepath);
    while (SDL_PollEvent(&sdlEvent) != 0)
    {

        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetOpenGL()->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
            case SDL_KEYDOWN:
                HandleKeyPress(sdlEvent.button.button);
                break;
            case SDL_KEYUP:
                HandleKeyUnpressed();
                break;
            case SDL_DROPFILE:
                HandleFileDrop(sdlEvent.drop.file);
                SDL_free(sdlEvent.drop.file); 
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    return UPDATE_CONTINUE;
}

update_status ModuleInput::PostUpdate() {

    filepath = "";

    return UPDATE_CONTINUE;
}
// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::HandleKeyPress(SDL_Keycode key)
{
   
    pressedKey = key;
  
}

void ModuleInput::HandleKeyUnpressed()
{

    pressedKey = SDLK_UNKNOWN;

}

void ModuleInput::HandleFileDrop(const char* dropFilePath)
{
    
    filepath= dropFilePath;
}
