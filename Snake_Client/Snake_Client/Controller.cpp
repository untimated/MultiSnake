//
//  Controller.cpp
//  Snake_Client
//
//  Created by Michael Herman on 12/8/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#include "Controller.h"



struct __attribute__((packed)) message2{
    unsigned char typeId;
    unsigned char posX;
    unsigned char posY;
}clientMessage2;

struct __attribute__((packed)) retrievedServerData{
    unsigned char typeId;
    unsigned char info[4];
};

Snake playerOne;
Snake playerTwo;
Renderer gameRenderer;

Controller:: Controller()
{
    playerOne = *new Snake();
    playerTwo = *new Snake();

}

Controller::~Controller()
{
}

void Controller::proccessEvent(int evt,char port[10])
{
    if (strcmp(port, "9000")==0) {
        switch (evt)
        {
            case KEY_DOWN:
                playerOne.setPlayerPosY(playerOne.getPosY()+2);
            break;
            case KEY_RIGHT:
                playerOne.setPlayerPosX(playerOne.getPosX()+2);
            break;
            default:
            
            break;
        }
    }else if(strcmp(port,"9001")==0)
    {
        switch (evt)
        {
            case KEY_DOWN:
                playerTwo.setPlayerPosY(playerTwo.getPosY() + 1);
                break;
            case KEY_RIGHT:
                playerTwo.setPlayerPosX(playerTwo.getPosX() + 1);
                break;
            default:
                
                break;
        }
    }
}

void Controller::updateWorld(unsigned char *snakeData)
{
    struct retrievedServerData *playerData = (struct retrievedServerData*) snakeData;
    playerOne.setPlayerPosX(playerData->info[0]);
    playerOne.setPlayerPosY(playerData->info[1]);
    
    playerTwo.setPlayerPosX(playerData->info[2]);
    playerTwo.setPlayerPosY(playerData->info[3]);

}

char* Controller::returnSnakeData(char port[10])
{
    if(strcmp(port, "9000")==0)
    {
        unsigned char p1X = playerOne.getPosX();
        unsigned char p1Y = playerOne.getPosY();
        clientMessage2.posX = p1X;
        clientMessage2.posY = p1Y;
        
    }else if(strcmp(port, "9001")==0)
    {
        unsigned char p2X = playerTwo.getPosX();
        unsigned char p2Y = playerTwo.getPosY();
        clientMessage2.posX = p2X;
        clientMessage2.posY = p2Y;
    }
    clientMessage2.typeId = 100;
    void *ptr = (void*) &clientMessage2;
    char *txt = (char*) ptr;
    return txt;
}


std::string Controller::renderWorld()
{
    return gameRenderer.returnScreen(playerOne, playerTwo);
}

unsigned char Controller::getSnakeX(char port[10])
{
    if(strcmp(port, "9000")==0)
    {
        return playerOne.getPosX();
    }else if(strcmp(port, "9001")==0)
    {
        return playerTwo.getPosX();
    }else{
        return 0;
    }
}

unsigned char Controller::getSnakeY(char port[10])
{
    if(strcmp(port, "9000")==0)
    {
        return playerTwo.getPosY();
    }else if(strcmp(port, "9001")==0)
    {
        return playerTwo.getPosY();
    }else{
        return 0;
    }
}