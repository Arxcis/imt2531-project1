#include <stdio.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <stdlib.h>  // EXIT_FAILURE
#include <vector>
#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ost {
const int LEVEL_COUNT  = 1;
const int OPENGL_MAJOR = 4;
const int OPENGL_MINOR = 1;

const char WIN_NAME[] = "Overkill Studio - Assignment1";
const int WIN_WIDTH   = 1024;
const int WIN_HEIGHT  = 768;
}

#define PANIC(msg) {\
            fprintf(stderr,\
                "----- PANIC -----| file: %s | line: %d | msg: %s\n", __FILE__, __LINE__,  msg);\
            getchar();\
            exit(EXIT_FAILURE);}\


GLuint buildShaderProgram(const char* path_vert_shader, const char* path_frag_shader);
GLuint loadAndCompileShader(const char* fname, GLenum shaderType);

int main(int argc, char* argv[]) {

    int _selectedLevel = 0;

    // PROCESS ARGUMENTS
    if (argc > 2) {
        PANIC("Too many arguments from command-line");
    }
    if (argc == 2) {
        _selectedLevel = atoi(argv[1]);
    }
    if (_selectedLevel < 0 || _selectedLevel >= ost::LEVEL_COUNT) {
        PANIC("Level not available");
    }
    const int selectedLevel = _selectedLevel;
    printf("Starting level %d\n", selectedLevel);

    // INITIALIZE G L F W
    if (!glfwInit()){
        glfwTerminate();
        PANIC("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ost::OPENGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ost::OPENGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // OPEN G L F W WINDOW AND HOOK UP THE OPEN GL CONTEXT
    GLFWwindow* window = glfwCreateWindow(ost::WIN_WIDTH, ost::WIN_HEIGHT, ost::WIN_NAME, NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        glfwTerminate();
        PANIC("Failed to open GLFW window");
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);
    
    // INITIALIZE G L E W
    glewExperimental = GL_TRUE;  // MACOS/intel cpu support
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        PANIC("Failed to initialize GLEW");
    }
    
    // LOAD ASSETS (map, spritesheet)

    // BUILD ASSET-STRUCTS

    // BUILD SHADER PROGRAMS
    const GLuint shaderProgram    = buildShaderProgram("./src/vertex.glsl", "./src/fragment.glsl");
    const GLint positionAttribute = glGetAttribLocation(shaderProgram, "position");
    const GLint colorAttribute    = glGetAttribLocation(shaderProgram, "color");

    // GENERATE GPU BUFFERS
    GLuint vao;
    glGenVertexArrays(1, &vao);
    GLuint vbo[2];
    glGenBuffers(2, vbo);

    // BUFFER DATA TO GPU
    glBindVertexArray(vao);

    const GLfloat vertixPosition[6] = {-0.5,-0.5,   0.5, -0.5,   0.0, 0.5};
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertixPosition), vertixPosition, GL_STATIC_DRAW);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribute);

    const GLfloat vertixColor[9] = {0.0,0.0,0.0,  0.0,0.0,0.0,  0.0,0.0,0.0};
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertixColor), vertixColor, GL_STATIC_DRAW);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorAttribute);

    // GENERATE MATRICES


    // GAMELOOP
    bool running = true;
    glClearColor(1,1,1,0);

    while (running) {

        // INPUT
        glfwPollEvents();
        running = (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                   glfwWindowShouldClose(window) == 0);    
        
        // UPDATE 

        // RENDER
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glfwSwapBuffers(window);
    }

    // CLEANUP
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



// 
// COPY PASTE FROM LAB03 (with minor modification)
//
GLuint loadAndCompileShader(const char* fname, GLenum shaderType) {
    // Load a shader from an external file
    std::vector<char> buffer;
    {
        std::ifstream in;
        in.open(fname, std::ios::binary);
        
        if (in.fail()) {
            std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
            //std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
            exit(-1);
        }
        // Get the number of bytes stored in this file
        in.seekg(0, std::ios::end);
        size_t length = (size_t)in.tellg();

        // Go to start of the file
        in.seekg(0, std::ios::beg);

        // Read the content of the file in a buffer
        buffer.resize(length + 1);
        in.read(&buffer[0], length);
        in.close();
        // Add a valid C - string end
        buffer[length] = '\0';
    }
    const char* src = &buffer[0];

    // Create shaders
    GLuint shader = glCreateShader(shaderType);
    {
        //attach the shader source code to the shader objec
        glShaderSource(shader, 1, &src, NULL);

        // Compile the shader
        glCompileShader(shader);
        // Comile the shader, translates into internal representation and checks for errors.
        GLint compileOK;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOK);
        if (!compileOK) {
            char infolog[1024];;
            glGetShaderInfoLog(shader, 1024, NULL, infolog);
            std::cerr << "The program failed to compile with the error:" << std::endl << infolog << std::endl;
            glfwTerminate();
            getchar();
            exit(-1);
        }
    }
    return shader;
}

GLuint buildShaderProgram(const char* path_vert_shader, const char* path_frag_shader) {
    // Load and compile the vertex and fragment shaders
    GLuint vertexShader = loadAndCompileShader(path_vert_shader, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadAndCompileShader(path_frag_shader, GL_FRAGMENT_SHADER);

    // Create a program object and attach the two shaders we have compiled, the program object contains
    // both vertex and fragment shaders as well as information about uniforms and attributes common to both.
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Now that the fragment and vertex shader has been attached, we no longer need these two separate objects and should delete them.
    // The attachment to the shader program will keep them alive, as long as we keep the shaderProgram.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Link the different shaders that are bound to this program, this creates a final shader that 
    // we can use to render geometry with.
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}
// 
// END OF COPY PASTE FROM LAB03
//
