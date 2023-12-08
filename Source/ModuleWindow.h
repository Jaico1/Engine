#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();
	
	void SetCurrentWindowSize();
	SDL_Window* GetSDLwindow() { return window; }
	int GetCurrentHeight() { return currentHeight; }
	int GetCurrentWidth() { return currentWidth; }

private:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	int currentHeight;
	int currentWidth;
};

#endif // __ModuleWindow_H__