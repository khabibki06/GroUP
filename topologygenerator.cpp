#include "topologygenerator.h"
#include "ui_topologygenerator.h"

TopologyGenerator::TopologyGenerator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopologyGenerator)
{
    ui->setupUi(this);
    ui->gmxlib->setVisible(0);
    ui->workDir_lineEdit->setText(QDir::current().absolutePath());
}

TopologyGenerator::~TopologyGenerator()
{
    delete ui;
}

void TopologyGenerator::readPDB(QString filename)
{
    QFile file(filename);
    ui->info_textEdit->clear();
    ui->info_textEdit->append(";molecules     numbers");
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    QString line;
    QString resname,temp;
    QStringList resnamelist, tempList;
    QList <int> residlist;
    int resid;
    QMultiMap <QString, int> residKey;
    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.startsWith("HETATM")||line.startsWith("ATOM"))
        {
            resid = line.mid(22,4).replace(" ","").toInt();
            resname = line.mid(17,4).replace(" ","");
            temp = resname + QString::number(resid);
            if (!tempList.contains(temp))
            {
                tempList << temp;
                residlist << resid;
                resnamelist << resname;
            }

        }
    }
    for (int i = 0;i<residlist.count();i++)
    {
        residKey.insert(resnamelist[i],residlist[i]);
    }
    QSet<QString>::iterator it;
    QSet<QString> keys = residKey.keys().toSet();
    for (it = keys.begin();it != keys.end();++it)
    {

        ui->info_textEdit->append(QString(*it) + "     " + QString::number(residKey.count(*it)));
    }
}


void TopologyGenerator::showEvent(QShowEvent *ev)
{
    createFFList();

}

void TopologyGenerator::on_workDir_toolButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("open working directory ..."),QDir::currentPath());
    QDir::setCurrent(directory);
    ui->workDir_lineEdit->setText(directory);
}

void TopologyGenerator::on_PDB_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open coordinate file ..."), QDir::currentPath(),tr("Coordinate Files (*.pdb)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->PDB_lineEdit->setText(fileNameRelative);
}

void TopologyGenerator::on_PDB_lineEdit_textChanged(const QString &arg1)
{
    readPDB(ui->PDB_lineEdit->text());
    int pos = ui->PDB_lineEdit->text().lastIndexOf(".");
    QString name = ui->PDB_lineEdit->text().left(pos);
    if (ui->systemName_lineEdit->text().isEmpty())
        ui->systemName_lineEdit->setText(name);
}

void TopologyGenerator::on_parameter_toolButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("select one or more parameter files ..."), QDir::currentPath(),tr("parameter fileas (*.itp)"));
    QDir dir(QDir::currentPath());
    QStringList filesNamesRelative;
    for (int i = 0;i<fileNames.size();i++)
           filesNamesRelative << dir.relativeFilePath(fileNames[i]);
    ui->parameter_textEdit->append(filesNamesRelative.join("\n"));
}

void TopologyGenerator::on_info_textEdit_textChanged()
{

}

void TopologyGenerator::createFFList()
{
    QDir ff_path(ui->gmxlib->text());
    QStringList ff = ff_path.entryList(QDir::Dirs);
    ff.removeAt(1);
    ff.removeAt(0);
    ui->selectFF_comboBox->addItems(ff);
    if (ui->selectFF_comboBox->findText("gromos54a7_atb.ff") != -1)
    {
        ui->selectFF_comboBox->setCurrentText("gromos54a7_atb.ff");
    }
}

void TopologyGenerator::setGMXLib(QString gmxlib)
{
    ui->gmxlib->setText(gmxlib);
}

void TopologyGenerator::on_pushButton_clicked()
{
    if (checkErrorMessage(ui->PDB_lineEdit->text(),"PDB file"))
        return;
    int pos = ui->PDB_lineEdit->text().lastIndexOf(".");
    QString filename = ui->PDB_lineEdit->text().left(pos) + ".top";
    QFile file(filename);
    QStringList itpfiles;
    itpfiles << ui->parameter_textEdit->toPlainText().split("\n");
    if (file.open(QFile::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&file);
        out <<";include GROMACS Forcefield" <<endl;
        out << "#include \"" + ui->selectFF_comboBox->currentText() + "/forcefield.itp\"" << endl << endl;
        if (ui->ions_checkBox->isChecked())
        {
            out << "; Include topology for ions" << endl;
            out << "#include \"" + ui->selectFF_comboBox->currentText() + "/ions.itp\"" << endl << endl;
        }
        if (ui->water_checkBox->isChecked())
        {
            out << "; Include water topology" << endl;
            out << "#include \"" + ui->selectFF_comboBox->currentText() + "/spce.itp\"" << endl << endl;
        }
        out <<"; include molecules parameter " << endl;
        if (!ui->parameter_textEdit->toPlainText().isEmpty())
        {
            foreach (const QString &itp, itpfiles) {
               out << "#include \""  << itp << "\"" << endl;
            }
        }
        out << endl;
        out << "[ system ] " << endl;
        out << ui->systemName_lineEdit->text() << endl << endl;
        out << "[ molecules ] " << endl;
        out << ui->info_textEdit->toPlainText();
        file.close();
    }
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(filename + " is successfully generated.....");
    msgBox.exec();
}

bool TopologyGenerator::checkErrorMessage(QString text, QString label)
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
