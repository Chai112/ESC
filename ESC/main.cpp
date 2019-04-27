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

#include "mouseInput.hpp"
#include "debug.hpp"
#include "render/texture.hpp"
#include "render/shader.hpp"

MouseIn mouse;
Debug debug;
V_Texture texture;
V_Shader shader;

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

glm::vec3 playerTransform = glm::vec3(4,3,3);
glm::vec3 playerVelocity = glm::vec3(0);
glm::vec3 playerAccel = glm::vec3(0);

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse.xPos = xpos;
    mouse.yPos = ypos;
}


int main() {
    assert(debug.restart_gl_log());
    // start GL context and O/S window using the GLFW helper library
    debug.gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    // register the error call-back function that we wrote, above
    glfwSetErrorCallback(glfw_error_callback);
    
    // uncomment these lines if on Apple OS xMouse
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (shader.init() == 1)
        return 1;
    GLFWwindow* window = shader.window;
    
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 4 / (float)3, 0.1f, 100.0f);
    
    // Or, for an ortho camera :
    //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    

    bool init = false;
    
    while(!glfwWindowShouldClose(window)) {
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
        
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
            playerAccel = glm::vec3(mouse.x(), 0, mouse.z());
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
            playerAccel = glm::vec3(-mouse.x(), 0, -mouse.z());
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
            playerAccel = glm::vec3(mouse.z(), 0, -mouse.x());
        }
        else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
            playerAccel = glm::vec3(-mouse.z(), 0, mouse.x());
        }
        
        playerAccel = playerVelocity * glm::vec3(-0.1);
        playerVelocity = playerVelocity + (playerAccel / glm::vec3(1));
        
        playerTransform = playerTransform + (playerVelocity / glm::vec3(1000));
        
        // draw points 0-3 from the currently bound VAO with current in-use shader
        
        // Camera matrix
        glm::mat4 View = glm::lookAt(
                                     playerTransform, // Camera is at (4,3,3), in World Space
                                     glm::vec3(mouse.x(), mouse.y(), mouse.z()) + playerTransform,
                                     glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                     );
        
        //gl_log("\nx %f %f\n", yMouse * 100, sinf(((yMouse-4.5)/4.5)*180*0.01745329252));
        
        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model = glm::mat4(1.0f);
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
        
        // Get a handle for our "MVP" uniform
        // Only during the initialisation
        GLuint MatrixID = glGetUniformLocation(shader.shader_programme, "MVP");
        
        glfwSetCursorPosCallback(window, cursor_position_callback);
        
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
        
        shader.update();
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        
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

