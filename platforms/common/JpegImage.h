//
//  JpegImage.h
//  test
//
//  Created by Олег on 17.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include "Base.h"
#include <string>
#include "IResourceManager.h"

class JpegImage
{
public:
    JpegImage() = delete;

    static Frame* load(const std::string& filename, IResourceManager* resourceManager);
    static bool save(const std::string& filename, const Frame& frame);
};

