#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
    class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget* parent = nullptr);
    ~OptionDialog();

    QString getName() const;
    QColor getColor() const;
    bool getVisibility() const;

private:
    Ui::OptionDialog* ui;
};

#endif // OPTIONDIALOG_H
