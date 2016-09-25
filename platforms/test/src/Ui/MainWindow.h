//
//  MainWindow.hpp
//  UiTest
//
//  Created by Олег on 20.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef MainWindow_hpp
#define MainWindow_hpp

#include <stdio.h>
#include <QtWidgets/QMainWindow>
#include "ImageControl.h"
#include "TestApplicationBridge.h"
#include "BaseResourceManager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


protected Q_SLOTS:

    void openImage();
    void applyFilter(QMouseEvent * event);

private:

    void applyFilter(index_t index);

    ImageControl* source;
    ImageControl* dest;
    
    TestApplicationBridge bridge;
    BaseResourceManager resourceManager;

};

#endif /* MainWindow_h */
