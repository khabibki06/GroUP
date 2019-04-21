#include "group.h"
#include "ui_group.h"



GroUP::GroUP(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GroUP)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);
    //child form pointer


    // dedicated process to view structure using VMD
    VMDExec = new QProcess(this);
    VMDExec->setProcessChannelMode(QProcess::MergedChannels);
    connect(VMDExec,SIGNAL(readyRead()),this,SLOT(readViewStructure()));
    //
    connect(ui->actionPlotUP, SIGNAL(triggered()),this, SLOT(plotup_clicked()));
    connect(ui->actionPackmol_GUI, SIGNAL(triggered()),this, SLOT(packmolgui_clicked()));
    connect(ui->actionBiomolecules_Preparation, SIGNAL(triggered()),this,SLOT(gromacsprep_clicked()));
    connect(ui->actionSimulation, SIGNAL(triggered()),this, SLOT(gromacsup_clicked()));
    connect(ui->actionSetting, SIGNAL(triggered()),this, SLOT(setting_clicked()));
    connect(ui->actionAnalysis, SIGNAL(triggered()),this, SLOT(gromacsAnalysis_clicked()));
    connect(ui->actionVersion,SIGNAL(triggered()),this, SLOT(version_clicked()));

    //
    settingform = new FormSetting;
    //view graph from energy form

}

GroUP::~GroUP()
{
    delete ui;
}

void GroUP::plotup_clicked()
{
    plot = new FormPlot;
    plot->show();
}

void GroUP::packmolgui_clicked()
{
    packmolform = new packmolgui;
     //view structure from Packmol GUI Form using VMD
    connect(packmolform,SIGNAL(viewStructureSignal(QString)),this, SLOT(viewStructure(QString)));
    //open packmol form
    packmolform->show();
}

void GroUP::gromacsprep_clicked()
{
    gromacsprepform = new GromacsPrep;
    //view structure from GROMACS Preparation Form using VMD
    connect(gromacsprepform, SIGNAL(viewStructureSignal(QString)), this, SLOT(viewStructure(QString)));
    //open GROMACS PREPARATION FORM
    gromacsprepform->show();
}

void GroUP::gromacsup_clicked()
{
    gromacsupform = new GromacsUP;
    //view structure from GROMACS Simulation Form using VMD
    connect(gromacsupform, SIGNAL(viewStructureSignal(QString)), this, SLOT(viewStructure(QString)));
    //open GROMACS SIMULATIONS FORM
    gromacsupform->show();
}
void GroUP::setting_clicked()
{
    settingform->show();

}

void GroUP::gromacsAnalysis_clicked()
{
    gromacsAnalysisForm = new GromacsAnalysis;
       //view graph from energy form
    connect(gromacsAnalysisForm, SIGNAL(viewGraphSignal(QString)), this, SLOT(viewGraph(QString)));
    //open form
    gromacsAnalysisForm->show();
}


//View Structure using VMD
void GroUP::viewStructure(QString structure)
{
    QString VMD = settingform->getVMDPath() ;
    QStringList command;
    command << structure;
    VMDExec->start(VMD,command);
}

void GroUP::readViewStructure()
{
    ui->textEdit->append(VMDExec->readAll());
}

//view graph
void GroUP::viewGraph(QString inputxvg)
{
    plot = new FormPlot;
    plot->setInput(inputxvg);
    plot->show();


}

void GroUP::version_clicked()
{
    versionform = new FormVersion;
    //open Version FORM
    versionform->show();
}
