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

    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);



    // Manually create a model tree
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top level items
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        QString colour("255,255,255");

        // Create child item
        ModelPart* childItem = new ModelPart({ name, visible, colour });

        // Append to tree top-level
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            QString colour("255,255,255");

            ModelPart* childChildItem = new ModelPart({ name, visible, colour });

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

    // Create an action for the context menu
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

    // Create an action for the context menu
    QAction* actionItemOptions = new QAction(tr("Item Options"), this);
    connect(actionItemOptions, &QAction::triggered, this, &MainWindow::on_actionItemOptions_triggered);

    // Add the action to the tree view's context menu
    ui->treeView->addAction(actionItemOptions);

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

void MainWindow::on_actionItemOptions_triggered() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
        return;

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart)
        return;

    // Open the dialog to edit the properties
    OptionDialog dialog(this);
    // Set the existing values to the dialog controls
    dialog.setName(selectedPart->data(0).toString());
    dialog.setColor(QColor(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB()));
    dialog.setVisibility(selectedPart->visible());

    if (dialog.exec() == QDialog::Accepted) {
        // Apply the changes
        QColor color = dialog.getColor();
        QString colorString = QString::number(color.red()) + "," + QString::number(color.green()) + "," + QString::number(color.blue());

        selectedPart->set(0, QVariant(dialog.getName())); // Set the name
        selectedPart->set(1, QVariant(dialog.getVisibility() ? "true" : "false")); // Set the visibility
        selectedPart->set(2, QVariant(colorString)); // Set the color string

        
        selectedPart->setColour(color.red(), color.green(), color.blue());
        selectedPart->setVisible(dialog.getVisibility());



        // Emit a message or update the view as needed
        emit statusUpdateMessage("Item updated.", 2000);
    }
}


/**
* @brief Slot to handle the action triggered for opening a file.
*/
void MainWindow::on_actionOpen_File_triggered() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QDir::homePath(), // Start directory
        tr("STL Files (*.stl);;Text Files (*.txt)")); // File filters

    // Check if fileName is not empty, meaning the user selected a file
    if (!fileName.isEmpty()) {
        // Extract just the filename without the path
        QFileInfo fileInfo(fileName);
        QString justFileName = fileInfo.fileName();

        // Get the current index of the selected item in the tree view
        QModelIndex currentIndex = ui->treeView->currentIndex();

        // Ensure a valid item is selected
        if (currentIndex.isValid()) {
            // Retrieve the selected part using the current index
            ModelPart* selectedPart = static_cast<ModelPart*>(currentIndex.internalPointer());

            if (selectedPart) {
                // Update the name of the selected part
                selectedPart->set(0, QVariant(justFileName));

                // If your model requires you to manually trigger an update (for example, to refresh the view),
                // you may need to emit a signal or call a method on your model to inform it of the change.
                // This depends on your specific model implementation.
                // Example:
                // emit dataChanged(currentIndex, currentIndex);

                // Inform the user about the update
                emit statusUpdateMessage(QString("The selected file is: %1").arg(fileName), 5000);
            }
        }
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