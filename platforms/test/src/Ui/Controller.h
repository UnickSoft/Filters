//
//  Controller.hpp
//  UiTest
//
//  Created by Олег on 25.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp


#include "TestApplicationBridge.h"
#include "BaseResourceManager.h"
#include "Controller.h"
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtGui/QImage>

class Controller
{
public:

    Controller ();

    QList<QString> filters();
    
    void applyFilter(index_t index, QImage& source, QImage& dest);

protected:

    TestApplicationBridge bridge;
    BaseResourceManager resourceManager;
};



#include <stdio.h>

#endif /* Controller_hpp */
