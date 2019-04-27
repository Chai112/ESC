//
//  mouseInput.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef mouseInput_hpp
#define mouseInput_hpp

class MouseIn
{
    
public:
    double xPos;
    double yPos;
    
    float x ();
    float y ();
    float z ();
    
private:
    float yl ();
    float zl ();
    
    double xMouse ();
    double yMouse ();
    double yMo = 0;
    double yMoo = 0;
};

#endif /* mouseInput_hpp */
