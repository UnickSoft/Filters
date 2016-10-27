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
#include "Controller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Controller& controller);


protected Q_SLOTS:

    void openImage();
    void applyFilter(QMouseEvent * event);
    void applyFilter(index_t index);
    void addFilter(int index);

private:

    ImageControl* source;
    ImageControl* dest;
    Controller& controller;
    QPixmap     currentImage;
};

#endif /* MainWindow_h */
