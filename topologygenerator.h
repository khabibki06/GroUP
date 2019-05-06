#ifndef TOPOLOGYGENERATOR_H
#define TOPOLOGYGENERATOR_H

#include <QWidget>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

namespace Ui {
class TopologyGenerator;
}

class TopologyGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit TopologyGenerator(QWidget *parent = 0);
    ~TopologyGenerator();
    void TopologyGenerator::setGMXLib(QString gmxlib);

protected:
    void showEvent(QShowEvent *ev);

private slots:
    void on_workDir_toolButton_clicked();

    void on_PDB_toolButton_clicked();

    void on_PDB_lineEdit_textChanged(const QString &arg1);

    void on_parameter_toolButton_clicked();

    void on_info_textEdit_textChanged();

    void on_pushButton_clicked();

private:
    Ui::TopologyGenerator *ui;
    void TopologyGenerator::readPDB(QString filename);
    void TopologyGenerator::createFFList();
    bool TopologyGenerator::checkErrorMessage(QString text, QString label);
};

#endif // TOPOLOGYGENERATOR_H
