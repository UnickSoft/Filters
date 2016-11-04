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
#include <QtCore/QMap>

class Controller : public QObject
{
    Q_OBJECT

public:

    Controller ();

    QList<QString> filters();
    
    void applyFilter(index_t index, const IParameterSet* parameters, QImage& source, QImage& dest);
    
    QVector<ParameterInfo> parameterList(index_t index);
    
protected:

    TestApplicationBridge bridge;
    BaseResourceManager resourceManager;
    QMap<QImage::Format, FrameParams::PixelFormat> formatMap;
};



#include <stdio.h>

#endif /* Controller_hpp */
