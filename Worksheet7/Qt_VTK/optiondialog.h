/**
 * @file OptionDialog.h
 * @brief Dialog for editing option properties.
 *
 * Provides a dialog interface for editing properties such as name, color, and visibility
 * of a given object or model part. This class is part of the UI and allows users to
 * interactively modify the attributes of selected items.
 */

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
    class OptionDialog;
}
/**
 * @class OptionDialog
 * @brief Dialog class for editing item options.
 *
 * Allows editing of properties such as name, color, and visibility for items.
 */
class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget* parent = nullptr);
    ~OptionDialog();

    QString getName() const; ///< Retrieves the item's name.
    QColor getColor() const; ///< Retrieves the item's color.
    bool getVisibility() const; ///< Retrieves the item's visibility state.

    void setName(const QString& name); ///< Sets the item's name.
    void setColor(const QColor& color); ///< Sets the item's color.
    void setVisibility(bool isVisible); ///< Sets the item's visibility state.

private:
    Ui::OptionDialog* ui; ///< Pointer to the dialog's UI.
};

#endif // OPTIONDIALOG_H