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
#include "newgroupdialog.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkNamedColors.h>
#include <QDebug>



 /**
  * @brief Constructor for the MainWindow class.
  * Initializes the user interface and connects signals to slots.
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

	//QString name = QString("Model");
	//QString visible("true");
	//QString colour("255,255,255");

	//// Create child item
	//ModelPart* childItem = new ModelPart({ name, visible, colour });

	//// Append to tree top-level
	//rootItem->appendChild(childItem);


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
	QAction* actionItemOptions = new QAction(tr("Item Options"), this);
	connect(actionItemOptions, &QAction::triggered, this, &MainWindow::on_actionItemOptions_triggered);

	// Add the action to the tree view's context menu
	ui->treeView->addAction(actionItemOptions);

	actionNewGroup = new QAction(tr("New Group"), this);
	ui->treeView->addAction(actionNewGroup); // Add the action to the tree view's context menu
	connect(actionNewGroup, &QAction::triggered, this, &MainWindow::on_actionNewGroup_triggered);


	// Set up the render window
	renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	ui->vtkWidget->setRenderWindow(renderWindow); // Name 'vtkWidget' may differ

	// Set up the renderer
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);

}

/**
 * @brief Handles clicks on the tree view, displaying the selected item's name.
 */
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
 * @brief Opens a dialog for editing properties of the selected item.
 */
void MainWindow::on_actionItemOptions_triggered() {
	QModelIndex index = ui->treeView->currentIndex();
	if (!index.isValid()) {
		return;
	}

	ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
	if (!selectedPart) {
		return;
	}

	OptionDialog dialog(this);
	dialog.setName(selectedPart->data(0).toString());
	dialog.setColor(QColor(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB()));
	dialog.setVisibility(selectedPart->visible());

	if (dialog.exec() == QDialog::Accepted) {
		QColor color = dialog.getColor();

		// Apply name, visibility, and color to the selected part
		applyPropertiesToPart(selectedPart, dialog.getName(), dialog.getVisibility(), color, true);

		// Recursively apply visibility and color to all children without changing their names
		updateChildrenProperties(selectedPart, dialog.getVisibility(), color);

		emit statusUpdateMessage("Item and its children updated.", 2000);
	}
}


void MainWindow::applyPropertiesToPart(ModelPart* part, const QString& name, bool visibility, const QColor& color, bool updateName) {
	if (!part) return;

	if (updateName) {
		part->set(0, QVariant(name)); // Update name only if updateName is true
	}
	part->set(1, QVariant(visibility ? "true" : "false"));
	part->set(2, QVariant(QString::number(color.red()) + "," + QString::number(color.green()) + "," + QString::number(color.blue())));

	part->setColour(color.red(), color.green(), color.blue());
	part->setVisible(visibility);



	QAbstractItemModel* model = ui->treeView->model();
	QModelIndex startIndex = model->index(part->row(), 0);
	QModelIndex endIndex = model->index(part->row(), model->columnCount());
	model->dataChanged(startIndex, endIndex);
	ui->treeView->update(startIndex);

	vtkSmartPointer<vtkActor> actor = part->getActor();
	if (actor) {
		actor->SetVisibility(visibility);
		actor->GetProperty()->SetDiffuseColor(color.red() / 255.0, color.green() / 255.0, color.blue() / 255.0);
		updateRender();
	}
}




void MainWindow::updateChildrenProperties(ModelPart* part, bool visibility, const QColor& color) {
	for (int i = 0; i < part->childCount(); ++i) {
		ModelPart* child = part->child(i);
		// Apply visibility and color without changing the name
		applyPropertiesToPart(child, QString(), visibility, color, false); // false to not update name
		updateChildrenProperties(child, visibility, color); // Recursive call
	}
}





/**
 * @brief Updates the rendering of the model based on the current tree structure.
 */
void MainWindow::updateRender() {
	renderer->RemoveAllViewProps(); // Remove existing actors

	// Get the count of top-level items in the tree
	int topLevelItemCount = partList->rowCount(QModelIndex());

	// Iterate through all top-level items
	for (int i = 0; i < topLevelItemCount; ++i) {
		QModelIndex topLevelIndex = partList->index(i, 0, QModelIndex());
		updateRenderFromTree(topLevelIndex); // Update render starting from each top-level item
	}

	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();
	renderer->Render(); // Render the scene
}

/**
 * @brief Recursively adds actors to the renderer starting from the provided tree index.
 * @param index The starting point in the model tree for adding actors to the render.
 */
void MainWindow::updateRenderFromTree(const QModelIndex& index) {
	vtkNew<vtkNamedColors> colors;
	if (index.isValid()) {
		ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
		if (selectedPart) {
			vtkActor* actor = selectedPart->getActor(); // Assuming getActor returns vtkActor*
			if (actor) {
				//actor->GetProperty()->SetDiffuseColor(
					//colors->GetColor3d("red").GetData());
				renderer->AddActor(actor);
			}
		}
	}

	// Recurse for children
	int rows = partList->rowCount(index);
	for (int i = 0; i < rows; ++i) {
		updateRenderFromTree(partList->index(i, 0, index));
	}
}

/**
 * @brief Slot triggered by the action to open and load an STL file.
 * It allows users to select an STL file to load, creating and appending a new ModelPart
 * based on the file, and updates the rendering accordingly.
 */
void MainWindow::on_actionOpen_File_triggered() {
	QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), QDir::homePath(), tr("STL Files (*.stl);;Text Files (*.txt)"));
	for (const QString& fileName : fileNames) {
		if (!fileName.isEmpty()) {
			QFileInfo fileInfo(fileName);
			QString justFileName = fileInfo.fileName();

			QList<QVariant> data;
			data << QVariant(justFileName) // Name
				<< QVariant("true") // Visibility
				<< QVariant("255,255,255"); // Color in R,G,B format

			auto* newPart = new ModelPart(data);
			QModelIndex currentIndex = ui->treeView->currentIndex();
			ModelPart* parentPart = currentIndex.isValid() ? static_cast<ModelPart*>(currentIndex.internalPointer()) : partList->getRootItem();
			parentPart->appendChild(newPart);
			newPart->loadSTL(fileName);

			// Setting the visibility and color
			QColor whiteColor(255, 255, 255);
			newPart->setColour(whiteColor.red(), whiteColor.green(), whiteColor.blue());
			newPart->setVisible(true);

			// Notifying the model about the data change
			QAbstractItemModel* model = ui->treeView->model();
			QModelIndex startIndex = model->index(newPart->row(), 0);
			QModelIndex endIndex = model->index(newPart->row(), model->columnCount() - 1);
			model->dataChanged(startIndex, endIndex);

			ui->treeView->model()->layoutChanged();
			updateRender();
			emit statusUpdateMessage(QString("Loaded STL file: %1").arg(fileName), 5000);
		}
	}
}





/**
 * @brief Destructor for the MainWindow class.
 * Cleans up allocated resources.
 */

MainWindow::~MainWindow()
{
	delete ui;
	delete partList; // Clean up partList

}

/**
 * @brief Handles the event when the first button is clicked.
 * It emits a signal to update the status message accordingly.
 */

void MainWindow::handleButton1() {
	// Emit signal with a message indicating the first button was clicked
	emit statusUpdateMessage("Button 1 was clicked.", 2000); // Display for 2000 ms
}

/**
 * @brief Handles the event when the second button is clicked.
 * Opens an option dialog for user input and updates the status message based on the dialog's outcome.
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

void MainWindow::on_actionNewGroup_triggered()
{
	newGroupDialog = new NewGroupDialog(this);

	QModelIndex index = ui->treeView->currentIndex();
	// Whether or not the index is valid, we show the dialog only once
	// So, we connect the dialog acceptance to a single logic flow that considers both cases
	connect(newGroupDialog, &NewGroupDialog::accepted, [this, index]() { // Capture index by value
		QString groupName = newGroupDialog->getGroupName(); // Obtain the group name from the dialog
		ModelPart* parentPart = index.isValid() ? static_cast<ModelPart*>(index.internalPointer()) : this->partList->getRootItem();
		ModelPart* newGroup = new ModelPart({ groupName, "true", "255,255,255" }); // Create a new group
		parentPart->appendChild(newGroup); // Add the new group under the selected part or root
		ui->treeView->model()->layoutChanged();
		});

	newGroupDialog->show(); // Show the dialog only once after setting up the connection
}

