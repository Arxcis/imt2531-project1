#pragma once

#include <stdio.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" // grab from here --> https://github.com/nothings/stb/blob/master/stb_image.h


#include "GL/glew.h"

namespace ost {
//
// TUTORIAL loadbmp here http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/ - 02.02.17
// TUTORIAL libpng here http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-3 - 02.02.17
// 
// @return GLuint > 0 if successs
// @error return 0 if error
//
GLuint loadTexture(const char* filepath) {
    int width, height, channelCount;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(filepath, &width, &height, &channelCount, 0);

    if (!data)
        return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,    // Texture target is the bound texture on previous line
                 0,                // mipmap level
                 GL_RGBA,           // openGL internal format
                 width,            // texture width
                 height,           // texture height
                 0,                // should always be 0
                 GL_RGBA,           // source image format
                 GL_UNSIGNED_BYTE, // source image data type
                 data);            // source image buffer
    
  //  glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    return textureID;
}

}