#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GL/glew.h"

#include "./logger.h"
#include "./spritesheet.hpp"

namespace Mesh { struct Mesh; }
namespace Shader {
    
using  Element = int;    
struct Vertex
{
    glm::vec2 position{};
    glm::vec4 color{};
    glm::vec2 texCoord{};
};

struct Shader
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    
    // Texture maps
    GLuint diffuse;
    GLuint normal;
    GLuint specular;

    GLuint program;

    GLint positionAttribute;
    GLint colorAttribute;
    GLint texcoordAttribute;

    GLenum drawMode;
    GLenum updateMode;

    size_t maxVertex;
    size_t maxElement;
    std::vector<Vertex>  vertexBuffer;
    std::vector<Element> elementBuffer;
};

inline void   _bindVertexArrayAttributes(Shader& shader);
inline void   _reserveVBO(Shader& shader);
inline void   _reserveEBO(Shader& shader);
inline void   _makeMeshVBO(Shader& shader, const size_t vertexCount);
inline void   _makeMeshEBO(Shader& shader, const size_t elementCount);

inline Shader makeShader_VBO(const GLuint program, const GLenum updateMode, const GLenum drawMode, const size_t maxVertex);
inline Shader makeShader_VBO_EBO_TEX(const GLuint program, const GLuint diffuse, const GLenum updateMode, const GLenum drawMode, const size_t maxVertex, const size_t maxElement);
inline void   drawVBO(const Shader& shader);
inline void   drawVBO_EBO_TEX(const Shader& shader);

inline auto   getMesh(Shader& shader, const size_t vertexCount) -> Mesh::Mesh;
inline auto   getMesh(Shader& shader, const size_t vertexCount, const size_t elementCount) -> Mesh::Mesh;
inline void   setUniformFloat(const Shader& shader, const std::string uniname, const float univalue);
inline void   setUniformVec4(const Shader& shader,  const std::string uniname, const glm::vec4 univalue);
inline void   setUniformMat4(const Shader& shader, const std::string uniname, const glm::mat4 univalue);
}

namespace Mesh {
struct Mesh 
{
    std::vector<Shader::Vertex>*  VBO;
    std::vector<Shader::Element>* EBO;
    int VBOindex;
    int EBOindex;
};

inline void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n);
inline void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n);

}


namespace Shader {

inline void _bindVertexArrayAttributes(Shader& shader) 
{
    shader.positionAttribute = glGetAttribLocation(shader.program, "position");
    shader.colorAttribute = glGetAttribLocation(shader.program, "color");
    shader.texcoordAttribute = glGetAttribLocation(shader.program, "texcoord");

    LOG_DEBUG("shader.program: %d, shader.positionAttribute: %d, shader.colorAttribute: %d, shader.texcoordAttribute: %d", shader.program, shader.positionAttribute, shader.colorAttribute, shader.texcoordAttribute)

    glVertexAttribPointer(shader.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(shader.colorAttribute,    4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec2));
    glVertexAttribPointer(shader.texcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)+sizeof(glm::vec4)));

    glEnableVertexAttribArray(shader.positionAttribute);
    glEnableVertexAttribArray(shader.colorAttribute);
    glEnableVertexAttribArray(shader.texcoordAttribute);
}

inline void _reserveVBO(Shader& shader) 
{
    shader.vertexBuffer.reserve(shader.maxVertex);

    glGenBuffers(1, &(shader.vbo));    
    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferData(GL_ARRAY_BUFFER, shader.maxVertex * sizeof(Vertex), shader.vertexBuffer.data(), shader.updateMode);
}

inline void _reserveEBO(Shader& shader) 
{
    shader.elementBuffer.reserve(shader.maxElement);

    glGenBuffers(1, &(shader.ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shader.maxElement * sizeof(int), shader.elementBuffer.data(), shader.updateMode);
}

/*
 * @function makeShader_VBO
 * @param updateMode - GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @param drawMode -   GL_POINTS, GL_TRIANGLES
 * @return Shader::Shader
 */
inline Shader makeShader_VBO(const GLuint program, const GLenum updateMode, const GLenum drawMode, const size_t maxVertex)
{
    Shader shader  = Shader{};
    shader.program = program;
    shader.drawMode   = drawMode;
    shader.updateMode = updateMode;
    shader.maxVertex  = maxVertex;
    
    // "The ordering doesn’t matter as long as you bind the VBO before using glBufferData and glBindVertexArray before you call glVertexAttribPointer."
    //  @doc http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/ - 12.02.18

    glUseProgram(shader.program);
    glGenVertexArrays(1, &(shader.vao));
    _reserveVBO(shader);
    glBindVertexArray(shader.vao);
    _bindVertexArrayAttributes(shader);

    glBindVertexArray(0);
    glUseProgram(0);
    return shader;
}

/*
 * @function makeShader_VBO_EBO_TEX
 * @param updateMode - GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @param drawMode -   GL_POINTS, GL_TRIANGLES
 * @return Shader::Shader
 */
inline Shader makeShader_VBO_EBO_TEX(const GLuint program, const GLuint diffuse, const GLenum updateMode, const GLenum drawMode, const size_t maxVertex, const size_t maxElement)
{

    Shader shader  = Shader{};
    shader.program = program;
    shader.diffuse = diffuse;
    shader.drawMode   = drawMode;
    shader.updateMode = updateMode;
    shader.maxVertex  = maxVertex;
    shader.maxElement = maxElement;

    glUseProgram(shader.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader.diffuse);
    glUniform1i(   glGetUniformLocation(shader.program, "diffuse"),    0);    

    glGenVertexArrays(1, &(shader.vao));
    _reserveVBO(shader);
    _reserveEBO(shader);
    glBindVertexArray(shader.vao);
    _bindVertexArrayAttributes(shader);

    glBindTexture(GL_TEXTURE_2D, 0);    
    glBindVertexArray(0);
    glUseProgram(0);
    return shader;
}


//
// @function drawVBO
//
inline void drawVBO(const Shader& shader)
{
    glUseProgram(shader.program);
    glBindVertexArray(shader.vao);

    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, shader.vertexBuffer.size()*sizeof(Vertex), shader.vertexBuffer.data());

    glDrawArrays(shader.drawMode, 0, shader.vertexBuffer.size());

    glBindVertexArray(0);
    glUseProgram(0);
}

//
// @function drawVBO_EBO_TEX
//
inline void drawVBO_EBO_TEX(const Shader& shader)
{
    glUseProgram(shader.program);
    glBindVertexArray(shader.vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader.diffuse);

    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, shader.vertexBuffer.size()*sizeof(Vertex), shader.vertexBuffer.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.ebo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, shader.elementBuffer.size()*sizeof(int), shader.elementBuffer.data());
    
    glDrawElements(shader.drawMode, shader.elementBuffer.size(), GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

//
// @function _makeMeshVBO
//
inline void _makeMeshVBO(Shader& shader, const size_t vertexCount)
{
    if (vertexCount + shader.vertexBuffer.size() > shader.maxVertex ) {
        LOG_ERROR("PREVENTED VERTEX BUFFER OVERFLOW");
    }
    for (size_t i = 0; i < vertexCount; ++i) {
        shader.vertexBuffer.push_back(Vertex{});
    }
}

//
// @function _makeMeshEBO
//
inline void _makeMeshEBO(Shader& shader, const size_t elementCount)
{
    if (elementCount + shader.elementBuffer.size() > shader.maxElement ) {
        LOG_ERROR("PREVENTED ELEMENT BUFFER OVERFLOW");
    }
    for (size_t i = 0; i < elementCount; ++i) {
        shader.elementBuffer.push_back(int{});
    }
}

//
// @function getMesh @overload
//
inline auto getMesh(Shader& shader, const size_t vertexCount) -> Mesh::Mesh
{
    int _VBOindex = shader.vertexBuffer.size();  
    _makeMeshVBO(shader, vertexCount);

    return Mesh::Mesh {
        &(shader.vertexBuffer),
        nullptr, // EBO null
        _VBOindex,
        -1  // EBO null
    };
}

// 
// @function getMesh @overload
//
inline auto getMesh(Shader& shader, const size_t vertexCount, const size_t elementCount) -> Mesh::Mesh
{   
    int _VBOindex = shader.vertexBuffer.size();  
    int _EBOindex = shader.elementBuffer.size();
    _makeMeshVBO(shader, vertexCount);
    _makeMeshEBO(shader, elementCount);

    return Mesh::Mesh {
        &shader.vertexBuffer,
        &shader.elementBuffer,
        _VBOindex,
        _EBOindex
    };
}

//
// @function setUniformFloat
//
inline void setUniformFloat(const Shader& shader, const std::string uniname, const float univalue)
{
    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    if (uniform == -1) {
        LOG_ERROR("UNIFORM == -1");
    }
    glUniform1f(uniform, univalue);
    glUseProgram(0);
}

//
// @function setUniformVec4
//
inline void setUniformVec4(const Shader& shader, const std::string uniname, const glm::vec4 univalue)
{
    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    if (uniform == -1) {
        LOG_ERROR("UNIFORM == -1");
    }
    glUniform4fv(uniform, 1, glm::value_ptr(univalue));
    glUseProgram(0);
}

//
// @function setUniformMat4
//
inline void setUniformMat4(const Shader& shader, const std::string uniname, const glm::mat4 univalue) {

    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    if (uniform == -1) {
        LOG_ERROR("UNIFORM == -1");
    }
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(univalue));
    glUseProgram(0);
}

} // END NAMESPACE SHADER

namespace Mesh {
    
inline void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n) 
{
    auto offsetVBO = mesh.VBOindex + n * 4;
    auto offsetEBO = mesh.EBOindex + n * 6;

    (*mesh.VBO)[offsetVBO + 0].position = pos;
    (*mesh.VBO)[offsetVBO + 1].position = pos + glm::vec2{ size.x, 0.0f};
    (*mesh.VBO)[offsetVBO + 2].position = pos + glm::vec2{ size.x, -size.y };
    (*mesh.VBO)[offsetVBO + 3].position = pos + glm::vec2{ 0.0f,   -size.y };
    
    (*mesh.VBO)[offsetVBO + 0].texCoord = uv.topleft;
    (*mesh.VBO)[offsetVBO + 1].texCoord = uv.topright;
    (*mesh.VBO)[offsetVBO + 2].texCoord = uv.botright;    
    (*mesh.VBO)[offsetVBO + 3].texCoord = uv.botleft;

    (*mesh.EBO)[offsetEBO + 0] = offsetVBO+0;   
    (*mesh.EBO)[offsetEBO + 1] = offsetVBO+1;
    (*mesh.EBO)[offsetEBO + 2] = offsetVBO+2;
    (*mesh.EBO)[offsetEBO + 3] = offsetVBO+2;
    (*mesh.EBO)[offsetEBO + 4] = offsetVBO+3;
    (*mesh.EBO)[offsetEBO + 5] = offsetVBO+0;
}


inline void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n) 
{
    auto offsetVBO = mesh.VBOindex + n * 4;

    (*mesh.VBO)[offsetVBO + 0].position = pos;
    (*mesh.VBO)[offsetVBO + 1].position = pos + glm::vec2{ size.x, 0.0f};
    (*mesh.VBO)[offsetVBO + 2].position = pos + glm::vec2{ size.x, -size.y };
    (*mesh.VBO)[offsetVBO + 3].position = pos + glm::vec2{ 0.0f, -size.y };
  
    (*mesh.VBO)[offsetVBO + 0].texCoord = uv.topleft;
    (*mesh.VBO)[offsetVBO + 1].texCoord = uv.topright;
    (*mesh.VBO)[offsetVBO + 2].texCoord = uv.botright;    
    (*mesh.VBO)[offsetVBO + 3].texCoord = uv.botleft;
}

} // END NAMESPACE MESH