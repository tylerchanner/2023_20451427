#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Assuming you have buttons named pushButton1, pushButton2,  in your UI
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1() {
    QMessageBox::information(this, "Button 1 Clicked", "You clicked button 1.");
}

void MainWindow::handleButton2() {
    QMessageBox::information(this, "Button 2 Clicked", "You clicked button 2.");
}

