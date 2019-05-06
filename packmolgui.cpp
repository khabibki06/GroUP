#include "packmolgui.h"
#include "ui_packmolgui.h"

packmolgui::packmolgui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::packmolgui)
{
    ui->setupUi(this);
    //Process
    packmolprocess = new QProcess(this);
    packmolprocess->setProcessChannelMode(QProcess::MergedChannels);
    //read output packmol
    connect(packmolprocess,SIGNAL(readyRead()),this,SLOT(ReadPackmolProcess()));
    ui->packmol->setVisible(0);
    //REG VALIDATOR
    ui->numbermol_lineEdit->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]*")));
    ui->x_lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
    ui->y_lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
    ui->z_lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
    ui->x_lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
    ui->y_lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
    ui->z_lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
     ui->tolerance_lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]*.?[0-9]*")));
    //set working directory
    ui->workdir_lineEdit->setText(QDir::current().absolutePath());
    //set table
    QStringList tableHeaders;
    tableHeaders << "molecules" << "numbers" << "constrain type" <<"constrain" << "extra parameter";
    ui->tableWidget->setHorizontalHeaderLabels(tableHeaders);
    ui->tableWidget->setStyleSheet("QTableView {selection-background-color: red;}");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //set pop up menu
    ui->tableWidget->setContextMenuPolicy((Qt::CustomContextMenu));
    //action when right click
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contexMenuRequest(const QPoint &)));
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
    QStringList moleculesData;
    moleculesData << ui->structure_lineEdit->text();
    moleculesData << ui->numbermol_lineEdit->text();
    moleculesData << ui->constraint_comboBox->currentText();
    moleculesData << ui->x_lineEdit->text() + " " + ui->y_lineEdit->text() + " " + ui->z_lineEdit->text() + " " +  ui->x_lineEdit_2->text() + " " +  ui->y_lineEdit_2->text() +" " + ui->z_lineEdit_2->text();
    moleculesData << " ";
    addMoleculesToTable(moleculesData);
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
    if (ui->tableWidget->rowCount() == 0)
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
//        out << ui->addedmolecules_textEdit->toPlainText();
        for (int i = 0; i<ui->tableWidget->rowCount();i++)
        {
            out << "structure " << ui->tableWidget->item(i,0)->text() << endl;
            out << "number " << ui->tableWidget->item(i,1)->text() <<endl;
            out << ui->tableWidget->item(i,2)->text() + " " + ui->tableWidget->item(i,3)->text() << endl;
            out << "end structure" << endl <<endl;
        }

        file.close();
    }
    QStringList command;
//WIN32
    command << "/C" << packmolExec << "<" << ui->jobname_lineEdit->text() + ".inp";
    packmolprocess->start("cmd.exe",command);
//    packmolprocess->waitForFinished();
}
void packmolgui::ReadPackmolProcess()
{
    ui->log_textEdit->append(packmolprocess->readAll());
}

void packmolgui::on_view_pushButton_clicked()
{
    emit viewStructureSignal(ui->jobname_lineEdit->text() + ".pdb");
}

void packmolgui::setPackmol(QString packmol)
{
    ui->packmol->setText(packmol);
}

void packmolgui::addMoleculesToTable(QStringList moleculesData)
{
    int rowcount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowcount);
    for (int i = 0; i<moleculesData.size();i++)
    {
        ui->tableWidget->setItem(rowcount,i,new QTableWidgetItem(moleculesData.value(i)));
    }
}


void packmolgui::contexMenuRequest(const QPoint &pos)
{
    QMenu menu(tr("menu"), this);
    menu.setAttribute(Qt::WA_DeleteOnClose);
    menu.addAction("Delete Selected Molecules", this, SLOT(deleteSelectedMolecule()));
    menu.addSeparator();
    menu.addAction("Clear All Molecules", this, SLOT(clearMolecules()));
    menu.exec(ui->tableWidget->mapToGlobal(pos));
}

void packmolgui::deleteSelectedMolecule()
{
    int row = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
}

void packmolgui::clearMolecules()
{
     ui->tableWidget->setRowCount(0);
}

