#include "group.h"
#include "ui_group.h"



GroUP::GroUP(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GroUP)
{
    ui->setupUi(this);
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
    connect(ui->actionTopology_Generator ,SIGNAL(triggered()),this, SLOT(topologygen_clicked()));
    connect(ui->actionTextEditor, SIGNAL(triggered()),this, SLOT(textEditor_clicked()));

    //load setting
    settingform = new FormSetting;
    settingform->loadSettings();
    //load Setting

    //layout
    QGridLayout *mainlayout = new QGridLayout;
    mainlayout->setSizeConstraint(QLayout::SetFixedSize);
    mainlayout->addWidget(ui->textEdit_3,0,0);
    mainlayout->addWidget(ui->textEdit_2,0,4);
    mainlayout->addWidget(ui->textEdit,2,0,1,2);
    setLayout(mainlayout);
}

GroUP::~GroUP()
{
    delete ui;
    VMDExec->close();
    delete VMDExec;
    settingform->close();
    delete settingform;
}

void GroUP::plotup_clicked()
{
    plot = new FormPlot;
    plot->show();
}

void GroUP::packmolgui_clicked()
{
    packmolform = new packmolgui;
    packmolform->setPackmol(settingform->getPackmol());
     //view structure from Packmol GUI Form using VMD
    connect(packmolform,SIGNAL(viewStructureSignal(QString)),this, SLOT(viewStructure(QString)));
    //open packmol form
    packmolform->show();
}

void GroUP::gromacsprep_clicked()
{
    gromacsprepform = new GromacsPrep;
    gromacsprepform->setGMX(settingform->getGMXExec(),settingform->getGMXLib());
    //view structure from GROMACS Preparation Form using VMD
    connect(gromacsprepform, SIGNAL(viewStructureSignal(QString)), this, SLOT(viewStructure(QString)));
    //open GROMACS PREPARATION FORM
    gromacsprepform->show();
}

void GroUP::gromacsup_clicked()
{
    gromacsupform = new GromacsUP;
    gromacsupform->setGMXExec(settingform->getGMXExec());
    //view structure from GROMACS Simulation Form using VMD
    connect(gromacsupform, SIGNAL(viewStructureSignal(QString)), this, SLOT(viewStructure(QString)));
    //view trajectory from GROMACS Simulation Form using VMD
    connect(gromacsupform, SIGNAL(viewTrajectorySignal(QString,QString)), this, SLOT(viewTrajectory(QString,QString)));
    //view input file
    connect(gromacsupform,SIGNAL(viewInputFile(QString)),this,SLOT(viewTextFile(QString)));
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
    gromacsAnalysisForm->setGMXExec(settingform->getGMXExec());
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
//view trajectory using VMD

void GroUP::viewTrajectory(QString structure, QString trjname)
{
    QString VMD = settingform->getVMDPath() ;
    QStringList command;
    command << structure << trjname;
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

void GroUP::topologygen_clicked()
{
    gentopo = new TopologyGenerator;
    gentopo->setGMXLib(settingform->getGMXLib());
    //open topology generator form
    gentopo->show();
}

void GroUP::textEditor_clicked()
{
    texteditorform = new TextEditor;
    texteditorform->show();

}

void GroUP::viewTextFile(QString filename)
{
    texteditorform = new TextEditor;
    texteditorform->openFile(filename);
    texteditorform->show();
}
