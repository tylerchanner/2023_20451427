#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox> // Include QMessageBox for showing messages

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // Enable Qt's Meta-Object Compiler

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void handleButton1();
    void handleButton2();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
