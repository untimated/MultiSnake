//
//  Renderer.cpp
//  Snake_Client
//
//  Created by Michael Herman on 12/8/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#include "Renderer.h"
#include <iostream>



Renderer::Renderer()
{
    
}

Renderer::~Renderer()
{
    
}

std::string Renderer::returnScreen(Snake p1, Snake p2)
{
    std::string screenData;
    unsigned char player1X;
    unsigned char player1Y;
    unsigned char player2X;
    unsigned char player2Y;
    player1X = p1.getPosX();
    player1Y = p1.getPosY();
    player2X = p2.getPosX();
    player2Y = p2.getPosY();
    
    for(unsigned char i = 0; i<= 20; i++)
    {
        for(unsigned char j = 0; j<=30; j++)
        {
            if((i == player1Y)&&(j==player1X))
            {
                screenData.append(">");
            }else{
                screenData.append("_");
            }
            
            if((i == player2Y)&&(j==player2X))
            {
                screenData.append("<");
            }else{
                screenData.append("_");
            }
        }
        screenData.append("\n");
    }
    return screenData;
    return 0;
}