/**
 * @file mainwindow.cpp
 * @brief Implementation file for the MainWindow class.
 *
 * This file provides the implementation of the MainWindow class, including
 * methods for setting up the UI, initializing the tree view model, and handling
 * UI events such as button clicks.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

 /**
  * @brief Constructs a MainWindow object.
  * @param parent The parent widget, nullptr if there's no parent.
  */

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    partList(nullptr) // Initialize partList to nullptr
{
    ui->setupUi(this);

    // Create/allocate the ModelList
    this->partList = new ModelPartList("Parts List");

    // Link it to the tree view in the GUI
    ui->treeView->setModel(this->partList);

    // Manually create a model tree
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        // Create child item
        ModelPart* childItem = new ModelPart({ name, visible });

        // Append to tree top-level
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({ name, visible });

            // Append to parent
            childItem->appendChild(childChildItem);
        }
    }

    // Connect button signals to slots
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Connect the custom signal to the QStatusBar's showMessage slot
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
}

/**
 * @brief Destroys the MainWindow object, including UI components and model list.
 */

MainWindow::~MainWindow()
{
    delete ui;
    delete partList; // Clean up partList
}

/**
 * @brief Slot to handle the first button's click event, updating the status bar.
 */

void MainWindow::handleButton1() {
    // Emit signal with a message indicating the first button was clicked
    emit statusUpdateMessage("Button 1 was clicked.", 2000); // Display for 2000 ms
}

/**
 * @brief Slot to handle the second button's click event, updating the status bar.
 */

void MainWindow::handleButton2() {
    // Emit signal with a message indicating the second button was clicked
    emit statusUpdateMessage("Button 2 was clicked.", 2000); // Display for 2000 ms
}
