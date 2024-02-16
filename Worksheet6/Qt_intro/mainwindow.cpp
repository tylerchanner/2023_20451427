#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief Constructs the main window and sets up UI components and connections.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect button signals to slots
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Connect the custom signal to the QStatusBar's showMessage slot
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1() {
    // Emit signal with a message indicating the first button was clicked
    emit statusUpdateMessage("Button 1 was clicked.", 2000); // Display for 2000 ms
}

void MainWindow::handleButton2() {
    // Emit signal with a message indicating the second button was clicked
    emit statusUpdateMessage("Button 2 was clicked.", 2000); // Display for 2000 ms
}
