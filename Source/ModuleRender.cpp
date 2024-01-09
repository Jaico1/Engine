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
#include "ModuleTexture.h"

ModuleRender::ModuleRender() {

}
ModuleRender::~ModuleRender(){

}

bool ModuleRender::Init() { 


	//model = math::float4x4::FromTRS(math::float3(2.0f, 0.0f, 0.0f), math::float3x3::RotateZ(math::pi / 4.0f), math::float3(2.0f, 1.0f, 1.0f));
	view = App->GetCamera()->GetViewMatrix();
	proj = App->GetCamera()->GetProjectionMatrix();


   
    textureCubeMap = loadTexture("../assets/cubemap.dds");
    VBO = CreateTriangleVBO();
    CreateVAO();

	return true;


}
update_status ModuleRender::PreUpdate() {

	return UPDATE_CONTINUE;

}
update_status ModuleRender::Update() {

	RenderVBO(App->GetProgram()->programCubeMap);

	return UPDATE_CONTINUE;

}
update_status ModuleRender::PostUpdate() {

	return UPDATE_CONTINUE;

}
bool ModuleRender::CleanUp() {

	DestroyVBO(VBO);

	return true;
}

unsigned int ModuleRender::loadTexture(const char* filePath) {
    unsigned int textureid = 0;

    textureid = App->GetTexture()->LoadCubeMap(filePath);

    return textureid;
}


unsigned ModuleRender::CreateTriangleVBO(){

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	unsigned VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	return VBO;
}

void ModuleRender::CreateVAO() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	/*glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);*/

	
		/*glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 0));*/
	

	glBindVertexArray(0);


}

void ModuleRender::RenderVBO( unsigned shaderProgram){
    
    glDepthMask(GL_FALSE);
    glUseProgram(shaderProgram);
	//glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);


    glBindVertexArray(VAO);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);
   // glUniform1i(glGetUniformLocation(App->GetProgram()->programCubeMap, "skybox"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
	
	

}


void ModuleRender::DestroyVBO(unsigned VBO){


		glDeleteBuffers(1, &VBO);
	

}