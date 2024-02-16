#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class manages the main window and its UI elements.
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

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
