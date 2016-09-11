//
//  main.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include <iostream>
#include "TestApplicationBridge.h"



int main(int argc, const char * argv[])
{
    TestApplicationBridge bridge;
    
    std::cout << "Filters number is " << bridge.filtersNumber() << std::endl;
    if (bridge.filtersNumber() > 0)
    {
        auto filter = bridge.createFilter(0);
        if (filter)
        {
            std::cout << "Created filter " << filter->name() << std::endl;
            Frame inputFrame, outputFrame;
            bool res = filter->apply(inputFrame, outputFrame, nullptr);
            std::cout << "Apply was " << (res ? "Good" : "Bad") << std::endl;
        }
        else
        {
            std::cout << "Cannot create filter" << std::endl;
        }
    }
    
    return 0;
}
