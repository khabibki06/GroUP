#ifndef PACKMOLGUI_H
#define PACKMOLGUI_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QProcess>
#include <QRegExpValidator>
#include <QMenu>

namespace Ui {
class packmolgui;
}

class packmolgui : public QWidget
{
    Q_OBJECT

public:
    explicit packmolgui(QWidget *parent = 0);
    ~packmolgui();
    void packmolgui::setPackmol(QString packmol);
public slots:
    void ReadPackmolProcess();
    void contexMenuRequest(const QPoint &pos);

signals:
    //signal for call vmd using mainwindows
    void viewStructureSignal(const QString text);

private slots:
    void on_workdir_toolButton_clicked();

    void on_structure_toolButton_clicked();

    void on_add_pushButton_clicked();

    void on_run_pushButton_clicked();

    void on_view_pushButton_clicked();

    void deleteSelectedMolecule();
    void clearMolecules();



private:
    Ui::packmolgui *ui;
    bool packmolgui::checkEmptyParameter(QString text, QString label);
    QProcess *packmolprocess;
    void packmolgui::addMoleculesToTable(QStringList moleculesData);
    QString packmolExec;
};

#endif // PACKMOLGUI_H
