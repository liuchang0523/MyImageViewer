#pragma once

#include <QtWidgets/QWidget>
#include "ui_MyImageViewer.h"

class MyImageViewer : public QWidget
{
    Q_OBJECT

public:
    MyImageViewer(QWidget *parent = Q_NULLPTR);

private slots:
    void on_widget_Coordinate(QString str);

private:
    Ui::MyImageViewerClass ui;
};
