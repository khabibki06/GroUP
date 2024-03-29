#ifndef GROUP_H
#define GROUP_H

#include <QMainWindow>
#include <formplot.h>
#include <packmolgui.h>
#include <gromacsprep.h>
#include <gromacsup.h>
#include <formsetting.h>
#include <gromacsanalysis.h>
#include <formversion.h>
#include <QList>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QWidget>
#include <topologygenerator.h>
#include <texteditor.h>

namespace Ui {
class GroUP;
}

class GroUP : public QMainWindow
{
    Q_OBJECT

public:
    explicit GroUP(QWidget *parent = 0);
    ~GroUP();
public slots:
    void viewStructure(QString structure);
    void viewTrajectory(QString structure, QString trjname);
    void readViewStructure();
    void viewGraph(QString inputxvg);
    void viewTextFile(QString filename);

private slots:
    void plotup_clicked();
    void packmolgui_clicked();
    void gromacsprep_clicked();
    void gromacsup_clicked();
    void setting_clicked();
    void gromacsAnalysis_clicked();
    void version_clicked();
    void topologygen_clicked();
    void textEditor_clicked();

private:
    Ui::GroUP *ui;
    QProcess *VMDExec;
    //open child form
    FormPlot *plot;
    packmolgui *packmolform;
    GromacsPrep *gromacsprepform;
    GromacsUP *gromacsupform;
    FormSetting *settingform;
    GromacsAnalysis *gromacsAnalysisForm;
    FormVersion *versionform;
    TopologyGenerator *gentopo;
    TextEditor *texteditorform;

};

#endif // GROUP_H
