/********************************************************************************
** Form generated from reading UI file 'dialogkeybind.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGKEYBIND_H
#define UI_DIALOGKEYBIND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogKeybind
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *DialogKeybind)
    {
        if (DialogKeybind->objectName().isEmpty())
            DialogKeybind->setObjectName(QString::fromUtf8("DialogKeybind"));
        DialogKeybind->resize(200, 60);
        verticalLayout_2 = new QVBoxLayout(DialogKeybind);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(DialogKeybind);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(DialogKeybind);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);


        retranslateUi(DialogKeybind);

        QMetaObject::connectSlotsByName(DialogKeybind);
    } // setupUi

    void retranslateUi(QDialog *DialogKeybind)
    {
        DialogKeybind->setWindowTitle(QApplication::translate("DialogKeybind", "Keybind", nullptr));
        label->setText(QApplication::translate("DialogKeybind", "Press any key to bind it.", nullptr));
        label_2->setText(QApplication::translate("DialogKeybind", "Press ESC to unbind.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogKeybind: public Ui_DialogKeybind {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGKEYBIND_H
