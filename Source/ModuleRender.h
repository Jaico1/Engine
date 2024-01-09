#pragma once
#include "Module.h"
#include "MathGeoLib.h" 
#include <vector>

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
	void CreateVAO();
	void RenderVBO( unsigned shaderProgram);
	void DestroyVBO(unsigned VBO);
	
	unsigned int loadTexture(const char* filePath);

	

private:

	unsigned VBO;
	unsigned VAO;
	unsigned shaderProgram;
	unsigned textureCubeMap;
	unsigned int textureVBO;
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 proj;

};

