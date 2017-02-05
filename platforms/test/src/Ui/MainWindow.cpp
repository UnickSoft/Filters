//
//  MainWindow.cpp
//  UiTest
//
//  Created by Олег on 20.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "MainWindow.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtCore/QBuffer>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QApplication>
#include <QtCore/QElapsedTimer>
#include <QtWidgets/QPushButton>


static const QString previewImageKey = "testImage";

MainWindow::MainWindow(Controller& controller) : controller(controller), filterIndex(0), settings("UnickSoft", "TestUI")
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    
    // Set layout in QWidget
    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    
    // Left and right parts of main window.
    QWidget* leftPart  = new QWidget();
    QWidget* rightPart = new QWidget();

    QVBoxLayout* preview = new QVBoxLayout();
    rightPart->setLayout(preview);

    QVBoxLayout* controlsLayout = new QVBoxLayout();
    leftPart->setLayout(controlsLayout);

    mainLayout->addWidget(leftPart, 1);
    mainLayout->addWidget(rightPart, 1);
    
    // Create effect controls.
    filterList = new QComboBox();
    auto filters = controller.filters();
    for (auto filter : filters)
    {
        filterList->addItem(filter);
    }
    
    // Preset saving.
    QWidget* presetPanel = new QWidget();
    auto presetPanelLayout = new QHBoxLayout();
    presetPanel->setLayout(presetPanelLayout);
    
    auto presetNameLable = new QLabel("Preset name", presetPanel);
    presetPanelLayout->addWidget(presetNameLable);
    
    _presetName = new QLineEdit("Preset", presetPanel);
    presetPanelLayout->addWidget(_presetName);
    
    QPushButton* savePreset = new QPushButton("Save preset", presetPanel);
    presetPanelLayout->addWidget(savePreset);
    
    QPushButton* loadPreset = new QPushButton("Load preset", presetPanel);
    presetPanelLayout->addWidget(loadPreset);
    
    controlsLayout->addWidget(filterList);
    controlsLayout->addWidget(&controls);
    controlsLayout->addWidget(presetPanel);
    controlsLayout->addWidget(createRenderSetup());
    controlsLayout->addStretch();
    
    // Setup preview outputs.
    source1 = new ImageControl("Source (Click to open image)", this);
    source2 = new ImageControl("Source (Click to open image)", this);
    
    source1->setObjectName("Source1");
    source2->setObjectName("Source2");

    dest1   = new ImageControl("Dest (Click to open filter)", this);
    dest2   = new ImageControl("Dest (Click to open filter)", this);
    
    previews = new QTabWidget();
    
    preview->addWidget(previews);
    
    previews->addTab(source1, "Source 1");
    previews->addTab(source2, "Source 2");
    previews->addTab(dest1, "Dest 1");
    previews->addTab(dest2, "Dest 2");
    
    
    // Set QWidget as the central layout of the main window
    setCentralWidget(window);

    connect(filterList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::addFilter);
    connect(source1, &ImageControl::onMousePress, this, &MainWindow::openImage);
    connect(source2, &ImageControl::onMousePress, this, &MainWindow::openImage);
    
    connect(&controls, &FilterControls::paramChanged, this, &MainWindow::paramChanged);
    connect(renderFormats, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::renderFormatChanged);
    
    connect(dest1, &ImageControl::onMousePress, this, &MainWindow::saveImage);
    connect(dest2, &ImageControl::onMousePress, this, &MainWindow::saveImage);
    
    connect(savePreset, &QPushButton::clicked, this, &MainWindow::savePreset);
    connect(loadPreset, &QPushButton::clicked, this, &MainWindow::loadPreset);
    
    resize(880, 600);

    if (settings.contains(previewImageKey))
    {
        loadImage(source1, settings.value(previewImageKey + source1->objectName()).toString());
        loadImage(source2, settings.value(previewImageKey + source2->objectName()).toString());
    }
    else
    {
        loadImage(source1, QApplication::applicationDirPath() + "/../Resources/resources/Lenna.png");
    }
    
    selectFilter((index_t)0, nullptr);
}


void MainWindow::openImage()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "~/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
    loadImage(static_cast<ImageControl*>(sender()), filename);
}

void MainWindow::loadImage(ImageControl* control, const QString& filename)
{
    if (!filename.isEmpty())
    {
        settings.setValue(previewImageKey + control->objectName(), filename);
        control->setImage(QPixmap(filename));
    }
}

void MainWindow::applyFilter(QMouseEvent * event)
{
    QMenu filterList;
    
    auto filters = controller.filters();
    
    for (index_t i = 0; i < filters.size(); i ++)
    {
        filterList.addAction(filters[(int)i], [=]()
                             {
                                 this->applyFilter(i);
                             });
    }
    
    filterList.exec(event->globalPos());
}

void MainWindow::applyFilter(index_t index)
{
    auto sourcePix1 = source1->image();
    auto sourcePix2 = source2->image();
    
    if (!sourcePix1.isNull())
    {
        QImage::Format renderFormat = (QImage::Format)renderFormats->itemData(renderFormats->currentIndex()).toInt();
        QImage destImage;
        
        QVector<QImage> source;
        QSize renderFrameSize = source1->image().size();
        
        auto sourceImage1 = sourcePix1.toImage().scaled(renderFrameSize).convertToFormat(renderFormat);
        source.push_back(sourceImage1);
        
        if (!sourcePix2.isNull())
        {
            auto sourceImage2 = sourcePix2.toImage().scaled(renderFrameSize).convertToFormat(renderFormat);
            source.push_back(sourceImage2);
        }
        else
        {
            source.push_back(sourceImage1);
        }
        
        QVector<QImage> dest;
        
        QElapsedTimer timer;
        timer.start();
        controller.applyFilter(index, &parameters, source, dest);
        
        renderTime->setText(QString::number(timer.elapsed()) + " ms " +
            QString::number(renderFrameSize.width()) + "x" + QString::number(renderFrameSize.height()));
        
        dest1->setImage(QPixmap::fromImage(dest[0].convertToFormat(QImage::Format_RGBA8888)));
        
        if (dest.size() > 1)
        {
            dest2->setImage(QPixmap::fromImage(dest[1].convertToFormat(QImage::Format_RGBA8888)));
        }
    }
}

void MainWindow::addFilter(int index)
{
    selectFilter(index, nullptr);
}

void MainWindow::selectFilter(int index, const IParameterSet* newValue)
{
    filterIndex = index;
    auto parameterInfo = controller.parameterList(index);
    
    if (newValue)
    {
        for (int i = 0; i < newValue->parametersNumber(); i++)
        {
            parameterInfo[i].defaultValue = newValue->value(i);
        }
    }
    
    fillDefaultParameters(parameterInfo);

    controls.setFilter(filterList->currentText(), parameterInfo);
    
    applyFilter(index);
}

void MainWindow::paramChanged(index_t index, const Parameter& value)
{
    parameters[index] = value;
    
    applyFilter(filterIndex);
}

void MainWindow::fillDefaultParameters(const QVector<ParameterInfo>& parameterInfo)
{
    parameters.clear();
    
    for (auto paramInfo : parameterInfo)
    {
        parameters.push_back(paramInfo.defaultValue);
    }
}

QWidget* MainWindow::createRenderSetup()
{
    auto res = new QWidget();
    QVBoxLayout* setupLayout = new QVBoxLayout();
    res->setLayout(setupLayout);
    
    auto title = new QLabel("Render setup");
    setupLayout->addWidget(title);
    
    renderFormats = new QComboBox();
    renderFormats->addItem("RGBA8",  QImage::Format_RGBA8888);
    renderFormats->addItem("RGB8",   QImage::Format_RGB888);
    renderFormats->addItem("Alpha8", QImage::Format_Alpha8);
    
    setupLayout->addWidget(renderFormats);
    
    auto renderTimeTitle = new QLabel("Render time and size");
    setupLayout->addWidget(renderTimeTitle);
    
    renderTime = new QLabel("0 1x1");
    setupLayout->addWidget(renderTime);
    
    return res;
}

void MainWindow::renderFormatChanged(int index)
{
    applyFilter(filterIndex);
}

void MainWindow::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "~/Pictures/result.png", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!filename.isEmpty())
    {
        static_cast<ImageControl*>(sender())->image().toImage().convertToFormat(QImage::Format_RGBA8888).save(filename);
    }
}

void MainWindow::savePreset()
{
    QString presetName = _presetName->text();
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Preset"), "~/Documents/" + presetName + ".json", tr("Preset files (*.json)"));
    if (!filename.isEmpty())
    {
        controller.savePreset(parameters, filterIndex, presetName, filename);
    }
}

void MainWindow::loadPreset()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Preset"), "~/Documents", tr("Preset files (*.json)"));
    if (!filename.isEmpty())
    {
        FilterPreset preset = controller.loadPreset(filename);
        auto filters = controller.filters();
        int index = filters.indexOf(QString::fromStdString(preset.filterName()));
        if (index >= 0)
        {
            filterList->setCurrentIndex(index);
            selectFilter(index, &preset);
            _presetName->setText(QString::fromStdString(preset.name()));
        }
    }
}
