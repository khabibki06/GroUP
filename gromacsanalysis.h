#ifndef GROMACSANALYSIS_H
#define GROMACSANALYSIS_H

#include <QWidget>
#include <QDir>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class GromacsAnalysis;
}

class GromacsAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit GromacsAnalysis(QWidget *parent = 0);
    ~GromacsAnalysis();

public slots:
    void ReadAnalysis();
    void StopAnalysis(int,QProcess::ExitStatus);

signals:
    void viewGraphSignal(const QString text);

private slots:
    void on_workDir_toolButton_clicked();

    void on_runEnergy_pushButton_clicked();

    void on_energyfile_toolButton_clicked();

    void on_viewGraphEn_pushButton_clicked();

private:
    Ui::GromacsAnalysis *ui;
    QString GromacsAnalysis::selectComponentEnergy();
    QProcess *ProcessAnalysis;
    bool GromacsAnalysis::checkEmptyParameter(QString text, QString label);

};

#endif // GROMACSANALYSIS_H
