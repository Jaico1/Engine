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
#include "ModuleInput.h"



using namespace std; 




class Model {
public:
	void Load(const char* assetFileName);
	void Draw(int modelIndex);
	void LoadMaterials(const tinygltf::Model& srcModel);
	const std::vector<Mesh*>& GetMeshes() const {
		return Meshes;
	}
	void Clear();
	
private: 
	
	std::vector<Mesh*>Meshes;
	std::vector<unsigned> textures;

};

class Mesh {
public:
	Mesh();
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive, std::vector<unsigned> textures);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void Render(const std::vector<unsigned>& textures, int materialIndex);
	void CreateVAO();
	void DestroyVBO(unsigned VBO);
	int triangleCount;
	int GetNumIndices(){ return numIndices; }
	void Clear();
	std::string meshName;
	bool hasTextures;

private:	
	int numIndices;
	int vertexCount;
	unsigned vbo;
	unsigned vao;
	unsigned ebo;
	unsigned texCoordVBO;


	
	float4x4 modelMatrix = float4x4::FromTRS(math::float3(0.0f, 0.f, 0.0f), math::float3x3::identity, math::float3(1.0f, 1.5f, 1.0f));

	
};

Mesh::Mesh() : triangleCount(0) {
	
}


Mesh mesh;
Model model;
Model model2;

ModuleGeometryLoader::ModuleGeometryLoader() {

}
ModuleGeometryLoader::~ModuleGeometryLoader() {
}

bool ModuleGeometryLoader::Init() {

	

	/*model.Load("../assets/BakerHouse.gltf");
	Models.push_back(model);*/


	return true;
}
update_status ModuleGeometryLoader::PreUpdate() {

	return UPDATE_CONTINUE;
}
update_status ModuleGeometryLoader::Update() {


	/*LoadGeometry();
	OnFileDrop();*/
	return UPDATE_CONTINUE;
}
update_status ModuleGeometryLoader::PostUpdate() {
	
	return UPDATE_CONTINUE;
}

bool ModuleGeometryLoader::CleanUp() {
	
	return true;
}

void ModuleGeometryLoader::OnFileDrop() {
	

	if (App->GetInput()->GetFilePath()!=""){
		for (auto& model : Models) {
			model.Clear();
		}
		model.Load(App->GetInput()->GetFilePath());
		Models.push_back(model);
	}
	
}

void ModuleGeometryLoader::SetFileDropCallback(FileDropCallback callback) {
	fileDropCallback = callback;
}

void Model::Load(const char* assetFileName){


	//const char* filePath=App->CreateFilePath()
	textures.clear();
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;

	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}

	LOG("Number of scenes: %d", model.scenes.size());
	LOG("Number of nodes: %d", model.nodes.size());
	LOG("Number of meshes: %d", model.meshes.size());
	LOG("Number of materials: %d", model.materials.size());

	for (size_t i = 0; i < model.scenes.size(); ++i) {
		LOG("Scene %d:", i);
		const tinygltf::Scene& scene = model.scenes[i];
		for (size_t j = 0; j < scene.nodes.size(); ++j) {
			LOG("  Node %d: %s", j, model.nodes[scene.nodes[j]].name.c_str());
		}
	}

	for (size_t i = 0; i < model.meshes.size(); ++i) {
		LOG("Mesh %d:", i);
		const tinygltf::Mesh& mesh = model.meshes[i];
		LOG("  Name: %s", mesh.name.c_str());
		LOG("  Number of primitives: %d", mesh.primitives.size());
		
	}

	LoadMaterials(model);
	
	for (const auto& srcMesh : model.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(model, srcMesh, primitive,textures);
			mesh->LoadEBO(model, srcMesh, primitive);
			mesh->CreateVAO();
			Meshes.push_back(mesh);

		}
	}
	
	
	
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{

	std::string baseDir = "..\\assets";

	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];

			std::string uri = image.uri;

			// Check if uri starts with "..\assets"
			if (uri.compare(0, baseDir.size(), baseDir) != 0)
			{
				// If not, concatenate the base directory
				uri = baseDir + "\\" + uri;
			}

			textureId = App->GetTexture()->LoadTexture(uri.c_str());
			LOG("texture id created")
				textures.push_back(textureId);
		}
		
		LOG("texture id created", textureId)
	}
}


void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive, std::vector<unsigned> textures)
{

	  meshName = mesh.name;
	
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

	hasTextures = !textures.empty();


	
	
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
	/*glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);*/

	if (hasTextures) {
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 0));
	}
	
	glBindVertexArray(0);

}

void Mesh::Render(const std::vector<unsigned>& textures, int materialIndex)
{
	if (textures.size()>0)
	{
		glUseProgram(App->GetProgram()->programTexture);
	}
	else
	{
		glUseProgram(App->GetProgram()->program);
	}
	


	float4x4 view = App->GetCamera()->GetViewMatrix();
	float4x4 proj = App->GetCamera()->GetProjectionMatrix();

	

	if (meshName == "Baker_house") {
		modelMatrix = float4x4::FromTRS(math::float3(0.0f, 0.0f, 0.0f), math::float3x3::identity, math::float3(20.0f, 30.0f, 20.0f));
		glUniformMatrix4fv(0, 1, GL_TRUE, modelMatrix.Transposed().ptr());
	}
	else if(meshName == "Chimney") {
		modelMatrix = float4x4::FromTRS(math::float3(0.0f, 0.0f, 0.0f), math::float3x3::identity, math::float3(20.0f, 30.0f, 20.0f));
		glUniformMatrix4fv(0, 1, GL_TRUE, modelMatrix.Transposed().ptr());
	}else if (meshName == "LOD3spShape") {
		modelMatrix = float4x4::FromTRS(math::float3(0.0f, 0.0f, 0.0f), math::float3x3::identity, math::float3(0.001f, 0.001f, 0.001f));
		glUniformMatrix4fv(0, 1, GL_TRUE, modelMatrix.Transposed().ptr());
	}
	else {
		glUniformMatrix4fv(0, 1, GL_TRUE, modelMatrix.Transposed().ptr());
	}
	

	

	
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
    glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);
	
	if (textures.size()>0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[materialIndex]);
		glUniform1i(glGetUniformLocation(App->GetProgram()->program, "diffuse"), 0);
	}
		
	
	
	/*glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,(void*)(sizeof(float) * 3 * 3));*/

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

	

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG("OpenGL Error after glDrawArrays: %x", error);
	}
}

void Model::Draw(int modelIndex) {
	for (int i = 0; i < Meshes.size(); i++)
	{
		if (textures.size()>0)
		{
			Meshes[i]->Render(textures, textures.size()-1); 
		}
		else
		{
			Meshes[i]->Render(textures, 0);
		}
		
	}
}

int ModuleGeometryLoader::GetTotalTriangleCount() {

	std::vector<Mesh*>Meshes;

	int totalTriangleCount = 0;
	for (size_t i = 0; i < Models.size(); i++)
	{
		Meshes=Models[i].GetMeshes();

		for (int j = 0; j < Meshes.size(); j++) {
					totalTriangleCount += Meshes[j]->GetNumIndices();
		}

	}
	return totalTriangleCount/3;
}

void ModuleGeometryLoader::LoadGeometry() {
	for (size_t i = 0; i < Models.size(); i++)
	{
		Models[i].Draw(i);
	}
}


void Model::Clear() {
	// Release textures
	/*for (unsigned texture : textures) {
		App->GetTexture()->UnloadTexture(texture);
	}*/
	textures.clear();
	LOG("Number of texture: %d", textures.size());
	// Release meshes
	for (Mesh* mesh : Meshes) {
		mesh->Clear();
		
	}
	Meshes.clear();
}

void Mesh::Clear() {
	// Release VBO, VAO, and EBO
	/*glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &texCoordVBO);
	glDeleteVertexArrays(1, &vao);*/

	// Reset data
	vbo = 0;
	ebo = 0;
	texCoordVBO = 0;
	vao = 0;
	numIndices = 0;
	vertexCount = 0;
	// Add any other cleanup code as needed
}

