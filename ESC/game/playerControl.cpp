//
//  keyboardInput.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "playerControl.hpp"

#include "mouseInput.hpp"

#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/gtc/matrix_transform.hpp>

void G_PlayerControl::update (GLFWwindow* window, MouseIn mouseIn)
{
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP)) {
        G_PlayerControl::accel = glm::vec3(mouseIn.x(), 0, mouseIn.z());
    }
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN)) {
        G_PlayerControl::accel = glm::vec3(-mouseIn.x(), 0, -mouseIn.z());
    }
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT)) {
        G_PlayerControl::accel = glm::vec3(mouseIn.z(), 0, -mouseIn.x());
    }
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT)) {
        G_PlayerControl::accel = glm::vec3(-mouseIn.z(), 0, mouseIn.x());
    }
    else
    {
        G_PlayerControl::accel = G_PlayerControl::velocity * glm::vec3(-0.1);
    }
}
