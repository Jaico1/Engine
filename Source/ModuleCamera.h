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
	float3 cameraPosition;
	float3 cameraFront;
	float3 cameraUp;
	float3 cameraRight;
	void HandleInput();

	

	

	float yaw;
	float pitch;
	float cameraSpeed;
	float cameraSensitivity;
	bool mouseDragging=false;
	int prevMouseX;
	int prevMouseY;
	int totalMouseDeltaX;
	int totalMouseDeltaY;

	
	math::float4x4 view;
	math::float4x4 proj;
};

