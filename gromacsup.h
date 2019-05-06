#ifndef GROMACSUP_H
#define GROMACSUP_H

#include <QWidget>
#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class GromacsUP;
}

class GromacsUP : public QWidget
{
    Q_OBJECT

public:
    explicit GromacsUP(QWidget *parent = 0);
    ~GromacsUP();
    void GromacsUP::setGMXExec(QString gmxexec);


public slots:
    void RunSimulationCommand();
    void ReadSimulationCommand();
    void StopSimulationCommand(int exitCode, QProcess::ExitStatus exitStatus);
protected:

signals:
    //view structure
    void viewStructureSignal(const QString text);
    //view trajectory
    void viewTrajectorySignal(const QString text, const QString trjname);
    //view input file using builtin text editor
    void viewInputFile(const QString filename);


private slots:

    void on_simulationType_comboBox_currentTextChanged(const QString &arg1);

    void on_ensemble_comboBox_currentTextChanged(const QString &arg1);

    void on_pcoupl_comboBox_currentTextChanged(const QString &arg1);

    void on_workingDir_toolButton_clicked();

    void on_coordinate_toolButton_clicked();

    void on_topology_toolButton_clicked();

    void on_reference_toolButton_clicked();

    void on_binaryInput_toolButton_clicked();

    void on_createInputfile_pushButton_clicked();

    void on_nsteps_lineEdit_textChanged(const QString &arg1);

    void on_tcoupl_comboBox_currentTextChanged(const QString &arg1);

    void on_viewInputfile_pushButton_clicked();

    void on_RestraintMolecules_checkBox_clicked();

    void on_continuation_checkBox_clicked();

    void on_ViewOutputSimulation_pushButton_clicked();

    void on_trj_pushButton_clicked();

    void on_jobName_lineEdit_textChanged(const QString &arg1);

    void on_jobName_lineEdit_textEdited(const QString &arg1);

    void on_timeSteps_lineEdit_textChanged(const QString &arg1);

    void on_checkpoint_toolButton_clicked();

private:
    Ui::GromacsUP *ui;
    void GromacsUP::changeFileSimulationWidget();
    void GromacsUP::defaultSimulationWidget();
    void GromacsUP::changeEnsemble();
    void GromacsUP::createSimulationInputFile();
    QString GromacsUP::OpenCoordinateFile();
    QString GromacsUP::setCurrentWorkingDir();
    void GromacsUP::DefaultSetting();
    QString GromacsUP::ExecDir();
    QProcess *exec;
    QStringList GromacsUP::RunGROMPP();
    QStringList GromacsUP::RunConvertTpr();
    QStringList GromacsUP::RunMdrun();
    bool GromacsUP::checkErrorMessage(QString text, QString label);
    QString GromacsUP::SpacingFilename(QString text);
    void GromacsUP::calculateSimulationTime();
    QString gmxExec; //setting gromacs Executable
};

#endif // GROMACSUP_H
