/**
 * @file mainwindow.h
 * @brief Header file for the MainWindow class in a Qt application.
 *
 * This file declares the MainWindow class, which is responsible for initializing
 * the application's main window and managing UI interactions. It includes setup
 * for a model-based tree view and handling button clicks to display messages in
 * the status bar.
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h" 
#include "ModelPart.h" 
#include "newgroupdialog.h"
#include <QString>
#include "optiondialog.h" 
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Ui {
	class MainWindow;
}
/**
 * @class MainWindow
 * @brief The MainWindow class manages the main window and its UI components.
 *
 * The MainWindow class setups the UI, initializes a model for a tree view,
 * and handles user interactions such as button clicks.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	void updateRender();
	void updateRenderFromTree(const QModelIndex& index);

	/**
	* @brief Constructs a MainWindow object.
	* @param parent The parent widget, nullptr if there's no parent.
	*/
	explicit MainWindow(QWidget* parent = nullptr);


	/**
	 * @brief Destroys the MainWindow object.
	 */
	~MainWindow();

signals:
	/**
	 * @brief Signal to update the status bar message.
	 * @param message The message to display.
	 * @param timeout Duration in milliseconds to display the message.
	 */
	void statusUpdateMessage(const QString& message, int timeout);

public slots:
	/**
	 * @brief Handles the first button click.
	 */
	void handleButton1();

	/**
	 * @brief Handles the second button click.
	 */
	void handleButton2();

	void handleTreeClicked();

	void on_actionOpen_File_triggered();

	void on_actionItemOptions_triggered();

	void on_actionNewGroup_triggered();



private:
	Ui::MainWindow* ui;
	ModelPartList* partList; ///< Pointer to the model parts list
	vtkSmartPointer<vtkRenderer> renderer; ///< Renderer for the VTK view
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow; ///< Window for rendering
	QAction* actionNewGroup; // Action to create a new group
	NewGroupDialog* newGroupDialog; // Pointer to the NewGroupDialog
	QAction* actionDeleteGroup;

};

#endif // MAINWINDOW_H