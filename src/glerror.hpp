#pragma once
#include "GL/glew.h"

#include "./logger.h"

namespace ost {

/*
 * @function handle_GLerror()
 * @brief  A loop which consumes a queue of errors sent by the graphics driver.
 *         All OpenGL error codes are described at the link below
 *  @doc https://www.khronos.org/opengl/wiki/OpenGL_Error#Testing_for_errors - 16.02.18
 */
inline void handle_GLerror()
{   
    GLenum err;

    while((err = glGetError()) != GL_NO_ERROR)
    {
        switch(err) 
        {   
            case GL_INVALID_ENUM:
            /* Given when an enumeration parameter is not a legal enumeration for that function. 
             * This is given only for local problems; if the spec allows the enumeration in certain
             * circumstances, where other parameters or state dictate those circumstances, 
             * then GL_INVALID_OPERATION is the result instead.
             */
                LOG_DEBUG("GL_INVALID_ENUM %zu", GL_INVALID_ENUM);
                break;


            case GL_INVALID_VALUE:
            /* Given when a value parameter is not a legal value for that function. 
             * This is only given for local problems; if the spec allows the value in certain circumstances, 
             * where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the 
             * result instead.
             */
                LOG_DEBUG("GL_INVALID_VALUE %zu", GL_INVALID_VALUE);
                break;


            case GL_INVALID_OPERATION:
            /* Given when the set of state for a command is not legal for the parameters given to that command. 
             * It is also given for commands where combinations of parameters define what the legal parameters are.
             */
                LOG_DEBUG("GL_INVALID_OPERATION %zu", GL_INVALID_OPERATION);
                break;


            case GL_STACK_OVERFLOW:
            /* Given when a stack pushing operation cannot be done because it would overflow the limit 
             * of that stack's size.
             */
                LOG_DEBUG("GL_STACK_OVERFLOW %zu", GL_STACK_OVERFLOW);
                break;


            case GL_STACK_UNDERFLOW:
            /* Given when a stack popping operation cannot be done because the stack is already at its lowest point.
             */ 
                LOG_DEBUG("GL_STACK_UNDERFLOW %zu", GL_STACK_UNDERFLOW);
                break;


            case GL_OUT_OF_MEMORY:
            /* Given when performing an operation that can allocate memory, and the memory cannot be allocated. 
             * The results of OpenGL functions that return this error are undefined; it is allowable for 
             * partial operations to happen.
             */
                LOG_DEBUG("GL_OUT_OF_MEMORY %zu", GL_OUT_OF_MEMORY);
                break;


            case GL_INVALID_FRAMEBUFFER_OPERATION:
            /* Given when doing anything that would attempt to read from or write/render to a framebuffer that 
             * is not complete.
             */
                LOG_DEBUG("GL_INVALID_FRAMEBUFFER_OPERATION %zu", GL_INVALID_FRAMEBUFFER_OPERATION);
                break;

            case GL_CONTEXT_LOST:
                /* (with OpenGL 4.5 or ARB_KHR_robustness)
                 * Given if the OpenGL context has been lost, due to a graphics card reset.
                 */

                LOG_DEBUG("GL_CONTEXT_LOST %zu", GL_CONTEXT_LOST);
                break;
        }
    }
}

/* @note This OpenGL message callback is an OpenGL version 4.3 feature and thus not used in this project,
 *       as Jonas MacOS highSierra only support openGL 4.1.
 *       The callback has to be configured somewhere in the program like this.
 *              >  glEnable              ( GL_DEBUG_OUTPUT );
 *              >  glDebugMessageCallback( (GLDEBUGPROC) ost::MessageCallback, 0 );
 */
inline void MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    LOG_DEBUG("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", 
                      (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                       type, 
                       severity, 
                       message);
}

}