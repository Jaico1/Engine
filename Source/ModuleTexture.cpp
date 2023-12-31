#include "ModuleTexture.h"
#include "DirectXTex.h"
#include "SDL.h"
#include "GL\glew.h"
#include <wchar.h>

using namespace std;

ModuleTexture::ModuleTexture() :textureHeight(0),textureWidth(0) {

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


unsigned ModuleTexture::LoadTexture(const char* filePath)
{
    const char* narrowFilePath = filePath;

    
    size_t bufferSize = strlen(narrowFilePath) + 1;
    wchar_t* wideFilePath = new wchar_t[bufferSize];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wideFilePath, bufferSize, narrowFilePath, _TRUNCATE);

    DirectX::ScratchImage image;
    DirectX::LoadFromWICFile(wideFilePath, DirectX::WIC_FLAGS_NONE, nullptr, image);
   
    textureWidth = image.GetMetadata().width;
    textureHeight = image.GetMetadata().height;
    
    unsigned textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    
    for (size_t i = 0; i < image.GetMetadata().mipLevels; ++i)
    {
        
        const DirectX::Image* mip = image.GetImage(i, 0, 0);
        const DirectX::TexMetadata metadata = image.GetMetadata();
        // Determine OpenGL format and internal format based on the DirectX format
        GLenum format, internalFormat;
        switch (metadata.format)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
           // type = GL_UNSIGNED_BYTE;
            
            break;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
           // type = GL_UNSIGNED_BYTE;
            
            break;
        case DXGI_FORMAT_B5G6R5_UNORM:
            internalFormat = GL_RGB8;
            format = GL_BGR;
           // type = GL_UNSIGNED_BYTE;
            
            break;
        default:
            assert(false && "Unsupported format");
        }
        

        LOG("WIDTH:  %f", mip->width)
        // Upload texture data to OpenGL
        glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, GL_UNSIGNED_BYTE, mip->pixels);
         
    }

    // Set minification and magnification filters, and generate mipmaps if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;

}
//
//unsigned ModuleTexture::LoadTexturePNG(const char* filePath)
//{
//    const char* narrowFilePath = filePath;
//
//    // Convert narrow string to wide string
//    size_t bufferSize = strlen(narrowFilePath) + 1;
//    wchar_t* wideFilePath = new wchar_t[bufferSize];
//    size_t convertedChars = 0;
//    mbstowcs_s(&convertedChars, wideFilePath, bufferSize, narrowFilePath, _TRUNCATE);
//
//    DirectX::ScratchImage image;
//    DirectX::LoadFromWICFile(wideFilePath, DirectX::WIC_FLAGS_NONE, nullptr, image);
//
//
//    // Assuming you have a valid OpenGL texture ID
//    unsigned textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//
//    // Iterate through mip levels
//    for (size_t i = 0; i < image.GetMetadata().mipLevels; ++i)
//    {
//        const DirectX::Image* mip = image.GetImage(i, 0, 0);
//        const DirectX::TexMetadata metadata = image.GetMetadata();
//        // Determine OpenGL format and internal format based on the DirectX format
//        GLenum format, internalFormat;
//        switch (metadata.format)
//        {
//        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
//        case DXGI_FORMAT_R8G8B8A8_UNORM:
//            internalFormat = GL_RGBA8;
//            format = GL_RGBA;
//            // type = GL_UNSIGNED_BYTE;
//            break;
//        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
//        case DXGI_FORMAT_B8G8R8A8_UNORM:
//            internalFormat = GL_RGBA8;
//            format = GL_BGRA;
//            // type = GL_UNSIGNED_BYTE;
//            break;
//        case DXGI_FORMAT_B5G6R5_UNORM:
//            internalFormat = GL_RGB8;
//            format = GL_BGR;
//            // type = GL_UNSIGNED_BYTE;
//            break;
//        default:
//            assert(false && "Unsupported format");
//        }
//
//
//        // Upload texture data to OpenGL
//        glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, GL_UNSIGNED_BYTE, mip->pixels);
//    }
//
//    // Set minification and magnification filters, and generate mipmaps if needed
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    // Unbind the texture
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//    return textureID;
//
//}