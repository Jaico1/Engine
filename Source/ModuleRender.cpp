#include "ModuleRender.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "ModuleOpenGL.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h" 
#include "ModuleCamera.h"

ModuleRender::ModuleRender() {

}
ModuleRender::~ModuleRender(){

}

bool ModuleRender::Init() { 


	//model = math::float4x4::FromTRS(math::float3(2.0f, 0.0f, 0.0f), math::float3x3::RotateZ(math::pi / 4.0f), math::float3(2.0f, 1.0f, 1.0f));
	view = App->GetCamera()->view;
	proj = App->GetCamera()->proj;


    VBO = CreateTriangleVBO();
	return true;


}
update_status ModuleRender::PreUpdate() {

	return UPDATE_CONTINUE;

}
update_status ModuleRender::Update() {

	//RenderVBO(VBO, App->GetProgram()->program);

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
//void ModuleRender::RenderVBO(unsigned VBO, unsigned shaderProgram){
//
//    glUseProgram(shaderProgram);
//	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
//	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
//	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	
//
//}



void ModuleRender::DestroyVBO(unsigned VBO){


		glDeleteBuffers(1, &VBO);
	

}