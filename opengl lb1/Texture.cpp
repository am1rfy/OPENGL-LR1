#include "Texture.h"
#include <iostream>

#include <GL/glew.h>
#include <Magick++.h>

Texture::Texture(GLenum TextureTarget, const std::string& FileName) {
    textureTarget = TextureTarget;
    fileName = FileName;             
    image = NULL;
}

bool Texture::Load() {
    try {
        image = new Image(fileName);
        image->write(&blob, "RGBA");
    }
    catch (Error& Error) {
        std::cout << "Error loading texture '" << fileName << "': " << Error.what() << std::endl;
        return false;
    }

    glGenTextures(1, &textureObj);        
    glBindTexture(textureTarget, textureObj);
    glTexImage2D(textureTarget, 0, GL_RGB, image->columns(), image->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
    glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::Bind(GLenum TextureUnit) {
    glActiveTexture(TextureUnit);
    glBindTexture(textureTarget, textureObj);
}