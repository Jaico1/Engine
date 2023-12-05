#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "MathGeoLib.h" 
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "MathGeoLib.h" 
#include "ModuleInput.h"

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
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	//model = math::float4x4::FromTRS(math::float3(2.0f, 0.0f, 0.0f), math::float3x3::RotateZ(math::pi / 4.0f), math::float3(2.0f, 1.0f, 1.0f));
	//view = LookAt(math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 5.0f, 15.0f), math::float3::unitY);
	view = float4x4(frustum.ViewMatrix());
	proj = frustum.ProjectionMatrix();



	/*glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*proj.v);*/

	cameraPosition = float3(0.0f, 1.0f, -2.0f);
	cameraFront = float3(0.0f, 0.0f, -1.0f);
	cameraUp = float3(0.0f, 1.0f, 0.0f);
	
	
	cameraSpeed = 0.05f;
	cameraSensitivity = 0.01f;
	

	return true;
}
update_status ModuleCamera::PreUpdate() {

	HandleInput();
	//view = LookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	return UPDATE_CONTINUE;
}
update_status ModuleCamera::Update() {

	view = LookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);


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

void ModuleCamera::HandleInput() {
	SDL_Keycode pressedKey = App->GetInput()->GetPressedKey();

	

	// Camera movement
	if (pressedKey==SDL_SCANCODE_W) {
		cameraPosition -= cameraSpeed * cameraFront;
		LOG("W PRESSED")
	}
	if (pressedKey == SDL_SCANCODE_S) {
		cameraPosition += cameraSpeed * cameraFront;
	}
	if (pressedKey == SDL_SCANCODE_A) {
		cameraPosition -= cameraSpeed * cameraFront.Cross(cameraUp).Normalized();
	}
	if (pressedKey == SDL_SCANCODE_D) {
		cameraPosition += cameraSpeed * cameraFront.Cross(cameraUp).Normalized();
	}
	if (pressedKey == SDL_SCANCODE_Q) {
		cameraPosition += cameraSpeed * cameraUp;
	}
	if (pressedKey == SDL_SCANCODE_E) {
		cameraPosition -= cameraSpeed * cameraUp;
	}
	if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		if (!mouseDragging) {
			prevMouseX = mouseX;
			prevMouseY = mouseY;
			totalMouseDeltaX = 0;
			totalMouseDeltaY = 0;
			mouseDragging = true;
		}
		else {
			if (mouseX != prevMouseX || mouseY != prevMouseY) {
				totalMouseDeltaX += mouseX - prevMouseX;
				totalMouseDeltaY += mouseY - prevMouseY;

				LOG("Before - Yaw: %f, Pitch: %f", yaw, pitch);

				float sensitivity = 0.01f;
				yaw += totalMouseDeltaX * sensitivity;
				pitch -= totalMouseDeltaY * sensitivity;

				LOG("After - Yaw: %f, Pitch: %f", yaw, pitch);
				const float yawClamp = 360.0f; // Adjust as needed
				yaw = fmod(yaw, yawClamp);


				// Constrain pitch to avoid camera flipping
				if (pitch > 89.0f) {
					pitch = 89.0f;
				}
				if (pitch < -89.0f) {
					pitch = -89.0f;
				}

				// Update previous mouse position for the next frame


				float3 front;
				front.x = cosf(math::DegToRad(yaw)) * cosf(math::DegToRad(pitch));
				front.y = sinf(math::DegToRad(pitch));
				front.z = sinf(math::DegToRad(yaw)) * cosf(math::DegToRad(pitch));
				cameraFront = front.Normalized();

				cameraRight = cameraFront.Cross(float3::unitY).Normalized();
				cameraUp = cameraRight.Cross(cameraFront).Normalized();


				prevMouseX = mouseX;
				prevMouseY = mouseY;
			}
			
		}
	}
	else {
		// Reset dragging state when the left mouse button is not pressed
		mouseDragging = false;
	}
}

