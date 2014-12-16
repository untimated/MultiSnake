//
//  Controller.h
//  Snake_Client
//
//  Created by Michael Herman on 12/8/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#ifndef __Snake_Client__Controller__
#define __Snake_Client__Controller__

#include "Snake.h"
#include "Renderer.h"
#include <ncurses.h>
#include <string>

class Controller{
    
public:
    Controller();
    ~Controller();
    
    void proccessEvent(int evt, char port[10]);
    std::string renderWorld();
    void updateWorld(unsigned char* snakeData);
    char* returnSnakeData(char port[10]);
    
    unsigned char getSnakeX(char port[10]);
    unsigned char getSnakeY(char port[10]);
};

#endif /* defined(__Snake_Client__Controller__) */
