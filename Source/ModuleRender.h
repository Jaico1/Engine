#pragma once
#include "Module.h"
#include "MathGeoLib.h" 
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
	float4x4 LookAt(float3 target, float3 eye, float3 up);

public:

	unsigned VBO;
	unsigned shaderProgram;
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 proj;

};
