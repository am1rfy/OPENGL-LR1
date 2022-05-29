#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <Magick++.h>

using namespace Magick;

class Texture {
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string fileName;
    GLenum textureTarget;
    GLuint textureObj;
    Image* image;
    Blob blob;
};


#endif