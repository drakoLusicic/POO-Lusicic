/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *frameCanvas;
    QVBoxLayout *verticalLayoutCanvas;
    QWidget *paintPlaceholder;
    QLabel *labelMessage;
    QFrame *frameControls;
    QVBoxLayout *verticalLayoutControls;
    QLabel *labelTitle;
    QLabel *labelCount;
    QLabel *labelColor;
    QLabel *labelWidth;
    QWidget *layoutButtonsWidget;
    QVBoxLayout *verticalLayoutButtons;
    QPushButton *btnSave;
    QPushButton *btnLoad;
    QPushButton *btnClear;
    QSpacerItem *verticalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        frameCanvas = new QFrame(centralwidget);
        frameCanvas->setObjectName("frameCanvas");
        frameCanvas->setFrameShape(QFrame::StyledPanel);
        frameCanvas->setFrameShadow(QFrame::Raised);
        verticalLayoutCanvas = new QVBoxLayout(frameCanvas);
        verticalLayoutCanvas->setObjectName("verticalLayoutCanvas");
        paintPlaceholder = new QWidget(frameCanvas);
        paintPlaceholder->setObjectName("paintPlaceholder");
        paintPlaceholder->setMinimumSize(QSize(520, 420));

        verticalLayoutCanvas->addWidget(paintPlaceholder);

        labelMessage = new QLabel(frameCanvas);
        labelMessage->setObjectName("labelMessage");
        labelMessage->setAlignment(Qt::AlignCenter);

        verticalLayoutCanvas->addWidget(labelMessage);


        horizontalLayout->addWidget(frameCanvas);

        frameControls = new QFrame(centralwidget);
        frameControls->setObjectName("frameControls");
        frameControls->setMaximumSize(QSize(320, 16777215));
        frameControls->setFrameShape(QFrame::StyledPanel);
        frameControls->setFrameShadow(QFrame::Raised);
        verticalLayoutControls = new QVBoxLayout(frameControls);
        verticalLayoutControls->setObjectName("verticalLayoutControls");
        labelTitle = new QLabel(frameControls);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setAlignment(Qt::AlignCenter);
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        labelTitle->setFont(font);

        verticalLayoutControls->addWidget(labelTitle);

        labelCount = new QLabel(frameControls);
        labelCount->setObjectName("labelCount");

        verticalLayoutControls->addWidget(labelCount);

        labelColor = new QLabel(frameControls);
        labelColor->setObjectName("labelColor");

        verticalLayoutControls->addWidget(labelColor);

        labelWidth = new QLabel(frameControls);
        labelWidth->setObjectName("labelWidth");

        verticalLayoutControls->addWidget(labelWidth);

        layoutButtonsWidget = new QWidget(frameControls);
        layoutButtonsWidget->setObjectName("layoutButtonsWidget");
        verticalLayoutButtons = new QVBoxLayout(layoutButtonsWidget);
        verticalLayoutButtons->setObjectName("verticalLayoutButtons");
        verticalLayoutButtons->setContentsMargins(0, 0, 0, 0);
        btnSave = new QPushButton(layoutButtonsWidget);
        btnSave->setObjectName("btnSave");

        verticalLayoutButtons->addWidget(btnSave);

        btnLoad = new QPushButton(layoutButtonsWidget);
        btnLoad->setObjectName("btnLoad");

        verticalLayoutButtons->addWidget(btnLoad);

        btnClear = new QPushButton(layoutButtonsWidget);
        btnClear->setObjectName("btnClear");

        verticalLayoutButtons->addWidget(btnClear);


        verticalLayoutControls->addWidget(layoutButtonsWidget);

        verticalSpacer = new QSpacerItem(20, 200, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayoutControls->addItem(verticalSpacer);


        horizontalLayout->addWidget(frameControls);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        labelMessage->setText(QCoreApplication::translate("MainWindow", "Use R/G/B, rueda del mouse, Escape y Ctrl+Z desde el lienzo.", nullptr));
        labelTitle->setText(QCoreApplication::translate("MainWindow", "Panel de Control", nullptr));
        labelCount->setText(QCoreApplication::translate("MainWindow", "Trazos: 0", nullptr));
        labelColor->setText(QCoreApplication::translate("MainWindow", "Color: #00FFFF", nullptr));
        labelWidth->setText(QCoreApplication::translate("MainWindow", "Grosor: 6 px", nullptr));
        btnSave->setText(QCoreApplication::translate("MainWindow", "Guardar dibujo", nullptr));
        btnLoad->setText(QCoreApplication::translate("MainWindow", "Cargar \303\272ltimo", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Limpiar lienzo", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
