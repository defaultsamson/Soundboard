/********************************************************************************
** Form generated from reading UI file 'dialogsound.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSOUND_H
#define UI_DIALOGSOUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgetaudio.h"
#include "widgetkeybind.h"

QT_BEGIN_NAMESPACE

class Ui_DialogSound
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QLabel *labelKeybind;
    WidgetKeybind *lineEdiKeybind;
    QLabel *labelFile;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEditFile;
    QPushButton *pushButtonFile;
    QLabel *label;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSlider *sliderVolume;
    QSpacerItem *horizontalSpacer;
    QSpinBox *spinBoxVolume;
    QHBoxLayout *horizontalLayout_3;
    WidgetAudio *widget_3;
    QPushButton *pushButton_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogSound)
    {
        if (DialogSound->objectName().isEmpty())
            DialogSound->setObjectName(QString::fromUtf8("DialogSound"));
        DialogSound->resize(400, 282);
        DialogSound->setMaximumSize(QSize(16777215, 282));
        verticalLayout = new QVBoxLayout(DialogSound);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        labelName = new QLabel(DialogSound);
        labelName->setObjectName(QString::fromUtf8("labelName"));

        verticalLayout->addWidget(labelName);

        lineEditName = new QLineEdit(DialogSound);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        lineEditName->setClearButtonEnabled(true);

        verticalLayout->addWidget(lineEditName);

        labelKeybind = new QLabel(DialogSound);
        labelKeybind->setObjectName(QString::fromUtf8("labelKeybind"));

        verticalLayout->addWidget(labelKeybind);

        lineEdiKeybind = new WidgetKeybind(DialogSound);
        lineEdiKeybind->setObjectName(QString::fromUtf8("lineEdiKeybind"));
        lineEdiKeybind->setCursor(QCursor(Qt::ArrowCursor));
        lineEdiKeybind->setFocusPolicy(Qt::ClickFocus);
        lineEdiKeybind->setReadOnly(true);
        lineEdiKeybind->setClearButtonEnabled(false);

        verticalLayout->addWidget(lineEdiKeybind);

        labelFile = new QLabel(DialogSound);
        labelFile->setObjectName(QString::fromUtf8("labelFile"));

        verticalLayout->addWidget(labelFile);

        widget = new QWidget(DialogSound);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEditFile = new QLineEdit(widget);
        lineEditFile->setObjectName(QString::fromUtf8("lineEditFile"));
        lineEditFile->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(lineEditFile);

        pushButtonFile = new QPushButton(widget);
        pushButtonFile->setObjectName(QString::fromUtf8("pushButtonFile"));

        horizontalLayout_2->addWidget(pushButtonFile);


        verticalLayout->addWidget(widget);

        label = new QLabel(DialogSound);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        widget_2 = new QWidget(DialogSound);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        sliderVolume = new QSlider(widget_2);
        sliderVolume->setObjectName(QString::fromUtf8("sliderVolume"));
        sliderVolume->setMaximum(100);
        sliderVolume->setValue(100);
        sliderVolume->setOrientation(Qt::Horizontal);
        sliderVolume->setTickInterval(1);

        horizontalLayout->addWidget(sliderVolume);

        horizontalSpacer = new QSpacerItem(1, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        spinBoxVolume = new QSpinBox(widget_2);
        spinBoxVolume->setObjectName(QString::fromUtf8("spinBoxVolume"));
        spinBoxVolume->setMaximum(100);
        spinBoxVolume->setValue(100);

        horizontalLayout->addWidget(spinBoxVolume);


        verticalLayout->addWidget(widget_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, -1, -1);
        widget_3 = new WidgetAudio(DialogSound);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_3->addWidget(widget_3);

        pushButton_2 = new QPushButton(DialogSound);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(100, 20));

        horizontalLayout_3->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_3);

        buttonBox = new QDialogButtonBox(DialogSound);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogSound);

        QMetaObject::connectSlotsByName(DialogSound);
    } // setupUi

    void retranslateUi(QDialog *DialogSound)
    {
        DialogSound->setWindowTitle(QApplication::translate("DialogSound", "Board Edit", nullptr));
        labelName->setText(QApplication::translate("DialogSound", "Name", nullptr));
        lineEditName->setPlaceholderText(QApplication::translate("DialogSound", "New Sound", nullptr));
        labelKeybind->setText(QApplication::translate("DialogSound", "Keybind", nullptr));
        labelFile->setText(QApplication::translate("DialogSound", "Audio File", nullptr));
        lineEditFile->setPlaceholderText(QApplication::translate("DialogSound", "Please choose a file", nullptr));
        pushButtonFile->setText(QApplication::translate("DialogSound", "Choose...", nullptr));
        label->setText(QApplication::translate("DialogSound", "Volume", nullptr));
        pushButton_2->setText(QApplication::translate("DialogSound", "Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogSound: public Ui_DialogSound {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSOUND_H
