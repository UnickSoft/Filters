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

class ImageControl : public QLabel
{
    Q_OBJECT

public:

    ImageControl (const QString& text, QWidget* parent);
    
Q_SIGNALS:

    void onMousePress(QMouseEvent * event);
    
protected:

    void mousePressEvent(QMouseEvent * event) override;


};


#endif /* ImageControl_hpp */
