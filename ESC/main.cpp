//
//  main.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 22/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include <stdarg.h>
#include <assert.h>

#include "game/playerControl.hpp"
#include "game/playerPhysics.hpp"

#include "input/mouseInput.hpp"

#include "render/texture.hpp"
#include "render/shader.hpp"
#include "render/perspective.hpp"

#include "debug.hpp"

MouseIn mouseIn;

G_PlayerControl playerControl;
G_PlayerPhysics playerPhysics;

V_Texture texture;
V_Shader shader;
V_Perspective perspective;

Debug debug;

#define GL_LOG_FILE "/Users/chaidhatchaimongkol/Documents/ESC/ESC/log/gl.log"
#define GL_TEXTURE_FILE "/Users/chaidhatchaimongkol/Documents/ESC/gl.bmp"

void glfw_error_callback(int error, const char* description) {
    debug.gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouseIn.xPos = xpos;
    mouseIn.yPos = ypos;
}


int main() {
    assert(debug.restart_gl_log());
    // start GL context and O/S window using the GLFW helper library
    debug.gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    // register the error call-back function that we wrote, above
    glfwSetErrorCallback(glfw_error_callback);
    
    playerPhysics.playerTransform = glm::vec3(4,3,3);
    
    // uncomment these lines if on Apple OS xMouse
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (shader.init() == 1)
        return 1;
    
    if (mouseIn.init(shader.window) == 1)
        return 1;
    

    bool init = false;
    
    while(!glfwWindowShouldClose(shader.window)) {
        if (GLFW_PRESS == glfwGetKey(shader.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(shader.window, 1);
        }
        
        playerControl.velocity = playerPhysics.playerVelocity;
        playerControl.update(shader.window, mouseIn);
        
        playerPhysics.playerAccel = playerControl.accel;
        playerPhysics.update();
        perspective.update(shader.window, mouseIn, playerPhysics.playerTransform, shader.shader_programme);
        
        glfwSetCursorPosCallback(shader.window, cursor_position_callback);
        
        shader.update();
        
        
        if (!init)
        {
            init = true;
            debug.gl_log("\nInitialised Successfully. %s %s \n", __DATE__, __TIME__);
        }
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

