/********************************************************************************
** Form generated from reading UI file 'MyImageViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYIMAGEVIEWER_H
#define UI_MYIMAGEVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "qimageviewer.h"

QT_BEGIN_NAMESPACE

class Ui_MyImageViewerClass
{
public:
    QGridLayout *gridLayout;
    QImageViewer *widget;
    QLabel *label;

    void setupUi(QWidget *MyImageViewerClass)
    {
        if (MyImageViewerClass->objectName().isEmpty())
            MyImageViewerClass->setObjectName(QStringLiteral("MyImageViewerClass"));
        MyImageViewerClass->resize(929, 668);
        gridLayout = new QGridLayout(MyImageViewerClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QImageViewer(MyImageViewerClass);
        widget->setObjectName(QStringLiteral("widget"));

        gridLayout->addWidget(widget, 1, 0, 1, 1);

        label = new QLabel(MyImageViewerClass);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        gridLayout->setRowStretch(1, 1);

        retranslateUi(MyImageViewerClass);

        QMetaObject::connectSlotsByName(MyImageViewerClass);
    } // setupUi

    void retranslateUi(QWidget *MyImageViewerClass)
    {
        MyImageViewerClass->setWindowTitle(QApplication::translate("MyImageViewerClass", "MyImageViewer", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyImageViewerClass: public Ui_MyImageViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYIMAGEVIEWER_H
