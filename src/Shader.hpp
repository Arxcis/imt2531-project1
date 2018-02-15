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


static size_t VBO_MAX_SIZE = 2048;
static size_t EBO_MAX_SIZE = 4096;

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

    std::vector<Vertex>  vertexBuffer;
    std::vector<Element> elementBuffer;

};

inline void   _bindVertexArrayAttributes(Shader& shader);

inline void   _initVBO(Shader& shader);
inline void   _initEBO(Shader& shader);
inline void   _newVertices(Shader& shader, const size_t vertexCount);
inline void   _newElements(Shader& shader, const size_t elementCount);

inline Shader makeShader_VBO(const GLuint program, const GLenum updateMode, const GLenum drawMode);
inline Shader makeShader_VBO_EBO_TEX(const GLuint program, const GLuint diffuse, const GLenum updateMode, const GLenum drawMode);

inline void initBuffers_VBO(Shader& shader);
inline void initBuffers_VBO_EBO_TEX(Shader& shader);
inline void   drawVBO(const Shader& shader);
inline void   drawVBO_EBO_TEX(const Shader& shader);

inline auto   newMesh(Shader& shader, const size_t vertexCount) -> Mesh::Mesh;
inline auto   newMesh(Shader& shader, const size_t vertexCount, const size_t elementCount) -> Mesh::Mesh;

inline void   setUniformFloat(const Shader& shader, const std::string uniname, const float univalue);
inline void   setUniformVec4(const Shader& shader,  const std::string uniname, const glm::vec4 univalue);
inline void   setUniformMat4(const Shader& shader, const std::string uniname, const glm::mat4 univalue);
}

namespace Mesh {

struct Mesh 
{
    std::vector<Shader::Vertex>::iterator  VBObegin;
    std::vector<Shader::Vertex>::iterator  VBOend;
    size_t VBOindex;
    size_t VBOcount;

    std::vector<Shader::Element>::iterator EBObegin;
    std::vector<Shader::Element>::iterator EBOend;
    size_t EBOindex;
    size_t EBOcount;
};

inline void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n);
inline void bindPoint(const Mesh& mesh, const glm::vec2 pos);
inline void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n);

}


namespace Shader {

inline void _bindVertexArrayAttributes(Shader& shader) 
{
    shader.positionAttribute = glGetAttribLocation(shader.program, "position");
    shader.colorAttribute    = glGetAttribLocation(shader.program, "color");
    shader.texcoordAttribute = glGetAttribLocation(shader.program, "texcoord");


    glVertexAttribPointer(shader.positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(shader.colorAttribute,    4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec2));
    glVertexAttribPointer(shader.texcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec2)+sizeof(glm::vec4)));

    glEnableVertexAttribArray(shader.positionAttribute);
    glEnableVertexAttribArray(shader.colorAttribute);
    glEnableVertexAttribArray(shader.texcoordAttribute);

    LOG_INFO("shader.program: %d, shader.positionAttribute: %d, shader.colorAttribute: %d, shader.texcoordAttribute: %d", shader.program, shader.positionAttribute, shader.colorAttribute, shader.texcoordAttribute)    
}

inline void _initVBO(Shader& shader) 
{
    glGenBuffers(1, &(shader.vbo));    
    glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);

    glBufferData(GL_ARRAY_BUFFER, shader.vertexBuffer.size() * sizeof(Vertex), shader.vertexBuffer.data(), shader.updateMode);

    LOG_INFO("vao: %d, ebo: %d, vbosize: %zu, updatemode: %d", shader.vao, shader.vbo, shader.vertexBuffer.size(), shader.updateMode);
}

inline void _initEBO(Shader& shader) 
{
    glGenBuffers(1, &(shader.ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader.ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shader.elementBuffer.size() * sizeof(int), shader.elementBuffer.data(), shader.updateMode);

    LOG_INFO("vao: %d, ebo: %d, vbosize: %zu, updatemode: %d", shader.vao, shader.ebo, shader.elementBuffer.size(), shader.updateMode);    
}

//
// @function _newVertices
//
inline void _newVertices(Shader& shader, const size_t vertexCount)
{
    for (size_t i = 0; i < vertexCount; ++i) {
        shader.vertexBuffer.push_back(Vertex{});
    }
}

//
// @function _newElements
//
inline void _newElements(Shader& shader, const size_t elementCount)
{
    for (size_t i = 0; i < elementCount; ++i) {
        shader.elementBuffer.push_back(Element{});
    }
}

/*
 * @function makeShader_VBO
 * @param updateMode - GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @param drawMode   -  GL_POINTS, GL_TRIANGLES
 * @return Shader::Shader
 */
inline Shader makeShader_VBO(const GLuint program, const GLenum updateMode, const GLenum drawMode)
{
    Shader shader     = Shader{};
    shader.program    = program;
    shader.drawMode   = drawMode;
    shader.updateMode = updateMode;

    shader.vertexBuffer.reserve(VBO_MAX_SIZE);

    return shader;
}

/*
 * @function makeShader_VBO_EBO_TEX
 * @param updateMode - GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @param drawMode   - GL_POINTS, GL_TRIANGLES
 * @return Shader::Shader
 */
inline Shader makeShader_VBO_EBO_TEX(const GLuint program, const GLuint diffuse, const GLenum updateMode, const GLenum drawMode)
{

    Shader shader     = Shader{};
    shader.program    = program;
    shader.diffuse    = diffuse;
    shader.drawMode   = drawMode;
    shader.updateMode = updateMode;

    shader.vertexBuffer.reserve(VBO_MAX_SIZE);
    shader.elementBuffer.reserve(EBO_MAX_SIZE);

    return shader;
}

/*
 * @function initBuffers_VBO
 *  @brief Call this function after all meshes have been allocated, but before you start drawing.
 */
inline void initBuffers_VBO(Shader& shader) 
{
    // "The ordering doesn’t matter as long as you bind the VBO before using glBufferData and glBindVertexArray before you call glVertexAttribPointer." - AND DO EVERYTHING BEFORE YOU do glVertexAttribPointer
    //  @doc http://headerphile.com/sdl2/opengl-part-2-vertexes-vbos-and-vaos/ - 12.02.18

    glUseProgram(shader.program);
    
    _initVBO(shader);
    
    glGenVertexArrays(1, &(shader.vao));
    glBindVertexArray(shader.vao);
    _bindVertexArrayAttributes(shader);

    LOG_INFO("vao: %d, vbo: %d, ebo: %d", shader.vao, shader.vbo, shader.ebo);

    glBindVertexArray(0);
    glUseProgram(0);
}

/*
 * @function initBuffers_VBO_EBO_TEX
 *  @brief Call this function after all meshes have been allocated, but before you start drawing.
 */
inline void initBuffers_VBO_EBO_TEX(Shader& shader) 
{
    glUseProgram(shader.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shader.diffuse);
    glUniform1i(   glGetUniformLocation(shader.program, "diffuse"),    0);    

    _initEBO(shader);
    _initVBO(shader);

    glGenVertexArrays(1, &(shader.vao));
    glBindVertexArray(shader.vao);
    _bindVertexArrayAttributes(shader);

    LOG_INFO("vao: %d, vbo: %d, ebo: %d", shader.vao, shader.vbo, shader.ebo);

    glBindTexture(GL_TEXTURE_2D, 0);    
    glBindVertexArray(0);
    glUseProgram(0);
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
// @function newMesh @overload
//  @brief Allocating only vertices, (not elements), in the VBO.
//  @ return A Mesh with iterators pointing to the collection of vertices 
//           generated in the VBO.
//
inline auto newMesh(Shader& shader, const size_t vertexCount) -> Mesh::Mesh
{

    size_t VBOindex = shader.vertexBuffer.size();
    auto   VBObegin = shader.vertexBuffer.end();

    _newVertices(shader, vertexCount);
    auto   VBOend  = shader.vertexBuffer.end();

    return Mesh::Mesh {
        VBObegin,
        VBOend,
        VBOindex,
        vertexCount,
        {},
        {},
        0,
        0
    };
}


// 
// @function newMesh @overload
//  @brief Allocating requested vertices and elements, in the corresponding VBO and EBO buffers.
//  @return Returns a Mesh struc, which holds pointers to the location of the allocated vertices
//          and elements.
//
inline auto newMesh(Shader& shader, const size_t vertexCount, const size_t elementCount) -> Mesh::Mesh
{   
    size_t VBOindex = shader.vertexBuffer.size(); 
    auto   VBObegin = shader.vertexBuffer.end();
    size_t EBOindex = shader.elementBuffer.size();
    auto   EBObegin = shader.elementBuffer.end();

    _newVertices(shader, vertexCount);
    _newElements(shader, elementCount);

    auto VBOend   = shader.vertexBuffer.end();
    auto EBOend   = shader.elementBuffer.end();

    return Mesh::Mesh {
        VBObegin,
        VBOend,
        VBOindex,
        vertexCount,
        EBObegin,
        EBOend,
        EBOindex,
        elementCount
    };
}


//
// @function setUniformFloat
//
inline void setUniformFloat(const Shader& shader, const std::string uniname, const float univalue)
{
    glUseProgram(shader.program);
    GLint uniform = glGetUniformLocation(shader.program, uniname.c_str());
    LOG_INFO("uniform: %d", uniform);
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
    LOG_INFO("uniform: %d", uniform);    
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
    LOG_INFO("uniform: %d", uniform);    
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
    auto offsetVBO = n * 4;
    auto offsetEBO = n * 6;


    mesh.VBObegin[offsetVBO + 0].position = pos;
    mesh.VBObegin[offsetVBO + 1].position = pos + glm::vec2{ size.x, 0.0f};
    mesh.VBObegin[offsetVBO + 2].position = pos + glm::vec2{ size.x, -size.y };
    mesh.VBObegin[offsetVBO + 3].position = pos + glm::vec2{ 0.0f,   -size.y };
    
    mesh.VBObegin[offsetVBO + 0].texCoord = uv.topleft;
    mesh.VBObegin[offsetVBO + 1].texCoord = uv.topright;
    mesh.VBObegin[offsetVBO + 2].texCoord = uv.botright;    
    mesh.VBObegin[offsetVBO + 3].texCoord = uv.botleft;

    mesh.EBObegin[offsetEBO + 0] = mesh.VBOindex + 0;   
    mesh.EBObegin[offsetEBO + 1] = mesh.VBOindex + 1;
    mesh.EBObegin[offsetEBO + 2] = mesh.VBOindex + 2;
    mesh.EBObegin[offsetEBO + 3] = mesh.VBOindex + 2;
    mesh.EBObegin[offsetEBO + 4] = mesh.VBOindex + 3;
    mesh.EBObegin[offsetEBO + 5] = mesh.VBOindex + 0;
}


inline void bindPoint(const Mesh& mesh, const glm::vec2 pos) 
{
    mesh.VBObegin[0].position = pos;
}


inline void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n) 
{
    auto offsetVBO = n * 4;

    mesh.VBObegin[offsetVBO + 0].position = pos;
    mesh.VBObegin[offsetVBO + 1].position = pos + glm::vec2{ size.x, 0.0f};
    mesh.VBObegin[offsetVBO + 2].position = pos + glm::vec2{ size.x, -size.y };
    mesh.VBObegin[offsetVBO + 3].position = pos + glm::vec2{ 0.0f, -size.y };
  
    mesh.VBObegin[offsetVBO + 0].texCoord = uv.topleft;
    mesh.VBObegin[offsetVBO + 1].texCoord = uv.topright;
    mesh.VBObegin[offsetVBO + 2].texCoord = uv.botright;    
    mesh.VBObegin[offsetVBO + 3].texCoord = uv.botleft;
}

} // END NAMESPACE MESH