#include "ModuleTexture.h"
#include "DirectXTex.h"
#include "SDL.h"
#include "GL\glew.h"


ModuleTexture::ModuleTexture(){

}


ModuleTexture::~ModuleTexture(){

}

bool ModuleTexture:: Init(){
	return true;

}
update_status ModuleTexture::PreUpdate(){
	return UPDATE_CONTINUE;

}
update_status ModuleTexture::Update(){
	return UPDATE_CONTINUE;

}
update_status ModuleTexture::PostUpdate(){
	return UPDATE_CONTINUE;

}
bool ModuleTexture::CleanUp(){
	return true;

}


unsigned LoadTexture(const char* filePath)
{
    DirectX::ScratchImage image;
    DirectX::LoadFromDDSFile(L"your_texture.dds", DirectX::DDS_FLAGS_NONE, nullptr, image);

    // Assuming you have a valid OpenGL texture ID
    unsigned textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Iterate through mip levels
    for (size_t i = 0; i < image.GetMetadata().mipLevels; ++i)
    {
        const DirectX::Image* mip = image.GetImage(i, 0, 0);

        // Determine OpenGL format and internal format based on the DirectX format
        GLenum format, internalFormat;
        switch (mip->format)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            format = GL_RGBA;
            internalFormat = GL_RGBA8;
            break;
            // Add more cases for other formats as needed

        default:
            // Handle unsupported formats
            break;
        }

        // Upload texture data to OpenGL
        glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, GL_UNSIGNED_BYTE, mip->pixels);
    }

    // Set minification and magnification filters, and generate mipmaps if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

}