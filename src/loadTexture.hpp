#pragma once

#include <stdio.h>
#include <iostream>

#include "GL/glew.h"

namespace ost {
//
// TUTORIAL loadbmp here http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/ - 02.02.17
// TUTORIAL libpng here http://www.libpng.org/pub/png/libpng-1.2.5-manual.html#section-3 - 02.02.17
// 
// @return GLuint > 0 if successs
// @error return 0 if error
//
GLuint loadTexture(const char* imagepath) {

    // @ASSUMPTION - Since I only know that this game has just 1 texture.
    // I can just return the textureID, if this function is called twice.
    static GLuint textureID = 0;
    
    if (textureID > 0) 
        return textureID;
    // @ASSUMPTION


    // Actual RGB data
    unsigned char* data;
    unsigned int   width;
    unsigned int   height;
    { 
        // Data read from the header of the BMP file
        unsigned char header[54]; // Each BMP file begins by a 54-bytes header
        unsigned int  dataPos;     // Position in the file where the actual data begins

        unsigned int  imageSize;   // = width*height*3

        // Open the file
        FILE * file = fopen(imagepath,"rb");
        if (!file) {
            std::cerr << "Image file not found\n";
            return 0;
        }

        if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
            std::cerr << "Not a correct BMP file\n";
            return 0;
        }

        if ( header[0]!='B' || header[1]!='M' ) {
            std::cerr << "Not a correct BMP file\n";
            return 0;
        }

        // Read ints from the byte array
        dataPos    = *(int*)&(header[0x0A]);
        imageSize  = *(int*)&(header[0x22]);
        width      = *(int*)&(header[0x12]);
        height     = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize==0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos==0)      dataPos = 54; // The BMP header is done that way

        // Create a buffer
        data = new unsigned char[imageSize];

        // Read the actual data from the file into the buffer
        fread(data, 1, imageSize, file);

        //Everything is in memory now, the file can be closed
        fclose(file);
    }

    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 
                                 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return textureID;
}

}