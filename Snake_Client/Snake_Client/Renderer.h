//
//  Renderer.h
//  Snake_Client
//
//  Created by Michael Herman on 12/8/14.
//  Copyright (c) 2014 Michael Herman. All rights reserved.
//

#ifndef __Snake_Client__Renderer__
#define __Snake_Client__Renderer__

#include <stdio.h>
#include "Snake.h"
#include <string>

class Renderer{
public:
    Renderer();
    ~Renderer();
    std::string returnScreen(Snake p1, Snake p2);
    
};

#endif /* defined(__Snake_Client__Renderer__) */
