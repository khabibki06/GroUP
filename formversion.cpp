#include "formversion.h"
#include "ui_formversion.h"

FormVersion::FormVersion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVersion)
{
    ui->setupUi(this);
}

FormVersion::~FormVersion()
{
    delete ui;
}
