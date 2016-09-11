//
//  IResourceManager.h
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef IResourceManager_h
#define IResourceManager_h

#include "Base.h"

class IResourceManager : public VirtualDestructor
{
public:

    // @return new frame.
    virtual Frame* createFrame(const FrameParams&) = 0;
    
    // Release created frame.
    virtual void releaseFrame(Frame*) = 0;
};

#endif /* IResourceManager_h */
