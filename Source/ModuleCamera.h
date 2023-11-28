#pragma once
#include "Module.h"
#include "MathGeoLib.h" 
class ModuleCamera :
    public Module
{
public:

	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	float4x4 LookAt(float3 target, float3 eye, float3 up);

public: 
	math::float4x4 model;
	math::float4x4 view;
	math::float4x4 proj;
};

