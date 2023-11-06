#pragma once
#include "Module.h"
class ModuleRender :
    public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	unsigned CreateTriangleVBO();
	void RenderVBO(unsigned VBO, unsigned shaderProgram);
	void DestroyVBO(unsigned VBO);

public:

	unsigned VBO;

};

