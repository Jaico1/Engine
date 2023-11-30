#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "MathGeoLib.h" 
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "MathGeoLib.h" 

ModuleCamera::ModuleCamera() {

}
ModuleCamera::~ModuleCamera() {
}

bool ModuleCamera::Init() {

	/*Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0.0f, 1.0f, -2.0f);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (App->GetWindow()->currentWidth / App->GetWindow()->currentHeight));

	model = math::float4x4::FromTRS(math::float3(2.0f, 0.0f, 0.0f), math::float3x3::RotateZ(math::pi / 4.0f), math::float3(2.0f, 1.0f, 1.0f));
	view = LookAt(math::float3(0.0f, 1.f, 4.0f), math::float3(0.0f, 1.0f, -15.0f), math::float3::unitY);
	proj = frustum.ProjectionMatrix();*/

	
	Frustum frustum;
	frustum.type = (FrustumType::PerspectiveFrustum);
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 200.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (App->GetWindow()->currentWidth / App->GetWindow()->currentHeight));
	frustum.pos = float3(0.0f, 1.0f, -2.0f);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	model = math::float4x4::FromTRS(math::float3(2.0f, 0.0f, 0.0f), math::float3x3::RotateZ(math::pi / 4.0f), math::float3(2.0f, 1.0f, 1.0f));
	view = LookAt(math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 10.0f, -30.0f), math::float3::unitY);
	proj = frustum.ProjectionMatrix();


	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*proj.v);

	

	return true;
}
update_status ModuleCamera::PreUpdate() {

	return UPDATE_CONTINUE;
}
update_status ModuleCamera::Update() {

	

	return UPDATE_CONTINUE;
}
update_status ModuleCamera::PostUpdate() {

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp() {

	return true;
}

float4x4 ModuleCamera::LookAt(float3 target, float3 eye, float3 up) {
	float4x4 matrix;

	math::float3 f = (target - eye).Normalized();
	math::float3 r = up.Cross(f).Normalized();
	math::float3 u = f.Cross(r);

	matrix[0][0] = r.x; matrix[0][1] = r.y; matrix[0][2] = r.z; matrix[0][3] = -r.Dot(eye);
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z; matrix[1][3] = -u.Dot(eye);
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z; matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;



	return matrix;
}
