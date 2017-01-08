//
//  FrameEx.hpp
//  FrameEx is wrapper for Frame. It is simplify work with it.
//  UiTest
//
//  Created by Олег on 04.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef FrameHelper_hpp
#define FrameHelper_hpp

#include <stdio.h>
#include "Base.h"

class FrameEx : public Frame
{
public:

    FrameEx(const Frame& frame);

    // @return pixel depth in bytes.
    uint32_t pixelDepth() const;
    
    // @return pointer to buffer with offset.
    uint8_t* dataPtr(uint32_t x = 0, uint32_t y = 0);
    
    static bool isEquals(const FrameParams& frameParams1, const FrameParams& frameParams2);
};



#endif /* FrameHelper_hpp */
