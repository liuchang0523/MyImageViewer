#include "MyImageViewer.h"

MyImageViewer::MyImageViewer(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.widget->setImage(QImage("lena.bmp"));
}

void MyImageViewer::on_widget_Coordinate(QString str)
{
    ui.label->setText(str);
}
