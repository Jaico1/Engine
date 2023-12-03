#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();
	SDL_Keycode GetPressedKey() const { return pressedKey; }
    void HandleKeyPress(SDL_Keycode key);
	void HandleKeyUnpressed();

private:
	const Uint8 *keyboard = NULL;
	
	SDL_Keycode pressedKey;
};