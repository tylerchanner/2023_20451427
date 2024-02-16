/**
 * @file mainwindow.cpp
 * @brief Implementation file for the MainWindow class.
 *
 * This file provides the implementation of the MainWindow class, including
 * methods for setting up the UI, initializing the tree view model, and handling
 * UI events such as button clicks.
 */


#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optiondialog.h"

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
    
    // Connect tree view click signal to the slot
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    // Connect custom signal to status bar showMessage slot
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);

        }
    // Repeat for green and blue sliders



void MainWindow::handleTreeClicked() {
    // Get the index of the selected item
    QModelIndex index = ui->treeView->currentIndex();

    // Get a pointer to the item from the index
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Retrieve the name string from the internal QVariant data array
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage("The selected item is: " + text, 2000);
}


    /**
 * @brief Slot to handle the action triggered for opening a file.
 */
void MainWindow::on_actionOpen_File_triggered() {

    emit statusUpdateMessage(QString("Open File Action Triggered"), 0);

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QDir::homePath(), // Start directory
        tr("STL Files (*.stl);;Text Files (*.txt)")); // File filters

    // Check if fileName is not empty, meaning the user selected a file
    if (!fileName.isEmpty()) {
        emit statusUpdateMessage(QString("The selected file is: %1").arg(fileName), 5000);
    }
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
    OptionDialog dialog(this);

    // Pre-dialog setup if needed (e.g., connect signals to slots for updating labels)

    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        QColor color = dialog.getColor();
        bool isVisible = dialog.getVisibility();

        QString visibilityText = isVisible ? "Visible" : "Not Visible";
        QString message = QString("Name: %1, RGB: %2,%3,%4, Visibility: %5")
            .arg(name)
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
            .arg(visibilityText);

        emit statusUpdateMessage(message, 5000);
    }
    else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}
