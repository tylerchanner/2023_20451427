#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    connect(ui->horizontalScrollBarRed, &QScrollBar::valueChanged, this, [this](int value) {
        ui->label_2->setText(QString::number(value));
        });
    connect(ui->horizontalScrollBarGreen, &QScrollBar::valueChanged, this, [this](int value) {
        ui->label_6->setText(QString::number(value));
        });
    connect(ui->horizontalScrollBarBlue, &QScrollBar::valueChanged, this, [this](int value) {
        ui->label_7->setText(QString::number(value));
        });
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

QString OptionDialog::getName() const {
    return ui->plainTextEdit->toPlainText();
}

QColor OptionDialog::getColor() const {
    return QColor(ui->horizontalScrollBarRed->value(),
        ui->horizontalScrollBarGreen->value(),
        ui->horizontalScrollBarBlue->value());
}

bool OptionDialog::getVisibility() const {
    return ui->checkBox->isChecked();
}

void OptionDialog::setName(const QString& name) {
    ui->plainTextEdit->setPlainText(name);
}

void OptionDialog::setColor(const QColor& color) {
    ui->horizontalScrollBarRed->setValue(color.red());
    ui->horizontalScrollBarGreen->setValue(color.green());
    ui->horizontalScrollBarBlue->setValue(color.blue());
}

void OptionDialog::setVisibility(bool isVisible) {
    ui->checkBox->setChecked(isVisible);
}