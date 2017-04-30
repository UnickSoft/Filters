//
//  BaseResourceManager.hpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef BaseResourceManager_hpp
#define BaseResourceManager_hpp

#include "IResourceManager.h"
#include <atomic>

class BaseResourceManager : public IResourceManager
{
public:
    
    BaseResourceManager();
    virtual ~BaseResourceManager();
    
    // @return new frame.
    Frame* createFrame(const FrameParams&) override;
    
    // Release created frame.
    void releaseFrame(Frame*) override;

protected:

    // Use this counter to check memory leaks.
    std::atomic<std::uint32_t> currentFrameNumber;
    
};


#endif /* BaseResourceManager_hpp */
