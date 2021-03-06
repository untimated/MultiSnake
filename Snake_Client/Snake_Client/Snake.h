//
//  Snake.h
//  Snake_Client
//
//  Created by Michael Herman on 12/3/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#ifndef __Snake_Client__Snake__
#define __Snake_Client__Snake__

#include <stdio.h>

class Snake{
    

    
public:
    Snake();
    ~Snake();
    
    void setPlayerPosX(unsigned char x);
    void setPlayerPosY(unsigned char y);
    unsigned char getPosX();
    unsigned char getPosY();
    
private:
    unsigned char positionX;
    unsigned char positionY;
    
};

#endif /* defined(__Snake_Client__Snake__) */
