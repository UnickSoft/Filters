//
//  Base.h
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef Base_h
#define Base_h

#include <cstdlib>

typedef size_t index_t;


struct FrameParams
{
    enum PixelFormat {RGBA8, RGB8, Alpha8};

    uint32_t width;
    uint32_t height;
    PixelFormat format;
};

struct Frame : FrameParams
{
    uint8_t* data;
    uint32_t byteSpan;
};

// Struct to hold any type of parameter.
struct Parameter;

// This struct hold parameter info.
struct ParameterInfo;


// TODO: we will use COM. But later.
class VirtualDestructor
{
public:
    virtual ~VirtualDestructor() {}
};

#endif /* Base_h */
