//
//  main.cpp
//  UiTest
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "UiTest.h"
#include <QtWidgets/QApplication>
#include "Ui/MainWindow.h"

extern "C" int runQtApp(int argc, const char * argv[])
{
    QApplication app(argc, (char**)argv);
    
    Controller controller;
    MainWindow mainWindow(controller);
    mainWindow.show();
    
    return app.exec();
}
