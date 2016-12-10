//
//  FrameHelper.cpp
//  UiTest
//
//  Created by Олег on 04.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "FrameEx.h"

FrameEx::FrameEx(const Frame& frame)
{
    *((Frame*)this) = frame;
}

// @return pixel depth in bytes.
uint32_t FrameEx::pixelDepth() const
{
    uint32_t res = 0;
    
    switch (format)
    {
        case RGBA8:  res = 4; break;
        case RGB8:   res = 3; break;
        case Alpha8: res = 1; break;
        default:     res = 0;
    }
    
    return res;
}

// @return pointer to buffer with offset.
uint8_t* FrameEx::dataPtr(uint32_t x, uint32_t y)
{
    return data ? data  + y  * byteSpan  + pixelDepth() * x : nullptr;
}

