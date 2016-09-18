//
//  main.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include <iostream>
#include "TestApplicationBridge.h"
#include "JpegImage.h"
#include "BaseResourceManager.h"

int main(int argc, const char * argv[])
{
    if (argc == 3)
    {
        BaseResourceManager resourceManager;
        TestApplicationBridge bridge;
        
        bridge.setResourceManager(&resourceManager);
        
        std::string inputFile  = argv[1];
        std::string outputFile = argv[2];
        
        std::cout << "Filters number is " << bridge.filtersNumber() << std::endl;
        if (bridge.filtersNumber() > 0)
        {
            auto filter = bridge.createFilter(0);
            if (filter)
            {
                std::cout << "Created filter " << filter->name() << std::endl;
                Frame* inputFrame  = JpegImage::load(inputFile, &resourceManager);
                Frame* outputFrame = nullptr;
                if (inputFrame)
                {
                    outputFrame = resourceManager.createFrame(*inputFrame);

                    bool res = filter->apply(inputFrame, outputFrame, nullptr);
                    std::cout << "Apply was " << (res ? "Good" : "Bad") << std::endl;
                    res = JpegImage::save(outputFile, *outputFrame);
                    std::cout << "Save output file was " << (res ? "Good" : "Bad") << std::endl;
                }
                else
                {
                    std::cout << "Cannot load image files" << std::endl;
                }
                
                resourceManager.releaseFrame(inputFrame);
                resourceManager.releaseFrame(outputFrame);
            }
            else
            {
                std::cout << "Cannot create filter" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Params:" << std::endl;
        std::cout << "inputfilename outputfilename" << std::endl;
    }
    
    return 0;
}
