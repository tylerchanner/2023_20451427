/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>



  /* Commented out for now, will be uncommented later when you have
   * installed the VTK library
   */
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>


   /**
	* @brief Constructor for ModelPart.
	* @param data Contains the initial data for the part, such as name and visibility.
	* @param parent The parent ModelPart, if any; nullptr for the root part.
	*/

ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent)
	: m_itemData(data), m_parentItem(parent), isVisible(false) {

	/* You probably want to give the item a default colour */
}

/**
 * @brief Destructor for ModelPart.
 */
ModelPart::~ModelPart() {
	qDeleteAll(m_childItems);
}

/**
 * @brief Appends a child part to this part.
 * @param item The child ModelPart to be added.
 */
void ModelPart::appendChild(ModelPart* item) {
	item->m_parentItem = this;
	m_childItems.append(item);
}

/**
 * @brief Retrieves a child part by its row.
 * @param row The row number of the child part to retrieve.
 * @return Pointer to the requested child ModelPart, or nullptr if invalid row.
 */
ModelPart* ModelPart::child(int row) {
	/* Return pointer to child item in row below this item.
	 */
	if (row < 0 || row >= m_childItems.size())
		return nullptr;
	return m_childItems.at(row);
}

/**
 * @brief Counts the number of child parts.
 * @return The number of child parts.
 */
int ModelPart::childCount() const {
	/* Count number of child items
	 */
	return m_childItems.count();
}

/**
 * @brief Counts the number of columns (properties) of this part.
 * @return The number of columns (properties).
 */
int ModelPart::columnCount() const {
	/* Count number of columns (properties) that this item has.
	 */
	return m_itemData.count();
}

/**
 * @brief Retrieves data for a specific column (property).
 * @param column The column number to retrieve data for.
 * @return The data stored in the specified column, as a QVariant.
 */
QVariant ModelPart::data(int column) const {
	if (column < 0 || column >= m_itemData.size())
		return QVariant();
	return m_itemData[column];
}


/**
 * @brief Sets the data for a specific column (property).
 * @param column The column number to set data for.
 * @param value The value to set for the specified column.
 */
void ModelPart::set(int column, const QVariant& value) {
	if (column >= 0 && column < m_itemData.size()) {
		m_itemData[column] = value;
	}
}


/**
 * @brief Gets the parent part of this part.
 * @return Pointer to the parent ModelPart, or nullptr if this is a root part.
 */
ModelPart* ModelPart::parentItem() {
	return m_parentItem;
}

/**
 * @brief Finds the row number of this part relative to its parent.
 * @return The row number of this part.
 */
int ModelPart::row() const {
	/* Return the row index of this item, relative to it's parent.
	 */
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
	return 0;
}

/**
 * @brief Sets the color of this part.
 * @param R Red component of the color.
 * @param G Green component of the color.
 * @param B Blue component of the color.
 */
void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
	color = QColor(R, G, B);
}

unsigned char ModelPart::getColourR() {
	return static_cast<unsigned char>(color.red());
}

unsigned char ModelPart::getColourG() {
	return static_cast<unsigned char>(color.green());
}


unsigned char ModelPart::getColourB() {
	return static_cast<unsigned char>(color.blue());
}


void ModelPart::setVisible(bool isVisible) {
	this->isVisible = isVisible;
}

bool ModelPart::visible() { return isVisible; }

/**
 * @brief Loads an STL file and initializes the visualization properties.
 * @param fileName The path to the STL file to load.
 */
void ModelPart::loadSTL(QString fileName) {
	vtkNew<vtkSTLReader> reader;
	reader->SetFileName(fileName.toStdString().c_str());
	reader->Update();

	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(reader->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	// Set visual properties as needed
	this->actor = actor; // Save the actor to the ModelPart instance
}

/**
 * @brief Retrieves the actor for rendering this part.
 * @return A smart pointer to the vtkActor for this part.
 */
vtkSmartPointer<vtkActor> ModelPart::getActor() {
	return this->actor;
}


/**
 * @brief Creates and returns a new actor based on this part's properties.
 * @return A new vtkActor with properties duplicated from the original actor.
 */
vtkSmartPointer<vtkActor> ModelPart::getNewActor() {

	if (!this->actor || !this->reader) {
		return nullptr; // Return a null smart pointer if preconditions are not met
	}

	// 1. Create a new mapper, identical to the original for consistency
	vtkSmartPointer<vtkPolyDataMapper> newMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	newMapper->SetInputConnection(this->reader->GetOutputPort());

	// 2. Create a new actor and link to the new mapper
	vtkSmartPointer<vtkActor> newActor = vtkSmartPointer<vtkActor>::New();
	newActor->SetMapper(newMapper);

	// 3. Copy the vtkProperties of the original actor to the new actor
	if (this->actor->GetProperty()) {
		newActor->GetProperty()->DeepCopy(this->actor->GetProperty());
	}

	// Return the smart pointer to the new actor, ensuring memory management is handled by vtkSmartPointer
	return newActor;
}
