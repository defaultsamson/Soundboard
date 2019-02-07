/********************************************************************************
** Form generated from reading UI file 'dialogboard.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGBOARD_H
#define UI_DIALOGBOARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include "widgetkeybind.h"

QT_BEGIN_NAMESPACE

class Ui_DialogBoard
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelName;
    QLineEdit *lineEditName;
    QLabel *labelKeybind;
    WidgetKeybind *lineEdiKeybind;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogBoard)
    {
        if (DialogBoard->objectName().isEmpty())
            DialogBoard->setObjectName(QString::fromUtf8("DialogBoard"));
        DialogBoard->resize(260, 145);
        DialogBoard->setMaximumSize(QSize(16777215, 145));
        verticalLayout = new QVBoxLayout(DialogBoard);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        labelName = new QLabel(DialogBoard);
        labelName->setObjectName(QString::fromUtf8("labelName"));

        verticalLayout->addWidget(labelName);

        lineEditName = new QLineEdit(DialogBoard);
        lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
        lineEditName->setClearButtonEnabled(true);

        verticalLayout->addWidget(lineEditName);

        labelKeybind = new QLabel(DialogBoard);
        labelKeybind->setObjectName(QString::fromUtf8("labelKeybind"));

        verticalLayout->addWidget(labelKeybind);

        lineEdiKeybind = new WidgetKeybind(DialogBoard);
        lineEdiKeybind->setObjectName(QString::fromUtf8("lineEdiKeybind"));
        lineEdiKeybind->setCursor(QCursor(Qt::ArrowCursor));
        lineEdiKeybind->setFocusPolicy(Qt::ClickFocus);
        lineEdiKeybind->setReadOnly(true);
        lineEdiKeybind->setClearButtonEnabled(false);

        verticalLayout->addWidget(lineEdiKeybind);

        buttonBox = new QDialogButtonBox(DialogBoard);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogBoard);

        QMetaObject::connectSlotsByName(DialogBoard);
    } // setupUi

    void retranslateUi(QDialog *DialogBoard)
    {
        DialogBoard->setWindowTitle(QApplication::translate("DialogBoard", "Board Edit", nullptr));
        labelName->setText(QApplication::translate("DialogBoard", "Name", nullptr));
        lineEditName->setPlaceholderText(QApplication::translate("DialogBoard", "New Board", nullptr));
        labelKeybind->setText(QApplication::translate("DialogBoard", "Keybind", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogBoard: public Ui_DialogBoard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGBOARD_H
