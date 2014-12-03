//
//  Controller.h
//  Snake_Client
//
//  Created by Michael Herman on 12/3/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#ifndef __Snake_Client__Controller__
#define __Snake_Client__Controller__

#include <stdio.h>
#include "Renderer.h"
#include "Snake.h"
#include <ncurses.h>



class Controller{
    
    Renderer gameDrawer;
    Snake playerOne;
    
    public:
    Controller();
    ~Controller();
    
    void handleEvent(int keyPressed);
    void RenderWorld();
    void updateModel(char* tmpServerMessage);
    
    
    
};

#endif /* defined(__Snake_Client__Controller__) */
