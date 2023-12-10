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
	update_status PostUpdate();
	bool CleanUp();
	SDL_Keycode GetPressedKey() const { return pressedKey; }
	const char* GetFilePath() const { return filepath; }
   

private:
	const Uint8 *keyboard = NULL;
	const char* filepath="";
	void HandleKeyPress(SDL_Keycode key);
	void HandleFileDrop(const char* filePath);
	void HandleKeyUnpressed();
	SDL_Keycode pressedKey;

};