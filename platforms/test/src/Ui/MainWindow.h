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
#include <QtWidgets/QTabWidget>

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
    void saveImage();
    
private:

    void fillDefaultParameters(const QVector<ParameterInfo>& parameterInfo);
    QWidget* createRenderSetup();
    void loadImage(ImageControl* control, const QString& filename);

    ImageControl* source1;
    ImageControl* dest1;
    ImageControl* source2;
    ImageControl* dest2;
    
    Controller& controller;
    QPixmap     currentImage;
    FilterControls  controls;
    QComboBox*  filterList;
    BaseParameterSet parameters;
    index_t filterIndex;
    QComboBox* renderFormats;
    QSettings settings;
    QTabWidget* previews;
    QLabel* renderTime;
};

#endif /* MainWindow_h */
