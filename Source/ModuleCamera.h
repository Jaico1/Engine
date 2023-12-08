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
	float4x4 GetViewMatrix() { return view; }
	float4x4 GetProjectionMatrix() { return proj; }
	

private:

	float yaw=-91.0f;
	float pitch=0.0f;
	float prevYaw;
	float prevPitch;
	float cameraSpeed;
	float cameraSensitivity;
	bool mouseDragging=false;
	int prevMouseX;
	int prevMouseY;
	int totalMouseDeltaX;
	int totalMouseDeltaY;

	
	math::float4x4 view ;
	math::float4x4 proj ;
};

