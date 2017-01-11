//
//  ImageControl.hpp
//  UiTest
//
//  Created by Олег on 22.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef ImageControl_hpp
#define ImageControl_hpp

#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>

class ImageControl : public QWidget
{
    Q_OBJECT

public:

    ImageControl (const QString& text, QWidget* parent);
    void setImage(const QPixmap& pixmap);
    const QPixmap& image();
    
Q_SIGNALS:

    void onMousePress(QMouseEvent * event);
    
protected:

    void mousePressEvent(QMouseEvent * event) override;
    
    QPixmap _image;
    QLabel*  _imageLabel;
    QScrollArea* _scrollArea;


};


#endif /* ImageControl_hpp */
