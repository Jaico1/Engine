#include "ModuleRender.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "ModuleOpenGL.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"

ModuleRender::ModuleRender() {

}
ModuleRender::~ModuleRender(){

}

bool ModuleRender::Init() { 

	const char* vertexShaderSource = "#version 430 core\n"
		"layout (location = 0) in vec3 my_vertex_position;\n"
		"layout(location = 0) uniform mat4 model;\n"
		"layout(location = 1) uniform mat4 view;\n"
		"layout(location = 2) uniform mat4 proj;\n"
		"void main(){\n"
		"   gl_Position = proj*view*model*vec4(my_vertex_position, 1.0)\n"
		"}\0";

	const char* fragmentShaderSource = "#version 430 core\n"
		"out vec4 FragColor;\n"
		"\n"
		"void main(){\n"
		"	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (App->GetWindow()->currentWidth / App->GetWindow()->currentHeight));

	model = math::float4x4::FromTRS(math::float3(2.0f, 0.0f, 0.0f), math::float3x3::RotateZ(math::pi / 4.0f), math::float3(2.0f, 1.0f, 1.0f));
	view = LookAt(math::float3(0.0f, 1.f, 4.0f), math::float3(0.0f, 1.0f, -20.0f), math::float3::unitY);
	proj = frustum.ProjectionMatrix();
    VBO = CreateTriangleVBO();
	return true;


}
update_status ModuleRender::PreUpdate() {

	return UPDATE_CONTINUE;

}
update_status ModuleRender::Update() {

	RenderVBO(VBO, shaderProgram);

	return UPDATE_CONTINUE;

}
update_status ModuleRender::PostUpdate() {

	return UPDATE_CONTINUE;

}
bool ModuleRender::CleanUp() {

	DestroyVBO(VBO);
	return true;
}


unsigned ModuleRender::CreateTriangleVBO(){

	float vertices[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	return VBO;
}
void ModuleRender::RenderVBO(unsigned VBO, unsigned shaderProgram){

    glUseProgram(shaderProgram);
	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	

}

float4x4 ModuleRender::LookAt(float3 target, float3 eye, float3 up) {

	float4x4 matrix;

	math::float3 f(target - eye); f.Normalize();
	math::float3 r(f.Cross(up)); r.Normalize();
	math::float3 u(r.Cross(f));
	matrix[0][0] = r.x; matrix[0][1] = r.y; matrix[0][2] = r.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -r.Dot(eye); matrix[1][3] = -u.Dot(eye); matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

void ModuleRender::DestroyVBO(unsigned VBO){


		glDeleteBuffers(1, &VBO);
	

}