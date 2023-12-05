#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "SDL.h"
#include "GL\glew.h"
#include "ModuleOpenGL.h"

ModuleProgram::ModuleProgram() {

}

ModuleProgram::~ModuleProgram() {

}

bool ModuleProgram::Init() {

	const char* vertexShaderSource = "#version 430\n"
    "layout (location = 0) in vec3 my_vertex_position;\n"
    "layout(location = 0) uniform mat4 model;\n"
    "layout(location = 1) uniform mat4 view;\n"
    "layout(location = 2) uniform mat4 proj;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = proj * view * model * vec4(my_vertex_position, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 430 \n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER,vertexShaderSource);
	unsigned int fragmentShader= CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	program = CreateProgram(vertexShader, fragmentShader);

	GLint success;
	GLchar infoLog[512];

	// Check vertex shader compilation status
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Vertex shader compilation failed: %s\n", infoLog);
	}

	// Check fragment shader compilation status
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("Fragment shader compilation failed: %s\n", infoLog);
	}

	// Check program linking status
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("Shader program linking failed: %s\n", infoLog);
	}

	return true;

}

update_status ModuleProgram::PreUpdate() {
	return UPDATE_CONTINUE;
}
update_status ModuleProgram::Update() {
	return UPDATE_CONTINUE;
}
update_status ModuleProgram::PostUpdate() {


	return UPDATE_CONTINUE;
}
bool ModuleProgram::CleanUp() {
	return true;
}


unsigned ModuleProgram::CompileShader(unsigned type, const char* source){

	unsigned shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, &source, 0);
	glCompileShader(shader_id);
	int res = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetShaderInfoLog(shader_id, len, &written, info);
			LOG("Log Info: %s", info);
			free(info);
		}
	}
	return shader_id;


}
unsigned ModuleProgram::CreateProgram(unsigned vtx_shader, unsigned frg_shader){

	unsigned program_id = glCreateProgram();
	glAttachShader(program_id, vtx_shader);
	glAttachShader(program_id, frg_shader);
	glLinkProgram(program_id);
	int res;
	glGetProgramiv(program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		int len = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			int written = 0;
			char* info = (char*)malloc(len);
			glGetProgramInfoLog(program, len, &written, info);
			LOG("Program Log Info: %s", info);
			free(info);
		}
	}
	glDeleteShader(vtx_shader);
	glDeleteShader(frg_shader);
	return program_id;


}