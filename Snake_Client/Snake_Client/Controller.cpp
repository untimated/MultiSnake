//
//  Controller.cpp
//  Snake_Client
//
//  Created by Michael Herman on 12/3/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#include "Controller.h"


struct __attribute__((packed)) message2{
    unsigned char typeId;
    unsigned char info[4];
};

Controller::Controller()
{
    
}

Controller::~Controller(){
    
}

void handleEvent(int keyPressed)
{
    switch (keyPressed)
    {
        case KEY_DOWN:
            
            break;
        case KEY_RIGHT:
            break;
        default:
            
            break;
    }
}

void RenderWorld()
{
    
}
void updateModel(char* tmpServerMessage)
{
    
}
