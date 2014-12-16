//
//  Snake.cpp
//  Snake_Client
//
//  Created by Michael Herman on 12/3/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#include "Snake.h"




Snake::Snake()
{
    positionX = 1;
    positionY = 1;
}
Snake::~Snake()
{
}

void Snake::setPlayerPosX(unsigned char x)
{
    positionX = x;
}
void Snake::setPlayerPosY(unsigned char y)
{
    positionY = y;
}
unsigned char Snake::getPosX()
{
    return positionX;
}
unsigned char Snake::getPosY()
{
    return positionY;
}