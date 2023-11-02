#pragma once
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	bool WindowResized(unsigned width, unsigned height);
	void* returnContext();
	unsigned int VBO;
	unsigned int VAO;
	unsigned int vertexShader;
	unsigned int shaderProgram;
	unsigned int fragmentShader;


public:
	void* context;
};
