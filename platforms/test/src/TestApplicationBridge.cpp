//
//  TestApplicationBridge.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "TestApplicationBridge.h"


// External function.
IFilterCollection* createCollection();

TestApplicationBridge::TestApplicationBridge()
{
    filterCollection = std::unique_ptr<IFilterCollection>(createCollection());
}

TestApplicationBridge::~TestApplicationBridge()
{
}

index_t TestApplicationBridge::filtersNumber()
{
    return filterCollection->filtersNumber();
}

std::shared_ptr<IFilter> TestApplicationBridge::createFilter(index_t index)
{
    return std::shared_ptr<IFilter>(filterCollection->createFilter(index));
}

void TestApplicationBridge::setResourceManager(IResourceManager* resourceManager)
{
    filterCollection->setResourceManager(resourceManager);
}

