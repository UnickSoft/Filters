//
//  FilterTemplates.hpp
//  UiTest
//
//  Created by Олег on 02.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef FilterTemplates_hpp
#define FilterTemplates_hpp

#include "Base.h"
#include "FrameEx.h"


/**
 * Call function for each pixel.
 * ROIs should be the same.
 * You can pass nullptr ROI for full images.
 */
template <typename T> bool processFrameToFramePixel(T function, FrameEx& inputFrame, FrameEx& outputFrame, const ROI* inputROI = nullptr, const ROI* outputROI = nullptr);




#include "FilterTemplatesImpl.h"

#endif /* FilterTemplates_hpp */
