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


// TODO define this classes later.
struct ParameterInfo {};

struct FrameParams
{
    uint32_t width;
    uint32_t height;
};

struct Frame : FrameParams
{
    uint32_t byteSpan;
    uint8_t* data;
};

struct Parameter {};


// TODO: we will use COM. But later.
class VirtualDestructor
{
public:
    virtual ~VirtualDestructor() {}
};

#endif /* Base_h */
