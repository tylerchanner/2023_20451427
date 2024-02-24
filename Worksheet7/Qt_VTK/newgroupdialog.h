#ifndef NEWGROUPDIALOG_H
#define NEWGROUPDIALOG_H

#include <QDialog>

namespace Ui {
    class NewGroupDialog;
}

class NewGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGroupDialog(QWidget* parent = nullptr);
    ~NewGroupDialog();

    QString getGroupName() const; // Add this line

private:
    Ui::NewGroupDialog* ui;
};

#endif // NEWGROUPDIALOG_H
