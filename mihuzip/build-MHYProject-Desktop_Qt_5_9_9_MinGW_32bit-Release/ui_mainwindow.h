/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QProgressBar *progressBar;
    QPushButton *runBtn;
    QPushButton *aboutBtn;
    QPushButton *fileBtn;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(350, 386);
        MainWindow->setAnimated(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setEnabled(true);
        progressBar->setGeometry(QRect(25, 300, 300, 40));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\200\235\346\272\220\351\273\221\344\275\223 CN Heavy"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        progressBar->setFont(font);
        progressBar->setCursor(QCursor(Qt::ArrowCursor));
        progressBar->setValue(24);
        progressBar->setTextVisible(false);
        runBtn = new QPushButton(centralwidget);
        runBtn->setObjectName(QStringLiteral("runBtn"));
        runBtn->setGeometry(QRect(25, 200, 300, 40));
        runBtn->setMinimumSize(QSize(0, 30));
        runBtn->setFont(font);
        runBtn->setCursor(QCursor(Qt::PointingHandCursor));
        aboutBtn = new QPushButton(centralwidget);
        aboutBtn->setObjectName(QStringLiteral("aboutBtn"));
        aboutBtn->setGeometry(QRect(25, 250, 300, 40));
        aboutBtn->setMinimumSize(QSize(0, 30));
        aboutBtn->setFont(font);
        aboutBtn->setCursor(QCursor(Qt::PointingHandCursor));
        fileBtn = new QPushButton(centralwidget);
        fileBtn->setObjectName(QStringLiteral("fileBtn"));
        fileBtn->setGeometry(QRect(25, 20, 300, 161));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\200\235\346\272\220\351\273\221\344\275\223 CN Heavy"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setWeight(75);
        font1.setStyleStrategy(QFont::PreferDefault);
        fileBtn->setFont(font1);
        fileBtn->setCursor(QCursor(Qt::PointingHandCursor));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 350, 23));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        runBtn->setText(QApplication::translate("MainWindow", "\345\216\213\347\274\251", Q_NULLPTR));
        aboutBtn->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216", Q_NULLPTR));
        fileBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
