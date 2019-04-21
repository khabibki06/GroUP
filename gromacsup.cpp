#include "gromacsup.h"
#include "ui_gromacsup.h"

GromacsUP::GromacsUP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GromacsUP)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);
    //initiate plot from
    //default simulation widget
    defaultSimulationWidget();

    //setting output simulation cmd to textviewer
    exec = new QProcess(this);
    exec->setProcessChannelMode(QProcess::MergedChannels);
    //run button click
    connect(ui->runSimulation_pushButton, SIGNAL(clicked()),this, SLOT(RunSimulationCommand()));
    //read processed
    connect(exec, SIGNAL(readyRead()), this, SLOT(ReadSimulationCommand()));
    //finished
    connect(exec, SIGNAL(finished(int QProcess::ExitStatus)), this, SLOT(StopSimulationCommand(int,QProcess::ExitStatus)));
    //---------end---------------

    //set exe dir
}
GromacsUP::~GromacsUP()
{
    delete ui;
}
void GromacsUP::defaultSimulationWidget()
{
    //File Setting
    ui->coordinate_lineEdit->setEnabled(1);
    ui->coordinate_toolButton->setEnabled(1);
    ui->topology_lineEdit->setEnabled(1);
    ui->topology_toolButton->setEnabled(1);
    ui->reference_lineEdit->setDisabled(1);
    ui->reference_toolButton->setDisabled(1);
    ui->binaryInput_lineEdit->setDisabled(1);
    ui->binaryInput_toolButton->setDisabled(1);
    //minimization tab is enable
    ui->SimulationOptionMinimization_tab->setEnabled(1);
    //Time Tab
    ui->timeSteps_lineEdit->setDisabled(1);
    //Continu check box
    ui->continuation_checkBox->setChecked(0);
    ui->continuation_checkBox->setDisabled(1);
    // Output Tab is Invisible
    ui->SimulationOptionOutput_tab->setDisabled(1);
    //deactivated ensemble frame
    ui->frame_ensemble->setDisabled(1);
   // thermostat tab is invisible
    ui->SimulationOptionThermostat_tab->setDisabled(1);
    //Pressure Tab is invisible
    ui->SimulationOptionPressure_tab->setDisabled(1);
    //Bonding Tab is invisible
    ui->SimulationOptionBonding_tab->setDisabled(1);
    //additional setting
    ui->simulationtime_lineEdit->setHidden(1);
    ui->RestraintMolecules_checkBox->setDisabled(1);
    ui->RestraintMolecules_checkBox->setChecked(0);
    //Non Bonding is invisible
    ui->SimulationOptionNonBonding_tab->setEnabled(1);
}
void GromacsUP::changeEnsemble()
{
    QString ensemble = ui->ensemble_comboBox->currentText();
    if (ensemble == "NVT")
    {
        ui->pcoupl_comboBox->setCurrentText("no");
        ui->SimulationOptionPressure_tab->setDisabled(1);
    } else {
        ui->pcoupl_comboBox->setCurrentText("Parrinello-Rahman");
        ui->SimulationOptionPressure_tab->setEnabled(1);
    }
}

void GromacsUP::changeFileSimulationWidget()
{
    QString selectedType = ui ->simulationType_comboBox -> currentText();
    if (selectedType == "Minimization")
    {
        defaultSimulationWidget();

    }
    else if (selectedType == "Extending Simulation")
    {
        defaultSimulationWidget();
        ui->binaryInput_lineEdit->setEnabled(1);
        ui->binaryInput_toolButton->setEnabled(1);
        ui->SimulationOptionMinimization_tab->setDisabled(1);
        ui->coordinate_lineEdit->setDisabled(1);
        ui->coordinate_toolButton->setDisabled(1);
        ui->topology_lineEdit->setDisabled(1);
        ui->topology_toolButton->setDisabled(1);
        ui->frame_ensemble->setDisabled(1);
        ui->continuation_checkBox->setDisabled(1);
        ui->timeSteps_lineEdit->setVisible(0);
        ui->SimulationOptionNonBonding_tab->setDisabled(1);

    }
    else {
        defaultSimulationWidget();
        ui->SimulationOptionMinimization_tab->setDisabled(1);
        ui->timeSteps_lineEdit->setEnabled(1);
        ui->frame_ensemble->setEnabled(1);
        ui->SimulationOptionThermostat_tab->setEnabled(1);
        ui->SimulationOptionBonding_tab->setEnabled(1);
        ui->continuation_checkBox->setEnabled(1);
        ui->continuation_checkBox->setEnabled(1);
        ui->SimulationOptionPressure_tab->setEnabled(1);
        ui->SimulationOptionNonBonding_tab->setEnabled(1);
        changeEnsemble();
        ui->RestraintMolecules_checkBox->setEnabled(1);
        ui->SimulationOptionOutput_tab->setEnabled(1);
    }
}
//-----------open coordinate File -----------------
QString GromacsUP::OpenCoordinateFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open coordinate file ..."), QDir::currentPath(),tr("Coordinate Files (*.gro *.pdb)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    return fileNameRelative;
}


//-----------end open coordinate File
//---------view using VMD


//------------error message casued missing file or name-------
bool GromacsUP::checkErrorMessage(QString text, QString label)
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

//----------------
//------------set current working directory----------
QString GromacsUP::setCurrentWorkingDir()
{
    QString directory = QFileDialog::getExistingDirectory(this,tr("open working directory ..."),QDir::currentPath());
    QDir::setCurrent(directory);
    return directory;
}

//exectuble dir
QString GromacsUP::ExecDir()
{
    QString Dir = QCoreApplication::applicationDirPath();
    return Dir;
}

//-----------create simulation input

void GromacsUP::createSimulationInputFile()
{
    ui->outputDiplay_textEdit->append("Creating Input File.......");
    QString modeMD = ui->simulationType_comboBox->currentText();
    QString fileName = ui->jobName_lineEdit->text() + ".mdp";
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ";------------Run Parameter-------------------"<<endl;
        if (modeMD == "Minimization")
        {
            out << "integrator      = " << "steep" <<endl;
            out << "emtol           = " << ui->emtol_lineEdit->text() <<endl;
            out << "emstep        = " << ui->emsteps_lineEdit->text()<<endl;
        } else
        {
            if (ui->RestraintMolecules_checkBox->isChecked())
                out << "define                  = -DPOSRES" <<endl;
            out << "integrator     = " << "md" <<endl;
            out << "dt             = " << ui->timeSteps_lineEdit->text()<<endl;
        }
        out << "nsteps              = " << ui->nsteps_lineEdit->text()<<endl;
        if (ui->SimulationOptionOutput_tab->isEnabled())
        {
            out << ";------------Output Control -------------------"<<endl;
            out << "nstxout            = " << ui->nstxout_spinBox->text()<<endl;
            out << "nstvout            = " << ui->nstvout_spinBox->text()<<endl;
            out << "nstenergy          = " << ui->nstenergy_spinBox->text()<<endl;
            out << "nstlog            = " << ui->nstlog_spinBox->text()<<endl;
        }
        out << ";-------------------Bonds Parameter --------------------"<<endl;
        if (ui->continuation_checkBox->isChecked() && ui->continuation_checkBox->isEnabled())
        {
            out << "continuation            = yes " <<endl;
            out << "gen_vel                 = no " <<endl;
        }
        else {
            out << "continuation            = no " <<endl;
            out << "gen_vel                 = yes" <<endl;
            if (ui->SimulationVelTemp_lineEdit->text().isEmpty())
                out << "gen_vel                 = 300 " <<endl;
            else
                out << "gen_temp                = " << ui->SimulationVelTemp_lineEdit->text()<<endl;;
        }
        if (ui->SimulationOptionBonding_tab->isEnabled())
        {
            out << "constraint_algorithm  = " << "lincs " << endl;
            out << "constraints      = " << ui->constraints_comboBox->currentText() <<endl;
        }
        //writing neighbors searching parameters
        out<< ";----------Neighbors Searching Parameters ----------" << endl;
        out << "nstlist         = " << ui->nstlist_lineEdit->text()<<endl;
        out << "cutoff-scheme   = " << ui->cutoffscheme_comboBox->currentText()<<endl;
        out << "ns_type         = " << ui->nsttype_comboBox->currentText()<<endl;
        out << "coulombtype     = " << ui->coltype_comboBox->currentText()<<endl;
        out << "rcoulomb        = " << ui->rcoloumb_lineEdit->text()<<endl;
        out << "rvdw            = " << ui->rvdw_lineEdit->text()<< endl;
        out << "pbc             = " << ui->pbc_comboBox->currentText()<<endl;
        if (ui->simulationType_comboBox->currentText() != "Minimization")
        {
            //writting temperature coupling parameters
            out << ";---------temprature coupling parameters--------------" << endl;
            out << "tcoupl                  = " << ui->tcoupl_comboBox->currentText()<<endl;
            if (ui->SimulationOptionThermostat_tab->isEnabled() && ui->tcoupl_comboBox->currentText() != "no")
            {
                out << "tc-grps                 = " << ui->tcGroups_lineEdit->text()<<endl;
                out << "tau_t                   = " << ui->taut_lineEdit->text()<<endl;
                out << "ref_t                   = " << ui->reft_lineEdit->text()<<endl;
            }
            //writting pressure coupling parameters
            out << "; -------------pressure coupling parameters----------" << endl;
            out << "pcoupl                  = " << ui->pcoupl_comboBox->currentText()<<endl;
            if (ui->SimulationOptionPressure_tab->isEnabled() && ui->pcoupl_comboBox->currentText() != "no" )
            {
                out << "pcoupltype              = " << ui->pcouopletype_comboBox->currentText()<<endl;
                out << "tau_p                   = " << ui->taup_lineEdit->text()<<endl;
                out << "ref_p                   = " << ui->refp_lineEdit->text()<<endl;
                if (ui->pcoupl_comboBox->currentText() == "Parrinello-Rahman")
                {
                    out << "compressibility         = 4.5e-5" <<endl;
                    out << "refcoord_scaling        = com " <<endl;
                }
            }
        }
        file.close();
    }
    ui->outputDiplay_textEdit->append("Input File Created.......");
    ui->outputDiplay_textEdit->append("You can view and edit input file using view/edit input file button");

}
//--------create simulation run
QStringList GromacsUP::RunGROMPP()
{
    QString mdpfile = ui->jobName_lineEdit->text() + ".mdp";
    QString tprfile = ui->jobName_lineEdit->text() + ".tpr";
    QStringList cmdcommand;
    cmdcommand <<"grompp" << "-f" << mdpfile << "-c" << ui->coordinate_lineEdit->text() << "-p" << ui->topology_lineEdit->text() << "-o" << tprfile << "-maxwarn" << "100";
    if (ui->RestraintMolecules_checkBox->isChecked())
        cmdcommand << "-r" << ui->reference_lineEdit->text();
    return cmdcommand;
}

QStringList GromacsUP::RunMdrun()
{
    QStringList cmdcommand;
    cmdcommand << "mdrun" << "-v" << "-deffnm" << ui->jobName_lineEdit->text();
    return cmdcommand;
}

//----------startup extra setting

//----spacing treatment
QString GromacsUP::SpacingFilename(QString text)
{
    if (text.contains(" "))
        text = "'" + text + "'";
    return text;
}


//-----------------actions

void GromacsUP::on_simulationType_comboBox_currentTextChanged(const QString &arg1)
{
    changeFileSimulationWidget();
}

void GromacsUP::on_ensemble_comboBox_currentTextChanged(const QString &arg1)
{
    changeEnsemble();
}

void GromacsUP::on_pcoupl_comboBox_currentTextChanged(const QString &arg1)
{
    if (ui->pcoupl_comboBox->currentText() == "no")
    {
        ui->pcouopletype_comboBox->setDisabled(1);
        ui->taup_lineEdit->setDisabled(1);
        ui->refp_lineEdit->setDisabled(1);
    } else{
        ui->pcouopletype_comboBox->setEnabled(1);
        ui->taup_lineEdit->setEnabled(1);
        ui->refp_lineEdit->setEnabled(1);
    }
}

void GromacsUP::on_workingDir_toolButton_clicked()
{
    QString directory = setCurrentWorkingDir();
    ui->workingDir_lineEdit->setText(directory);
}

void GromacsUP::on_coordinate_toolButton_clicked()
{
    QString filename = OpenCoordinateFile();
    ui->coordinate_lineEdit->setText(filename);
}

void GromacsUP::on_topology_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open topology file ..."), QDir::currentPath(),tr("Topology Files (*.top)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->topology_lineEdit->setText(fileNameRelative);
}

void GromacsUP::on_reference_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open restraint coordinate file ..."), QDir::currentPath(),tr("Coordinate Files (*.gro *.pdb)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->reference_lineEdit->setText(fileNameRelative);
}

void GromacsUP::on_binaryInput_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open gromacs binary input file ..."), QDir::currentPath(),tr("Binary Gromacs Files (*.tpr)"));
    QDir dir(QDir::currentPath());
    QString fileNameRelative = dir.relativeFilePath(fileName);
    ui->binaryInput_lineEdit ->setText(fileNameRelative);
}

void GromacsUP::on_createInputfile_pushButton_clicked()
{
    if (checkErrorMessage(ui->jobName_lineEdit->text(), "JOBNAME "))
        return;
    createSimulationInputFile();
}

void GromacsUP::on_nsteps_lineEdit_textChanged(const QString &arg1)
{
    ui->nsteps_lineEdit->setValidator(new QIntValidator());
    int totalTime = ui->nsteps_lineEdit->text().toInt() * ui->timeSteps_lineEdit->text().toInt()/1000;
    ui->simulationtime_lineEdit->setText(QString::number(totalTime));
}

void GromacsUP::on_tcoupl_comboBox_currentTextChanged(const QString &arg1)
{
    if (ui->tcoupl_comboBox-> currentText() == "no")
    {
        ui->tcGroups_lineEdit->setDisabled(1);
        ui->taut_lineEdit->setDisabled(1);
        ui->reft_lineEdit->setDisabled(1);
    } else {
        ui->tcGroups_lineEdit->setEnabled(1);
        ui->taut_lineEdit->setEnabled(1);
        ui->reft_lineEdit->setEnabled(1);
    }
}


void GromacsUP::on_viewInputfile_pushButton_clicked()
{
    QString cmdcommand = "notepad.exe " + ui->jobName_lineEdit->text() + ".mdp";
    QProcess *process = new QProcess(this);
    process->start(cmdcommand);
}


void GromacsUP::RunSimulationCommand()
{
    if (checkErrorMessage(ui->jobName_lineEdit->text(), "JOBNAME "))
        return;
    if (checkErrorMessage(ui->coordinate_lineEdit->text(), "Coordinate File "))
        return;
    if (checkErrorMessage(ui->topology_lineEdit->text(), "Topology File "))
        return;
    if (ui->RestraintMolecules_checkBox->isChecked())
    {
        if (checkErrorMessage(ui->reference_lineEdit->text(), "Reference Coordinate "))
            return;
    }
    if (QFile::exists(ui->jobName_lineEdit->text() + ".mdp"))
    {

    } else
        createSimulationInputFile();
    exec->start("gmx.exe", RunGROMPP() );
    exec->waitForFinished();
    exec->start("gmx.exe", RunMdrun());
}
void GromacsUP::ReadSimulationCommand()
{
    ui->outputDiplay_textEdit->append(exec->readAll());
}

void GromacsUP::StopSimulationCommand(int exitCode, QProcess::ExitStatus exitStatus)
{

}


void GromacsUP::on_RestraintMolecules_checkBox_clicked()
{
    if (ui->RestraintMolecules_checkBox->isChecked())
    {
        ui->reference_lineEdit->setEnabled(1);
        ui->reference_toolButton->setEnabled(1);
        ui->reference_label->setEnabled(1);
    } else
    {
        ui->reference_lineEdit->setDisabled(1);
        ui->reference_toolButton->setDisabled(1);
        ui->reference_label->setDisabled(1);
    }
}

void GromacsUP::on_continuation_checkBox_clicked()
{
    if (ui->continuation_checkBox->isChecked())
        ui->SimulationVelTemp_lineEdit->setDisabled(1);
    else
        ui->SimulationVelTemp_lineEdit->setEnabled(1);
}


void GromacsUP::on_ViewOutputSimulation_pushButton_clicked()
{
    emit viewStructureSignal(ui->jobName_lineEdit->text() + ".gro");
}
