/**
 * @file optiondialog.cpp
 * @brief Implementation of the OptionDialog class.
 *
 * Implements the functionality of the OptionDialog class, which provides a user interface
 * for editing the properties of an item, such as its name, color, and visibility.
 */

#include "optiondialog.h"
#include "ui_optiondialog.h"

 /**
  * @brief Constructs an OptionDialog object.
  * @param parent The parent widget of this dialog, nullptr if there's no parent.
  */
OptionDialog::OptionDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    // Set the checkbox to be checked by default
    ui->checkBox->setChecked(true);

    // Update labels and line edits when scroll bars change
    connect(ui->horizontalScrollBarRed, &QScrollBar::valueChanged, this, [this](int value) {
        ui->lineEdit->setText(QString::number(value)); // Update for red
        });
    connect(ui->horizontalScrollBarGreen, &QScrollBar::valueChanged, this, [this](int value) {
        ui->lineEdit_2->setText(QString::number(value)); // Update for green
        });
    connect(ui->horizontalScrollBarBlue, &QScrollBar::valueChanged, this, [this](int value) {
        ui->lineEdit_3->setText(QString::number(value)); // Update for blue
        });

    // Update scroll bars when line edits change
    connect(ui->lineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok) ui->horizontalScrollBarRed->setValue(value);
        });
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, [this](const QString& text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok) ui->horizontalScrollBarGreen->setValue(value);
        });
    connect(ui->lineEdit_3, &QLineEdit::textChanged, this, [this](const QString& text) {
        bool ok;
        int value = text.toInt(&ok);
        if (ok) ui->horizontalScrollBarBlue->setValue(value);
        });
}



/**
 * @brief Destroys the OptionDialog object.
 */
OptionDialog::~OptionDialog()
{
    delete ui;
}

/**
 * @brief Retrieves the name property of the item.
 * @return The name as a QString.
 */
QString OptionDialog::getName() const {
    return ui->plainTextEdit->toPlainText();
}

/**
 * @brief Retrieves the color property of the item.
 * @return The color as a QColor object.
 */
QColor OptionDialog::getColor() const {
    return QColor(ui->horizontalScrollBarRed->value(),
        ui->horizontalScrollBarGreen->value(),
        ui->horizontalScrollBarBlue->value());
}

/**
 * @brief Retrieves the visibility state of the item.
 * @return True if visible, false otherwise.
 */
bool OptionDialog::getVisibility() const {
    return ui->checkBox->isChecked();
}

/**
 * @brief Sets the name property of the item.
 * @param name The new name of the item.
 */
void OptionDialog::setName(const QString& name) {
    ui->plainTextEdit->setPlainText(name);
}

/**
 * @brief Sets the color property of the item.
 * @param color The new color of the item.
 */
void OptionDialog::setColor(const QColor& color) {
    ui->horizontalScrollBarRed->setValue(color.red());
    ui->horizontalScrollBarGreen->setValue(color.green());
    ui->horizontalScrollBarBlue->setValue(color.blue());
}

/**
 * @brief Sets the visibility state of the item.
 * @param isVisible True to make the item visible, false to hide it.
 */
void OptionDialog::setVisibility(bool isVisible) {
    ui->checkBox->setChecked(isVisible);
}