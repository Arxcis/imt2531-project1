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
#include "Entity.h"
#include "./spritesheet.hpp"

namespace Mesh { struct Mesh; }
namespace Shader {

static size_t VBO_MAX_SIZE = 2048;
static size_t EBO_MAX_SIZE = 4096;

class Shader
{
public:
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

inline void   initBuffers_VBO(Shader& shader);
inline void   initBuffers_VBO_EBO_TEX(Shader& shader);
inline void   drawVBO(const Shader& shader);
inline void   drawVBO_EBO_TEX(const Shader& shader);

inline auto   newMesh(Shader& shader, const size_t vertexCount) -> Mesh::Mesh;
inline auto   newMesh(Shader& shader, const size_t vertexCount, const size_t elementCount) -> Mesh::Mesh;

inline void   setUniformFloat(const Shader& shader, const std::string uniname, const float univalue);
inline void   setUniformVec4(const Shader& shader,  const std::string uniname, const glm::vec4 univalue);
inline void   setUniformMat4(const Shader& shader, const std::string uniname, const glm::mat4 univalue);
}

namespace Mesh {

inline void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n);
inline void bindPoint(const Mesh& mesh, const glm::vec2 pos);

inline void bindText(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const std::vector<ost::Rect>& uv, std::string text, float margin, glm::vec4 color);
inline void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect& uv, const size_t n);
inline void updateTextColor(const Mesh& mesh, std::string text, glm::vec4 color);
inline void updateTextUV(const Mesh& mesh, std::string text,std::vector<ost::Rect>& uvs);

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

/*
 * @function newMesh @overload
 *  @brief Allocating only vertices, (not elements), in the VBO.
 *  @ return A Mesh with iterators pointing to the collection of vertices
 *           generated in the VBO.
 */
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


/*
 * @function newMesh @overload
 *  @brief Allocating requested vertices and elements, in the corresponding VBO and EBO buffers.
 *  @return Returns a Mesh struc, which holds pointers to the location of the allocated vertices
 *          and elements.
 */
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

namespace Mesh
{

inline void bindRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect uv, const size_t n)
{
    const auto VBOoffset = (n * 4);
    const auto EBOoffset = (n * 6);
    const auto VBOit = mesh.VBObegin + VBOoffset;
    const auto EBOit = mesh.EBObegin + EBOoffset;

    VBOit[0].position = pos;
    VBOit[1].position = pos + glm::vec2{ size.x, 0.0f};
    VBOit[2].position = pos + glm::vec2{ size.x, -size.y };
    VBOit[3].position = pos + glm::vec2{ 0.0f,   -size.y };

    VBOit[0].texCoord = uv.topleft;
    VBOit[1].texCoord = uv.topright;
    VBOit[2].texCoord = uv.botright;
    VBOit[3].texCoord = uv.botleft;

    EBOit[0] = mesh.VBOindex + VBOoffset + 0;
    EBOit[1] = mesh.VBOindex + VBOoffset + 1;
    EBOit[2] = mesh.VBOindex + VBOoffset + 2;
    EBOit[3] = mesh.VBOindex + VBOoffset + 2;
    EBOit[4] = mesh.VBOindex + VBOoffset + 3;
    EBOit[5] = mesh.VBOindex + VBOoffset + 0;
}


inline void bindPoint(const Mesh& mesh, const glm::vec2 pos, const glm::vec4 color)
{
    mesh.VBObegin[0].position = pos;
    mesh.VBObegin[0].color = color;
}

inline void bindText(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const std::vector<ost::Rect>& uv, std::string text, float margin, glm::vec4 color)
{
    const size_t letterVertexCount = 4;
    const size_t letterElementCount = 6;
 
    #ifdef OST_DEBUG
        if (letterVertexCount * text.size() >= mesh.VBOcount) LOG_ERROR("if (letterVertexCount * text.size() >= mesh.VBOcount), iterator out of bounds");
        if (letterElementCount * text.size() >= mesh.EBOcount) LOG_ERROR("if (letterElementCount * text.size() >= mesh.VBOcount), iterator out of bounds");
    #endif
        auto VBOit = mesh.VBObegin;
        auto EBOit = mesh.EBObegin;
        auto i = 0;
        for(const auto t: text) {
            
            auto letterOffsetPosition = pos + glm::vec2{size.x + margin, 0} * float(i);;
            auto uvRect    = uv[i];
            auto VBOoffset = i * letterVertexCount;  

            VBOit[0].position = letterOffsetPosition;
            VBOit[1].position = letterOffsetPosition + glm::vec2{ size.x, 0.0f};
            VBOit[2].position = letterOffsetPosition + glm::vec2{ size.x, -size.y };
            VBOit[3].position = letterOffsetPosition + glm::vec2{ 0.0f,   -size.y };

            VBOit[0].color = color;
            VBOit[1].color = color;
            VBOit[2].color = color;
            VBOit[3].color = color;
            
            VBOit[0].texCoord = uvRect.topleft;
            VBOit[1].texCoord = uvRect.topright;
            VBOit[2].texCoord = uvRect.botright;
            VBOit[3].texCoord = uvRect.botleft;

            EBOit[0] = mesh.VBOindex + VBOoffset + 0;
            EBOit[1] = mesh.VBOindex + VBOoffset + 1;
            EBOit[2] = mesh.VBOindex + VBOoffset + 2;
            EBOit[3] = mesh.VBOindex + VBOoffset + 2;
            EBOit[4] = mesh.VBOindex + VBOoffset + 3;
            EBOit[5] = mesh.VBOindex + VBOoffset + 0;
            
            VBOit += letterVertexCount;
            EBOit += letterElementCount;
            i     += 1;
        }
}


inline void updateTextColor(const Mesh& mesh, std::string text, glm::vec4 color)
{   
    const size_t letterVertexCount = 4;
 
    #ifdef OST_DEBUG
        if (letterVertexCount * text.size() >= mesh.VBOcount) LOG_ERROR("if (letterVertexCount * text.size() >= mesh.VBOcount), iterator out of bounds");
    #endif

    auto it = mesh.VBObegin;
    for (const auto t: text) {
        it[0].color = color;
        it[1].color = color;
        it[2].color = color;
        it[3].color = color;
        it += letterVertexCount;
    }
}

inline void updateTextUV(const Mesh& mesh, std::string text,std::vector<ost::Rect>& uvs)
{
    const size_t letterVertexCount = 4;
 
    #ifdef OST_DEBUG
        if (letterVertexCount * text.size() >= mesh.VBOcount) LOG_ERROR("if (letterVertexCount * text.size() >= mesh.VBOcount), iterator out of bounds");
    #endif

    auto it = mesh.VBObegin;
    for (const auto t: text) {
        it[0].texCoord = uvs[t].topleft;
        it[1].texCoord = uvs[t].topright;
        it[2].texCoord = uvs[t].botright;
        it[3].texCoord = uvs[t].botleft;
        it += letterVertexCount;
    }
}

inline void updateRect(const Mesh& mesh, const glm::vec2 pos, const glm::vec2 size, const ost::Rect& uv, const size_t n)
{
    #ifdef OST_DEBUG
        if (n * 4 >= mesh.VBOcount) LOG_ERROR("if (n * 4 >= mesh.VBOcount), iterator out of bounds");
    #endif

    auto it = mesh.VBObegin + (n * 4);

    it[0].position = pos;
    it[1].position = pos + glm::vec2{ size.x, 0.0f};
    it[2].position = pos + glm::vec2{ size.x, -size.y };
    it[3].position = pos + glm::vec2{ 0.0f, -size.y };

    it[0].texCoord = uv.topleft;
    it[1].texCoord = uv.topright;
    it[2].texCoord = uv.botright;
    it[3].texCoord = uv.botleft;
}

inline void updatePoint(const Mesh& mesh, const float alpha)
{
    mesh.VBObegin[0].color[3] = alpha;
}

} // END NAMESPACE MESH
