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
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkNamedColors.h>


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


	// Set up the render window
	renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	ui->vtkWidget->setRenderWindow(renderWindow); // Name 'vtkWidget' may differ

	// Set up the renderer
	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow->AddRenderer(renderer);


	// Create the cylinder
	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	cylinder->SetResolution(100);

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

	vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
	cylinderActor->SetMapper(cylinderMapper);
	cylinderActor->GetProperty()->SetColor(1.0, 5.0, 0.35);
	cylinderActor->RotateX(30.0);
	cylinderActor->RotateY(-45.0);

	renderer->AddActor(cylinderActor);
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Azimuth(30);
	renderer->GetActiveCamera()->Elevation(30);
	renderer->ResetCameraClippingRange();

}
// Repeat for green and blue sliders


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
 * @brief Updates the rendering of the model based on the current tree structure.
 */
void MainWindow::updateRender() {
	renderer->RemoveAllViewProps(); // Remove existing actors
	updateRenderFromTree(partList->index(0, 0, QModelIndex())); // Start with the root
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
				actor->GetProperty()->SetDiffuseColor(
					colors->GetColor3d("red").GetData());
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
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("STL Files (*.stl);;Text Files (*.txt)"));
	if (!fileName.isEmpty()) {
		QFileInfo fileInfo(fileName);
		QString justFileName = fileInfo.fileName();

		// Prepare data for ModelPart construction
		QList<QVariant> data;
		data << QVariant(justFileName) << QVariant("true"); // Adjust based on actual ModelPart constructor

		// Create a new ModelPart instance with the prepared data
		auto* newPart = new ModelPart(data);
		QModelIndex currentIndex = ui->treeView->currentIndex();
		ModelPart* parentPart = currentIndex.isValid() ? static_cast<ModelPart*>(currentIndex.internalPointer()) : partList->getRootItem();
		parentPart->appendChild(newPart); // Simplify appendChild to directly add to parent
		newPart->loadSTL(fileName);

		ui->treeView->model()->layoutChanged(); // Properly signal the tree view to update
		updateRender(); // Update rendering from the root
		emit statusUpdateMessage(QString("Loaded STL file: %1").arg(fileName), 5000);
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