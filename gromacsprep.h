#ifndef GROMACSPREP_H
#define GROMACSPREP_H

#include <QWidget>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

namespace Ui {
class GromacsPrep;
}

class GromacsPrep : public QWidget
{
    Q_OBJECT

public:
    explicit GromacsPrep(QWidget *parent = 0);
    ~GromacsPrep();

public slots:
    void ReadPreparation();
    void StopPreparation(int,QProcess::ExitStatus);

signals:
    void viewStructureSignal(const QString text);

private slots:
    void on_Coordinate_toolButton_clicked();

    void on_WorkingDir_toolButton_clicked();

    void on_RunPrepare_pushButton_clicked();

    void on_SolvateYES_radioButton_clicked();

    void on_SolvateNO_radioButton_2_clicked();

    void on_BiomolIonsYES_radioButton_3_clicked();

    void on_neutralize_checkBox_2_clicked();

    void on_VEW_pushButton_clicked();

private:
    Ui::GromacsPrep *ui;
    void GromacsPrep::createFFList();
    void GromacsPrep::stripWater(QString filename);
    bool GromacsPrep::checkEmptyParameter(QString text, QString label);
    QProcess *PreparationExec;
    QString GromacsPrep::ExecDir();
};

#endif // GROMACSPREP_H
