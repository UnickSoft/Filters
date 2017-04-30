//
//  BaseResourceManager.cpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseResourceManager.h"
#include <assert.h>
#include <cstring>

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
        uint32_t pixelByteSize = 0;
        switch (res->format)
        {
            case FrameParams::Alpha8: pixelByteSize = 1; break;
            case FrameParams::RGB8:   pixelByteSize = 3; break;
            case FrameParams::RGBA8:  pixelByteSize = 4; break;
            default: assert("Wrong pixel format");
        }
        res->byteSpan = res->width * pixelByteSize;
        res->data = new uint8_t[res->byteSpan * res->height];
        res->roi.x = 0;
        res->roi.y = 0;
        res->roi.width  = res->width;
        res->roi.height = res->height;
        
        std::memset(res->data, 0, res->byteSpan * res->height);
        
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


