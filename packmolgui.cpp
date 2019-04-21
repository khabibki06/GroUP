#include "packmolgui.h"
#include "ui_packmolgui.h"

packmolgui::packmolgui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::packmolgui)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);
    //Process
    packmolprocess = new QProcess(this);
    packmolprocess->setProcessChannelMode(QProcess::MergedChannels);
    //read output packmol
    connect(packmolprocess,SIGNAL(readyRead()),this,SLOT(ReadPackmolProcess()));
}

packmolgui::~packmolgui()
{
    delete ui;
}

void packmolgui::on_workdir_toolButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("open working directory ..."),QDir::currentPath());
    QDir::setCurrent(directory);
    ui->workdir_lineEdit->setText(directory);
}

void packmolgui::on_structure_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open coordinate file ..."), QDir::currentPath(),tr("Coordinate Files (*.pdb)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->structure_lineEdit->setText(fileNameRelative);
}

void packmolgui::on_add_pushButton_clicked()
{
    if (checkEmptyParameter(ui->jobname_lineEdit->text(), "JOBNAME"))
    {
        ui->jobname_lineEdit->setFocus();
        return;
    }
    if (checkEmptyParameter(ui->structure_lineEdit->text(), "STRUCTURE"))
    {
        ui->structure_toolButton->setFocus();
        return;
    }
    if (checkEmptyParameter(ui->numbermol_lineEdit->text(), "number of molecules "))
    {
        ui->numbermol_lineEdit->setFocus();
        return;
    }
    if (checkEmptyParameter(ui->x_lineEdit_2->text(), "final coordinate x "))
    {
        ui->x_lineEdit_2->setFocus();
        return;
    }
    if (checkEmptyParameter(ui->y_lineEdit_2->text(), "final coordinate y "))
    {
        ui->y_lineEdit_2->setFocus();
        return;
    }
    if (checkEmptyParameter(ui->z_lineEdit_2->text(), "final coordinate z "))
    {
        ui->z_lineEdit_2->setFocus();
        return;
    }
    QString molecules;
    molecules = "structure " + ui->structure_lineEdit->text() + "\n" ;
    molecules.append("number " + ui->numbermol_lineEdit->text() + " \n");
    molecules.append(ui->constraint_comboBox->currentText() + " " + ui->x_lineEdit->text() + " " + ui->y_lineEdit->text() + " " + ui->z_lineEdit->text() + " ");
    molecules.append(ui->x_lineEdit_2->text() + " " + ui->y_lineEdit_2->text() + " " + ui->z_lineEdit_2->text() + " \n");
    molecules.append("end structure \n \n");
    ui->addedmolecules_textEdit->append(molecules);
    ui->structure_lineEdit->clear();
    ui->numbermol_lineEdit->clear();
}

bool packmolgui::checkEmptyParameter(QString text, QString label)
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

void packmolgui::on_run_pushButton_clicked()
{
    if (checkEmptyParameter(ui->jobname_lineEdit->text(), "JOBNAME"))
    {
        ui->jobname_lineEdit->setFocus();
        return;
    }
    if (ui->addedmolecules_textEdit->toPlainText().isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("no molecule is added. please add at least one molecules");
        msgBox.exec();
        return;
    }
 //PR log edit get focus
    ui->log_textEdit->append("---------create packmol input file ----------------\n \n \n ");
    QString inputfilename = ui->jobname_lineEdit->text() + ".inp";
    QFile file(inputfilename);
    if (file.open(QFile::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&file);
        out << "tolerance " << ui->tolerance_lineEdit->text() << endl;
        out << "output " << ui->jobname_lineEdit->text() + ".pdb" << endl;
        out << "filetype pdb" <<endl;
        out << "add_box_sides 1.0 \n" << endl;
        out << ui->addedmolecules_textEdit->toPlainText();
        file.close();
    }
    QStringList command;
//WIN32
    command << "/C" << "packmol.exe < " << ui->jobname_lineEdit->text() + ".inp";
    packmolprocess->start("cmd.exe",command);
    packmolprocess->waitForFinished();
}
void packmolgui::ReadPackmolProcess()
{
    ui->log_textEdit->append(packmolprocess->readAll());
}

void packmolgui::on_view_pushButton_clicked()
{
    emit viewStructureSignal(ui->jobname_lineEdit->text() + ".pdb");
}
