#include "gromacsprep.h"
#include "ui_gromacsprep.h"

GromacsPrep::GromacsPrep(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GromacsPrep)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);
    createFFList();
    //setting output preparation to textviewer
    PreparationExec = new QProcess(this);
    PreparationExec->setProcessChannelMode(QProcess::MergedChannels);
    connect(PreparationExec, SIGNAL(readyRead()), this, SLOT(ReadPreparation()));
    connect(PreparationExec, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(StopPreparation(int,QProcess::ExitStatus)));
}

GromacsPrep::~GromacsPrep()
{
    delete ui;
}

void GromacsPrep::createFFList()
{
    QString ff_name = "C:/Program Files (x86)/Gromacs/share/gromacs/top";
    QDir ff_path(ff_name);
    QStringList ff = ff_path.entryList(QDir::Dirs);
    ff.removeAt(1);
    ff.removeAt(0);
    QStringList ff_new;
    for (QString i : ff)
    {
        int pos = i.lastIndexOf(".");
        i = i.left(pos);
        ff_new << i;
    }
    ui->FF_comboBox->addItems(ff_new);
    if (ui->FF_comboBox->findText("gromos54a7_atb") != -1)
    {
        ui->FF_comboBox->setCurrentText("gromos54a7_atb");
    }
}

void GromacsPrep::on_Coordinate_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open structure file ..."), QDir::currentPath(),tr("Coordinate Files (*.gro *.pdb)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->Coordinate_lineEdit->setText(fileNameRelative);
}

void GromacsPrep::on_WorkingDir_toolButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("open working directory ..."),QDir::currentPath());
    QDir::setCurrent(directory);
    ui->WorkingDir_lineEdit->setText(directory);
}

void GromacsPrep::stripWater(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    QString line;
    QStringList outputlist;
    while (!in.atEnd())
    {
        line = in.readLine();
        if (!line.contains("HOH"))
        {
            outputlist << line;
        }
    }
    file.close();
    //write temp out file without water
    QFile outfile("temp.pdb");
    if (outfile.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&outfile);
        for (int i = 0; i < outputlist.size();++i)
            out << outputlist.at(i) << "\n";
        outfile.close();
    }
}

bool GromacsPrep::checkEmptyParameter(QString text, QString label)
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

void GromacsPrep::on_RunPrepare_pushButton_clicked()
{
    if (checkEmptyParameter(ui->JobName_lineEdit->text(), "JOBNAME"))
        return;
    if (checkEmptyParameter(ui->Coordinate_lineEdit->text(), "Initial Structure"))
        return;
    QString topology_out = ui->JobName_lineEdit->text() + ".top";
    QString coordinate_out = ui->Coordinate_lineEdit->text() + ".gro";
    QStringList runcommand;

    if (ui->StripWater_checkBox->isChecked())
        stripWater(ui->Coordinate_lineEdit->text());

    runcommand << "pdb2gmx" <<  "-water" << "spce" << "-ff" << ui->FF_comboBox->currentText() << "-o" << coordinate_out << "-p" << topology_out;
    if (ui->StripWater_checkBox->isChecked())
        runcommand <<  "-f" << "temp.pdb";
    else
        runcommand <<  "-f" << ui->Coordinate_lineEdit->text();
    PreparationExec->start("gmx.exe", runcommand);
    PreparationExec->waitForFinished();
    if (ui->SolvateYES_radioButton->isChecked())
    {
        runcommand.clear();
        runcommand << "editconf" << "-f" << coordinate_out << "-o" << coordinate_out << "-c" << "-d" << ui->Distance_lineEdit->text() << "-bt" << "cubic";
        PreparationExec->start("gmx.exe", runcommand);
        PreparationExec->waitForFinished();
        runcommand.clear();
        runcommand << "solvate" << "-cp" << coordinate_out << "-cs" << "spc216.gro" << "-o" << coordinate_out << "-p" << topology_out;
        PreparationExec->start("gmx.exe", runcommand);
        PreparationExec->waitForFinished();
    }
    if (ui->BiomolIonsYES_radioButton_3->isChecked())
    {
        QString ionsmdp = ExecDir() + "/template/ions.mdp";
        runcommand.clear();
        runcommand << "grompp" << "-f" << ionsmdp << "-c" << coordinate_out <<  "-o" << "ions.tpr" <<  "-p" << topology_out << "-maxwarn" << "100";
        PreparationExec->start("gmx.exe", runcommand);
        PreparationExec->waitForFinished();
        runcommand.clear();
        runcommand <<"genion" << "-s" << "ions.tpr" << "-o"  << coordinate_out <<  "-p" <<  topology_out << "-pname" << ui->BiomolCation_lineEdit_2->text() << "-nname" << ui->BiomolAnions_lineEdit_2->text() ;
        if (ui->neutralize_checkBox_2->isChecked())
        {
            runcommand <<  "-neutral";
        } else
        {
            runcommand  <<"-conc" << ui->BiomolSaltConc_lineEdit_2->text();
        }
        PreparationExec->start("gmx.exe", runcommand);
        PreparationExec->write("SOL \n");
       }
}

void GromacsPrep::ReadPreparation()
{
    ui->Output_textEdit->append(PreparationExec->readAll());
}

void GromacsPrep::on_SolvateYES_radioButton_clicked()
{
    if (ui->SolvateYES_radioButton->isChecked())
        ui->Distance_lineEdit->setEnabled(1);
    else
        ui->Distance_lineEdit->setDisabled(1);
}

void GromacsPrep::on_SolvateNO_radioButton_2_clicked()
{
    if (ui->SolvateYES_radioButton->isChecked())
        ui->Distance_lineEdit->setEnabled(1);
    else
        ui->Distance_lineEdit->setDisabled(1);
}

void GromacsPrep::on_BiomolIonsYES_radioButton_3_clicked()
{
    if (ui->SolvateYES_radioButton->isChecked())
        ui->frame_addIons_2->setEnabled(1);
    else
        ui->frame_addIons_2->setDisabled(1);
}

void GromacsPrep::on_neutralize_checkBox_2_clicked()
{
    if (ui->neutralize_checkBox_2->isChecked())
        ui->BiomolSaltConc_lineEdit_2->setDisabled(1);
    else
        ui->BiomolSaltConc_lineEdit_2->setEnabled(1);
}
QString GromacsPrep::ExecDir()
{
    QString Dir = QCoreApplication::applicationDirPath();
    return Dir;
}

void GromacsPrep::StopPreparation(int exitCode, QProcess::ExitStatus exitStatus)
{
    ui->Output_textEdit->append("-------Preparation is finished--------");
}


//emit signal view structure when view button clicked
void GromacsPrep::on_VEW_pushButton_clicked()
{
    emit viewStructureSignal(ui->JobName_lineEdit->text() + ".gro");
}
