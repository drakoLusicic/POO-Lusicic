/********************************************************************************
** Form generated from reading UI file 'ventana.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTANA_H
#define UI_VENTANA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ventana
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *backgroundLabel;
    QHBoxLayout *contentLayout;
    QLabel *photoLabel;
    QLabel *descriptionLabel;
    QLabel *profileDetailsLabel;

    void setupUi(QWidget *Ventana)
    {
        if (Ventana->objectName().isEmpty())
            Ventana->setObjectName("Ventana");
        Ventana->resize(1120, 780);
        verticalLayout = new QVBoxLayout(Ventana);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(Ventana);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        subtitleLabel = new QLabel(Ventana);
        subtitleLabel->setObjectName("subtitleLabel");
        subtitleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(subtitleLabel);

        backgroundLabel = new QLabel(Ventana);
        backgroundLabel->setObjectName("backgroundLabel");
        backgroundLabel->setMinimumSize(QSize(0, 300));
        backgroundLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(backgroundLabel);

        contentLayout = new QHBoxLayout();
        contentLayout->setObjectName("contentLayout");
        photoLabel = new QLabel(Ventana);
        photoLabel->setObjectName("photoLabel");
        photoLabel->setMinimumSize(QSize(180, 180));
        photoLabel->setAlignment(Qt::AlignCenter);

        contentLayout->addWidget(photoLabel);

        descriptionLabel = new QLabel(Ventana);
        descriptionLabel->setObjectName("descriptionLabel");
        descriptionLabel->setWordWrap(true);

        contentLayout->addWidget(descriptionLabel);

        profileDetailsLabel = new QLabel(Ventana);
        profileDetailsLabel->setObjectName("profileDetailsLabel");
        profileDetailsLabel->setWordWrap(true);

        contentLayout->addWidget(profileDetailsLabel);


        verticalLayout->addLayout(contentLayout);


        retranslateUi(Ventana);

        QMetaObject::connectSlotsByName(Ventana);
    } // setupUi

    void retranslateUi(QWidget *Ventana)
    {
        Ventana->setWindowTitle(QCoreApplication::translate("Ventana", "Ventana Principal", nullptr));
        titleLabel->setText(QCoreApplication::translate("Ventana", "Perfil profesional", nullptr));
        subtitleLabel->setText(QCoreApplication::translate("Ventana", "Bienvenido al resumen ejecutivo Dark Tech", nullptr));
        photoLabel->setText(QCoreApplication::translate("Ventana", "Foto", nullptr));
        descriptionLabel->setText(QCoreApplication::translate("Ventana", "Descripci\303\263n profesional", nullptr));
        profileDetailsLabel->setText(QCoreApplication::translate("Ventana", "Detalles profesionales", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Ventana: public Ui_Ventana {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTANA_H
