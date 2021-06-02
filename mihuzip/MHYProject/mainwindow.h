#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QtGui>
#include <iostream>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void enableRunBtn();
    void disableRunBtn();

    ~MainWindow();

private slots:
    void aboutBtnClicked();
    void chooseFile();
    void runZip();

private:
    Ui::MainWindow *ui;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void HuffmanZip();
    void HuffmanUnZip();

    string curFileName;
    bool readyToRun;
    int fileCompact;
};
#endif // MAINWINDOW_H
