//
//  BaseResourceManager.cpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseResourceManager.h"
#include <assert.h>


BaseResourceManager::BaseResourceManager() : currentFrameNumber(0)
{
    
}

BaseResourceManager::~BaseResourceManager()
{
    assert(currentFrameNumber == 0);
}

// @return new frame.
Frame* BaseResourceManager::createFrame(const FrameParams& params)
{
    Frame* res = new Frame();
    
    if (res)
    {
        *(dynamic_cast<FrameParams*>(res)) = params;
        res->byteSpan = res->width * 3;
        res->data = new uint8_t[res->byteSpan * res->height];
        
        currentFrameNumber++;
    }
    
    return res;
}

// Release created frame.
void BaseResourceManager::releaseFrame(Frame* frame)
{
    if (frame)
    {
        delete[] frame->data;
        delete frame;
        currentFrameNumber--;
    }
}


