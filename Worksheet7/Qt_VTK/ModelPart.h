/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>
#include <QColor.h>

#include <vtkSmartPointer.h>
#include <vtkMapper.h>
#include <vtkActor.h>
#include <vtkSTLReader.h>
#include <vtkColor.h>

  /**
   * @class ModelPart
   * @brief Represents a part or component of a model.
   *
   * ModelPart instances are used to build a hierarchical tree structure representing components of a model.
   * Each part can have multiple child parts. This class also provides functionalities to load visual representations
   * from STL files and manage their appearance properties such as visibility and color.
   */
class ModelPart {
public:
	/**
	* @brief Constructor for creating a new ModelPart instance.
	* @param data A list of QVariant objects containing the initial data for this part.
	* @param parent A pointer to the parent ModelPart, if any (nullptr for root parts).
	*/
	ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

	/**
	* @brief Destructor for ModelPart.
	*/
	~ModelPart();

	/**
	 * @brief Appends a child part to this ModelPart.
	 * @param item The child ModelPart to be added.
	 */
	void appendChild(ModelPart* item);

	/** Return child at position 'row' below this item
	  * @param row is the row number (below this item)
	  * @return pointer to the item requested.
	  */
	ModelPart* child(int row);

	/** Return number of children to this item
	  * @return number of children
	  */
	int childCount() const;         /* Note on the 'const' keyword - it means that this function is valid for
									 * constant instances of this class. If a class is declared 'const' then it
									 * cannot be modifed, this means that 'set' type functions are usually not
									 * valid, but 'get' type functions are.
									 */

									 /** Get number of data items (2 - part name and visibility string) in this case.
									   * @return number of visible data columns
									   */
	int columnCount() const;

	/** Return the data item at a particular column for this item.
	  * i.e. either part name of visibility
	  * used by Qt when displaying tree
	  * @param column is column index
	  * @return the QVariant (represents string)
	  */
	QVariant data(int column) const;


	/** Default function required by Qt to allow setting of part
	  * properties within treeview.
	  * @param column is the index of the property to set
	  * @param value is the value to apply
	  */
	void set(int column, const QVariant& value);

	/** Get pointer to parent item
	  * @return pointer to parent item
	  */
	ModelPart* parentItem();

	/** Get row index of item, relative to parent item
	  * @return row index
	  */
	int row() const;


	/** Set colour
	  * (0-255 RGB values as ints)
	  */
	void setColour(const unsigned char R, const unsigned char G, const unsigned char B);


	/**
	 * @brief Gets the color components of this ModelPart.
	 * @return The red, green, and blue components of the color.
	 */
	unsigned char getColourR();
	unsigned char getColourG();
	unsigned char getColourB();

	/** Set visible flag
	  * @param isVisible sets visible/non-visible
	  */
	void setVisible(bool isVisible);

	/** Get visible flag
	  * @return visible flag as boolean
	  */
	bool visible();

	/**
	 * @brief Loads the geometrical data from an STL file for this part.
	 * @param fileName The path to the STL file to be loaded.
	 */
	void loadSTL(QString fileName);

	/** Return actor
	  * @return pointer to default actor for GUI rendering
	  */
	vtkSmartPointer<vtkActor> getActor();

	/** Return new actor for use in VR
	  * @return pointer to new actor
	  */
	vtkSmartPointer<vtkActor> getNewActor();

private:
	QList<ModelPart*> m_childItems; /**< Child parts of this model part. */
	QList<QVariant> m_itemData; /**< Data associated with this part, like name and visibility. */
	ModelPart* m_parentItem; /**< Parent part of this model part. */
	bool isVisible; /**< Visibility state of this part. */
	QColor color; /**< Color of this part. */
	vtkSmartPointer<vtkSTLReader> reader; /**< STL reader for loading geometrical data. */
	vtkSmartPointer<vtkMapper> mapper; /**< Mapper for geometrical data. */
	vtkSmartPointer<vtkActor> actor; /**< Actor for rendering. */
};


#endif