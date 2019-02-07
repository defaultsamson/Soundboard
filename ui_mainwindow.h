/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSettings;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionGitHub;
    QAction *actionUpdate;
    QAction *actionWiki;
    QAction *actionExit;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *labelBoards;
    QSpacerItem *verticalSpacer;
    QListWidget *listBoards;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonAddBoard;
    QPushButton *buttonRemoveBoard;
    QPushButton *buttonEditBoard;
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QLabel *labelSounds;
    QSpacerItem *verticalSpacer_2;
    QListWidget *listSounds;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonAddSound;
    QPushButton *buttonRemoveSound;
    QPushButton *buttonEditSound;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(615, 305);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionOpen->setShortcutContext(Qt::WindowShortcut);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionGitHub = new QAction(MainWindow);
        actionGitHub->setObjectName(QString::fromUtf8("actionGitHub"));
        actionUpdate = new QAction(MainWindow);
        actionUpdate->setObjectName(QString::fromUtf8("actionUpdate"));
        actionWiki = new QAction(MainWindow);
        actionWiki->setObjectName(QString::fromUtf8("actionWiki"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(8, 4, 8, 8);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setHandleWidth(6);
        splitter->setChildrenCollapsible(false);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        verticalLayout_5 = new QVBoxLayout(widget_2);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        labelBoards = new QLabel(widget_2);
        labelBoards->setObjectName(QString::fromUtf8("labelBoards"));
        labelBoards->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(labelBoards);

        verticalSpacer = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer);

        listBoards = new QListWidget(widget_2);
        listBoards->setObjectName(QString::fromUtf8("listBoards"));

        verticalLayout_5->addWidget(listBoards);

        verticalSpacer_3 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_5->addItem(verticalSpacer_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        buttonAddBoard = new QPushButton(widget_2);
        buttonAddBoard->setObjectName(QString::fromUtf8("buttonAddBoard"));

        horizontalLayout->addWidget(buttonAddBoard);

        buttonRemoveBoard = new QPushButton(widget_2);
        buttonRemoveBoard->setObjectName(QString::fromUtf8("buttonRemoveBoard"));

        horizontalLayout->addWidget(buttonRemoveBoard);

        buttonEditBoard = new QPushButton(widget_2);
        buttonEditBoard->setObjectName(QString::fromUtf8("buttonEditBoard"));

        horizontalLayout->addWidget(buttonEditBoard);


        verticalLayout_5->addLayout(horizontalLayout);

        splitter->addWidget(widget_2);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        labelSounds = new QLabel(widget);
        labelSounds->setObjectName(QString::fromUtf8("labelSounds"));
        labelSounds->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(labelSounds);

        verticalSpacer_2 = new QSpacerItem(20, 2, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_2);

        listSounds = new QListWidget(widget);
        listSounds->setObjectName(QString::fromUtf8("listSounds"));

        verticalLayout_4->addWidget(listSounds);

        verticalSpacer_4 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_4->addItem(verticalSpacer_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        buttonAddSound = new QPushButton(widget);
        buttonAddSound->setObjectName(QString::fromUtf8("buttonAddSound"));

        horizontalLayout_2->addWidget(buttonAddSound);

        buttonRemoveSound = new QPushButton(widget);
        buttonRemoveSound->setObjectName(QString::fromUtf8("buttonRemoveSound"));

        horizontalLayout_2->addWidget(buttonRemoveSound);

        buttonEditSound = new QPushButton(widget);
        buttonEditSound->setObjectName(QString::fromUtf8("buttonEditSound"));

        horizontalLayout_2->addWidget(buttonEditSound);


        verticalLayout_4->addLayout(horizontalLayout_2);

        splitter->addWidget(widget);

        verticalLayout_2->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 615, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionSettings);
        menuHelp->addAction(actionWiki);
        menuHelp->addAction(actionGitHub);
        menuHelp->addSeparator();
        menuHelp->addAction(actionUpdate);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Soundboard", nullptr));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        actionNew->setText(QApplication::translate("MainWindow", "New", nullptr));
#ifndef QT_NO_SHORTCUT
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionSaveAs->setText(QApplication::translate("MainWindow", "Save As", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionGitHub->setText(QApplication::translate("MainWindow", "GitHub Page", nullptr));
        actionUpdate->setText(QApplication::translate("MainWindow", "Check For Updates", nullptr));
        actionWiki->setText(QApplication::translate("MainWindow", "Wiki", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_NO_SHORTCUT
        labelBoards->setText(QApplication::translate("MainWindow", "Boards", nullptr));
        buttonAddBoard->setText(QApplication::translate("MainWindow", "Add", nullptr));
        buttonRemoveBoard->setText(QApplication::translate("MainWindow", "Remove", nullptr));
        buttonEditBoard->setText(QApplication::translate("MainWindow", "Edit", nullptr));
        labelSounds->setText(QApplication::translate("MainWindow", "Sounds", nullptr));
        buttonAddSound->setText(QApplication::translate("MainWindow", "Add", nullptr));
        buttonRemoveSound->setText(QApplication::translate("MainWindow", "Remove", nullptr));
        buttonEditSound->setText(QApplication::translate("MainWindow", "Edit", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
