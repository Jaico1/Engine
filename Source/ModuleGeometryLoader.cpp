#include "ModuleGeometryLoader.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"
#include "SDL.h"
#include "GL\glew.h"
#include "MathGeoLib.h" 
#include "ModuleProgram.h"
#include "Application.h"


using namespace std; 

class Model;
class Mesh;
class Primitive;


class Model {
public:
	void Load(const char* assetFileName);


};

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render();
	void DestroyVBO(unsigned VBO);

	unsigned vbo;
	unsigned ebo;


};


Mesh mesh;

ModuleGeometryLoader::ModuleGeometryLoader() {

}
ModuleGeometryLoader::~ModuleGeometryLoader() {
}

bool ModuleGeometryLoader::Init() {

	Model model = Model();
	Mesh mesh = Mesh();
	model.Load("../Source/TinyGlft/Triangle.gltf");



	return true;
}
update_status ModuleGeometryLoader::PreUpdate() {

	return UPDATE_CONTINUE;
}
update_status ModuleGeometryLoader::Update() {

	//mesh.Render();

	return UPDATE_CONTINUE;
}
update_status ModuleGeometryLoader::PostUpdate() {

	return UPDATE_CONTINUE;
}

bool ModuleGeometryLoader::CleanUp() {
	
	return true;
}

void Model::Load(const char* assetFileName){


	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}

	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(model, srcMesh, primitive);
		}
	}

}

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const  tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const  tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, bufferPos, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (ptr) {
			for (size_t i = 0; i < posAcc.count; ++i){
			
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posView.byteStride;

			}
		}
		else {

			LOG("Error mapping buffer for writing");

		}

		
		glUnmapBuffer(GL_ARRAY_BUFFER);


	}
}


void Mesh::Render()
{
	glUseProgram(App->GetProgram()->program);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLES, 0, 3);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG("OpenGL Error after glDrawArrays: %x", error);
	}
}




