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
#include "ModelPartList.h" // Include the header for your ModelPartList
#include "ModelPart.h" // Include this if you'll refer to ModelPart directly in MainWindow
#include <QString>
#include "optiondialog.h" // Adjust the include to match your dialog's actual header file name


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


private:
    Ui::MainWindow* ui;
    ModelPartList* partList; // Pointer to your ModelPartList
};

#endif // MAINWINDOW_H