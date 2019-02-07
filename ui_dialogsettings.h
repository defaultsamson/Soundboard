/********************************************************************************
** Form generated from reading UI file 'dialogsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSETTINGS_H
#define UI_DIALOGSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgetaudio.h"
#include "widgetkeybind.h"

QT_BEGIN_NAMESPACE

class Ui_DialogSettings
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *checkBoxDarkTheme;
    QCheckBox *checkBoxFocusVisuals;
    QCheckBox *checkBoxTrayMinimize;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelEnableKeybinds;
    QLabel *labelDisableKeybinds;
    QLabel *labelStop;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_4;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_3;
    WidgetKeybind *lineEditEnableKeybinds;
    WidgetKeybind *lineEditDisableKeybinds;
    WidgetKeybind *lineEditStop;
    WidgetKeybind *lineEditPause;
    WidgetKeybind *lineEditResume;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelOutputDevice;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_5;
    QComboBox *comboBoxOutputDevice_2;
    QComboBox *comboBoxOutputDevice;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout;
    WidgetAudio *outputBar;
    QPushButton *pushButtonOutput;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *DialogSettings)
    {
        if (DialogSettings->objectName().isEmpty())
            DialogSettings->setObjectName(QString::fromUtf8("DialogSettings"));
        DialogSettings->resize(500, 536);
        verticalLayout = new QVBoxLayout(DialogSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        groupBox_3 = new QGroupBox(DialogSettings);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        checkBoxDarkTheme = new QCheckBox(groupBox_3);
        checkBoxDarkTheme->setObjectName(QString::fromUtf8("checkBoxDarkTheme"));

        verticalLayout_6->addWidget(checkBoxDarkTheme);

        checkBoxFocusVisuals = new QCheckBox(groupBox_3);
        checkBoxFocusVisuals->setObjectName(QString::fromUtf8("checkBoxFocusVisuals"));

        verticalLayout_6->addWidget(checkBoxFocusVisuals);

        checkBoxTrayMinimize = new QCheckBox(groupBox_3);
        checkBoxTrayMinimize->setObjectName(QString::fromUtf8("checkBoxTrayMinimize"));

        verticalLayout_6->addWidget(checkBoxTrayMinimize);


        verticalLayout->addWidget(groupBox_3);

        groupBox = new QGroupBox(DialogSettings);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        widget_3 = new QWidget(groupBox);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        labelEnableKeybinds = new QLabel(widget_3);
        labelEnableKeybinds->setObjectName(QString::fromUtf8("labelEnableKeybinds"));

        verticalLayout_2->addWidget(labelEnableKeybinds);

        labelDisableKeybinds = new QLabel(widget_3);
        labelDisableKeybinds->setObjectName(QString::fromUtf8("labelDisableKeybinds"));

        verticalLayout_2->addWidget(labelDisableKeybinds);

        labelStop = new QLabel(widget_3);
        labelStop->setObjectName(QString::fromUtf8("labelStop"));

        verticalLayout_2->addWidget(labelStop);

        label = new QLabel(widget_3);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(widget_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_2->addWidget(label_2);


        horizontalLayout_4->addWidget(widget_3);

        horizontalSpacer_4 = new QSpacerItem(2, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_3 = new QVBoxLayout(widget_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        lineEditEnableKeybinds = new WidgetKeybind(widget_2);
        lineEditEnableKeybinds->setObjectName(QString::fromUtf8("lineEditEnableKeybinds"));
        lineEditEnableKeybinds->setCursor(QCursor(Qt::ArrowCursor));
        lineEditEnableKeybinds->setFocusPolicy(Qt::ClickFocus);

        verticalLayout_3->addWidget(lineEditEnableKeybinds);

        lineEditDisableKeybinds = new WidgetKeybind(widget_2);
        lineEditDisableKeybinds->setObjectName(QString::fromUtf8("lineEditDisableKeybinds"));
        lineEditDisableKeybinds->setCursor(QCursor(Qt::ArrowCursor));
        lineEditDisableKeybinds->setFocusPolicy(Qt::ClickFocus);

        verticalLayout_3->addWidget(lineEditDisableKeybinds);

        lineEditStop = new WidgetKeybind(widget_2);
        lineEditStop->setObjectName(QString::fromUtf8("lineEditStop"));
        lineEditStop->setCursor(QCursor(Qt::ArrowCursor));
        lineEditStop->setFocusPolicy(Qt::ClickFocus);

        verticalLayout_3->addWidget(lineEditStop);

        lineEditPause = new WidgetKeybind(widget_2);
        lineEditPause->setObjectName(QString::fromUtf8("lineEditPause"));
        lineEditPause->setCursor(QCursor(Qt::ArrowCursor));
        lineEditPause->setFocusPolicy(Qt::ClickFocus);

        verticalLayout_3->addWidget(lineEditPause);

        lineEditResume = new WidgetKeybind(widget_2);
        lineEditResume->setObjectName(QString::fromUtf8("lineEditResume"));
        lineEditResume->setCursor(QCursor(Qt::ArrowCursor));
        lineEditResume->setFocusPolicy(Qt::ClickFocus);

        verticalLayout_3->addWidget(lineEditResume);


        horizontalLayout_4->addWidget(widget_2);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(DialogSettings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        labelOutputDevice = new QLabel(groupBox_2);
        labelOutputDevice->setObjectName(QString::fromUtf8("labelOutputDevice"));

        verticalLayout_4->addWidget(labelOutputDevice);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        comboBoxOutputDevice_2 = new QComboBox(groupBox_2);
        comboBoxOutputDevice_2->setObjectName(QString::fromUtf8("comboBoxOutputDevice_2"));

        verticalLayout_5->addWidget(comboBoxOutputDevice_2);

        comboBoxOutputDevice = new QComboBox(groupBox_2);
        comboBoxOutputDevice->setObjectName(QString::fromUtf8("comboBoxOutputDevice"));

        verticalLayout_5->addWidget(comboBoxOutputDevice);


        horizontalLayout_5->addLayout(verticalLayout_5);

        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(100, 20));

        horizontalLayout_5->addWidget(pushButton_2);


        verticalLayout_4->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer);

        widget_4 = new QWidget(groupBox_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        horizontalLayout = new QHBoxLayout(widget_4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 0, 6, 0);
        outputBar = new WidgetAudio(widget_4);
        outputBar->setObjectName(QString::fromUtf8("outputBar"));
        outputBar->setMaximumSize(QSize(16777215, 20));

        horizontalLayout->addWidget(outputBar);

        pushButtonOutput = new QPushButton(widget_4);
        pushButtonOutput->setObjectName(QString::fromUtf8("pushButtonOutput"));
        pushButtonOutput->setMaximumSize(QSize(100, 20));

        horizontalLayout->addWidget(pushButtonOutput);


        verticalLayout_4->addWidget(widget_4);


        verticalLayout->addWidget(groupBox_2);

        widget = new QWidget(DialogSettings);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMaximumSize(QSize(16777215, 30));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        horizontalLayout_2->addWidget(buttonBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(widget);


        retranslateUi(DialogSettings);

        QMetaObject::connectSlotsByName(DialogSettings);
    } // setupUi

    void retranslateUi(QDialog *DialogSettings)
    {
        DialogSettings->setWindowTitle(QApplication::translate("DialogSettings", "Settings", nullptr));
        groupBox_3->setTitle(QApplication::translate("DialogSettings", "General", nullptr));
        checkBoxDarkTheme->setText(QApplication::translate("DialogSettings", "Dark Theme", nullptr));
        checkBoxFocusVisuals->setText(QApplication::translate("DialogSettings", "Update visuals when window not focussed", nullptr));
        checkBoxTrayMinimize->setText(QApplication::translate("DialogSettings", "Minimize to tray (TODO)", nullptr));
        groupBox->setTitle(QApplication::translate("DialogSettings", "Keybinds", nullptr));
        labelEnableKeybinds->setText(QApplication::translate("DialogSettings", "Enable All Keybinds", nullptr));
        labelDisableKeybinds->setText(QApplication::translate("DialogSettings", "Disable All Keybinds", nullptr));
        labelStop->setText(QApplication::translate("DialogSettings", "Stop All Sounds", nullptr));
        label->setText(QApplication::translate("DialogSettings", "Pause All Sounds", nullptr));
        label_2->setText(QApplication::translate("DialogSettings", "Resume All Sounds", nullptr));
        groupBox_2->setTitle(QApplication::translate("DialogSettings", "Audio", nullptr));
        labelOutputDevice->setText(QApplication::translate("DialogSettings", "Output Devices", nullptr));
        pushButton_2->setText(QApplication::translate("DialogSettings", "Refresh", nullptr));
        pushButtonOutput->setText(QApplication::translate("DialogSettings", "Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogSettings: public Ui_DialogSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSETTINGS_H
