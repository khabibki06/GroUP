#include "gromacsanalysis.h"
#include "ui_gromacsanalysis.h"

GromacsAnalysis::GromacsAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GromacsAnalysis)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);

    //setting analysis and output  to textviewer
    ProcessAnalysis = new QProcess(this);
    ProcessAnalysis->setProcessChannelMode(QProcess::MergedChannels);
    connect(ProcessAnalysis, SIGNAL(readyRead()), this, SLOT(ReadAnalysis()));
    connect(ProcessAnalysis, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(StopAnalysis(int,QProcess::ExitStatus)));
}

GromacsAnalysis::~GromacsAnalysis()
{
    delete ui;
}

void GromacsAnalysis::on_workDir_toolButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("open working directory ..."),QDir::currentPath());
    QDir::setCurrent(directory);
    ui->workDir_lineEdit->setText(directory);
}

QString GromacsAnalysis::selectComponentEnergy()
{
    if (ui->epot_radioButton->isChecked())
        return "Potential";
    else if (ui->temperature_radioButton->isChecked())
        return "Temperature";
    else if (ui->density_radioButton->isChecked())
        return "Density";
    else if (ui->pressure_radioButton->isChecked())
        return "Pressure";
    else
        return " ";
}

void GromacsAnalysis::on_runEnergy_pushButton_clicked()
{
    if (checkEmptyParameter(ui->energyfile_lineEdit->text(), "Energy File (*.edr"))
        return;
    if (checkEmptyParameter(ui->energyout_lineEdit->text(), "Output File (*.xvg"))
        return;
    QStringList command;
    command << "energy" << "-f" << ui->energyfile_lineEdit->text() << "-o" << ui->energyout_lineEdit->text();
    ProcessAnalysis->start("gmx.exe", command);
    if (ui->temperature_radioButton->isChecked())
        ProcessAnalysis->write("Temperature \n 0 \n");
    if (ui->density_radioButton->isChecked())
        ProcessAnalysis->write("Density \n 0 \n");
    if (ui->epot_radioButton->isChecked())
        ProcessAnalysis->write("Potential \n 0 \n");
    if (ui->pressure_radioButton->isChecked())
        ProcessAnalysis->write("Pressure \n 0 \n");
}

void GromacsAnalysis::StopAnalysis(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->outputText_textEdit->append("-------Analyis is finished--------");
}

void GromacsAnalysis::ReadAnalysis()
{
    ui->outputText_textEdit->append(ProcessAnalysis->readAll());
}

bool GromacsAnalysis::checkEmptyParameter(QString text, QString label)
{
    if (text.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(label + " is empty. Please fill it before run ");
        msgBox.exec();
        return 1;
    } else
        return 0;
}


void GromacsAnalysis::on_energyfile_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open energy  file ..."), QDir::currentPath(),tr("Energy Files (*.edr)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->energyfile_lineEdit->setText(fileNameRelative);
}

//emit signal view structure when view button clicked
void GromacsAnalysis::on_viewGraphEn_pushButton_clicked()
{
    if (checkEmptyParameter(ui->energyout_lineEdit->text(), "Output File (*.xvg"))
        return;
    QString outenergy;
    if (!ui->energyout_lineEdit->text().endsWith("xvg"))
        outenergy =  ui->energyout_lineEdit->text() + ".xvg";
    else
        outenergy = ui->energyout_lineEdit->text();
    emit viewGraphSignal(outenergy);
}
