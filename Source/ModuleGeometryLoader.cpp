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
#include "ModuleCamera.h"
#include "ModuleTexture.h"



using namespace std; 

class Model;
class Mesh;
class Primitive;



class Model {
public:
	void Load(const char* assetFileName);
	void Draw();
	void LoadMaterials(const tinygltf::Model& srcModel);
private: 
	std::vector<Mesh*>Meshes;
	std::vector<unsigned> textures;

};

class Mesh {
public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render(const std::vector<unsigned>& textures);
	void CreateVAO();
	void DestroyVBO(unsigned VBO);
	
	int numIndices;
	int vertexCount;
	unsigned vbo;
	unsigned vao;
	unsigned ebo;
	unsigned texCoordVBO;


	float4x4 model = float4x4::FromTRS(math::float3(0.0f, 0.0f, 0.0f), math::float3x3::identity, math::float3(20.0f, 20.0f, 10.0f));

};


Mesh mesh;
Model model;

ModuleGeometryLoader::ModuleGeometryLoader() {

}
ModuleGeometryLoader::~ModuleGeometryLoader() {
}

bool ModuleGeometryLoader::Init() {

	
	model.Load("./assets/BakerHouse.gltf");
	

	return true;
}
update_status ModuleGeometryLoader::PreUpdate() {

	return UPDATE_CONTINUE;
}
update_status ModuleGeometryLoader::Update() {

	model.Draw();

	return UPDATE_CONTINUE;
}
update_status ModuleGeometryLoader::PostUpdate() {

	return UPDATE_CONTINUE;
}

bool ModuleGeometryLoader::CleanUp() {
	
	return true;
}

void Model::Load(const char* assetFileName){


	//const char* filePath=App->CreateFilePath()

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
			mesh->LoadEBO(model, srcMesh, primitive);
			mesh->CreateVAO();
			Meshes.push_back(mesh);

		}
	}
	LoadMaterials(model);
	LOG("Textures:", textures.size())
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureId = App->GetTexture()->LoadTexture(image.uri.c_str());
			LOG("texture id created")
		}
		textures.push_back(textureId);
		LOG("texture id created", textureId)
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
		float* ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (ptr) {

			for (size_t i = 0; i < posAcc.count; ++i){
				for (size_t j = 0; j < 3; j++)
				{
					ptr[i*3+j] = *reinterpret_cast<const float*>(bufferPos);
					bufferPos += sizeof(float);
				}
			
			}
		}
		else {

			LOG("Error mapping buffer for writing");

		}

		
		glUnmapBuffer(GL_ARRAY_BUFFER);

		


	}

	const auto& itTexCoord = primitive.attributes.find("TEXCOORD_0");
	if (itTexCoord != primitive.attributes.end())
	{
		const tinygltf::Accessor& texCoordAcc = model.accessors[itTexCoord->second];
		SDL_assert(texCoordAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texCoordAcc.componentType == GL_FLOAT);

		const tinygltf::BufferView& texCoordView = model.bufferViews[texCoordAcc.bufferView];
		const tinygltf::Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
		const unsigned char* bufferTexCoord = &(texCoordBuffer.data[texCoordAcc.byteOffset + texCoordView.byteOffset]);

		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * texCoordAcc.count, bufferTexCoord, GL_STATIC_DRAW);


		vertexCount = texCoordAcc.count;

	}
}


void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{

	if (primitive.indices >= 0)
	{
		
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);
			numIndices = indAcc.count;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < numIndices; ++i) ptr[i] = bufferInd[i];
		}
		/* TODO indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT*/
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < numIndices; ++i) ptr[i] = bufferInd[i];
		}
		/* TODO indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE*/
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < numIndices; ++i) ptr[i] = bufferInd[i];
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

}

void Mesh::CreateVAO() {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 0));

	


	glBindVertexArray(0);

}

void Mesh::Render(const std::vector<unsigned>& textures)
{
	glUseProgram(App->GetProgram()->program);


	float4x4 view = App->GetCamera()->view;
	float4x4 proj = App->GetCamera()->proj;

	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(glGetUniformLocation(App->GetProgram()->program, "diffuse"), 0);
	/*glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(void*)(sizeof(float) * 3 * 3));*/

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG("OpenGL Error after glDrawArrays: %x", error);
	}
}

void Model::Draw() {
	for (int i = 0; i < Meshes.size(); i++)
	{
		Meshes[i]->Render(textures);
	}
}





