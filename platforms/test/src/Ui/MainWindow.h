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
#include "FilterControls.h"
#include <QtWidgets/QComboBox>
#include "BaseParameterSet.h"
#include <QtCore/QSettings>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Controller& controller);
    
protected slots:

    void openImage();
    void applyFilter(QMouseEvent * event);
    void applyFilter(index_t index);
    void addFilter(int index);
    void paramChanged(index_t index, const Parameter& value);
    void renderFormatChanged(int index);

private:

    void fillDefaultParameters(const QVector<ParameterInfo>& parameterInfo);
    QWidget* createRenderSetup();
    void loadImage(const QString& filename);

    ImageControl* source;
    ImageControl* dest;
    Controller& controller;
    QPixmap     currentImage;
    FilterControls  controls;
    QComboBox*  filterList;
    BaseParameterSet parameters;
    index_t filterIndex;
    QComboBox* renderFormats;
    QSettings settings;
};

#endif /* MainWindow_h */
