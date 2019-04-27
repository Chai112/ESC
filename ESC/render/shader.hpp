//
//  shader.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

class V_Shader
{
public:
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "layout(location = 0) in vec3 vertexPosition_modelspace;"
    ""
    "uniform mat4 MVP;"
    ""
    "void main(){"
    "   gl_Position =  MVP * vec4(vertexPosition_modelspace,1.0);"
    "}";
    
    const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";
    
    GLuint vbo = 0;
    GLuint vao = 0;
    GLuint vs, fs, shader_programme;
    
    GLFWwindow* window;
    
    int init ();
    void update ();
};

#endif /* shader_hpp */
