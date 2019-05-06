#include "formsetting.h"
#include "ui_formsetting.h"

FormSetting::FormSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSetting)
{
    ui->setupUi(this);
    loadSettings();
}

FormSetting::~FormSetting()
{
    delete ui;
}


void FormSetting::on_VMDPath_toolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("select vmd executable..."),QDir::currentPath(),tr("vmd.exe (vmd.exe)"));
    if (!filename.isEmpty())
        ui->VMDPath_lineEdit->setText(filename);
}

QString FormSetting::getVMDPath()
{
    return ui->VMDPath_lineEdit->text();
}

QString FormSetting::getGMXExec()
{
    return ui->GMXPath_lineEdit->text();
}
QString FormSetting::getGMXLib()
{
    return ui->GMXLIB_lineEdit->text();
}

void FormSetting::on_GMXPath_toolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("select gromacs executable..."),QDir::currentPath(),tr("gmx.exe (gmx.exe)"));
    if (!filename.isEmpty())
        ui->GMXPath_lineEdit->setText(filename);
}

void FormSetting::on_saveSetting_pushButton_clicked()
{
    saveSettings();
}

void FormSetting::saveSettings()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/group.ini", QSettings::IniFormat);
    settings.setValue("VMDPATH", ui->VMDPath_lineEdit->text());
    settings.setValue("GMXPATH", ui->GMXPath_lineEdit->text());
    settings.setValue("GMXLIB", ui->GMXLIB_lineEdit->text());
    settings.setValue("packmolPATH", ui->packmolPath_lineEdit->text());
}

void FormSetting::loadSettings()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::DataLocation)+"/group.ini", QSettings::IniFormat);
    ui->GMXPath_lineEdit->setText(settings.value("GMXPATH","C:/Program Files (x86)/Gromacs/bin/gmx.exe").toString());
    ui->GMXLIB_lineEdit->setText(settings.value("GMXLIB","C:/Program Files (x86)/Gromacs/share/gromacs/top").toString());
    ui->VMDPath_lineEdit->setText(settings.value("VMDPATH","C:/Program Files (x86)/University of Illinois/VMD/vmd.exe").toString());
    ui->packmolPath_lineEdit->setText(settings.value("packmolPATH",QCoreApplication::applicationDirPath()+"/packmol.exe").toString());
}

void FormSetting::on_Load_pushButton_clicked()
{
   loadSettings();
}

void FormSetting::on_GMXLIB_toolButton_clicked()
{
    QString dirname = QFileDialog::getExistingDirectory(this, tr("select gromacs topology"), ui->GMXPath_lineEdit->text());
    if (!dirname.isEmpty())
        ui->GMXLIB_lineEdit->setText(dirname);
}

void FormSetting::on_GMXPath_lineEdit_textChanged(const QString &arg1)
{
    QDir GromacsBin(ui->GMXPath_lineEdit->text());
    QString GromacsLibrary = GromacsBin.cleanPath(GromacsBin.absoluteFilePath("../share/gromacs/top"));
    if (ui->GMXLIB_lineEdit->text().isEmpty())
        ui->GMXLIB_lineEdit->setText(GromacsLibrary);
}

void FormSetting::on_packmolPath_toolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("select packmol executable..."),QDir::currentPath(),tr("packmol.exe (packmol.exe)"));
    if (!filename.isEmpty())
        ui->packmolPath_lineEdit->setText(filename);
}

QString FormSetting::getPackmol()
{
    return ui->packmolPath_lineEdit->text();
}
