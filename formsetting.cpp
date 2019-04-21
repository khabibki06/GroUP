#include "formsetting.h"
#include "ui_formsetting.h"

FormSetting::FormSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSetting)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);
}

FormSetting::~FormSetting()
{
    delete ui;
}


void FormSetting::on_VMDPath_toolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("select vmd executable..."),QDir::currentPath(),tr("All Files (*.*"));
    if (!filename.isEmpty())
        ui->VMDPath_lineEdit->setText(filename);
}

QString FormSetting::getVMDPath()
{
    return ui->VMDPath_lineEdit->text();
}
