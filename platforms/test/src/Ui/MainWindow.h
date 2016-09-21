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
#include <QtWidgets/QLabel>


class MainWindow : public QMainWindow
{
public:
    MainWindow();


private:

    QLabel* source;
    QLabel* dest;

};

#endif /* MainWindow_h */
