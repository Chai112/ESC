//
//  mouseInput.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "mouseInput.hpp"
#include <glm/gtc/matrix_transform.hpp>

//glm::vec3((sinf(((-xMouse-7)/7)*180*0.01745329252) * zl), yl, (cosf(((-xMouse-7)/7)*180*0.01745329252) * zl)) + playerTransform, // and looks at the origin

double MouseIn::xMouse() {   return MouseIn::xPos / -100;    }

double MouseIn::yMouse() {   return MouseIn::yPos / -100;    }

float MouseIn::yl() {   return sinf(((MouseIn::yMouse()-4.5)/4.5)*180*0.01745329252);     }

float MouseIn::zl() {   return cosf(((-MouseIn::yMouse()-4.5)/4.5)*180*0.01745329252);    }

float MouseIn::x() {   return sinf(((-MouseIn::xMouse()-7)/7)*180*0.01745329252) * MouseIn::zl();  }

float MouseIn::y() {   return MouseIn::yl();   }

float MouseIn::z() {   return cosf(((-MouseIn::xMouse()-7)/7)*180*0.01745329252) * MouseIn::zl();  }
