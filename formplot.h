#ifndef FORMPLOT_H
#define FORMPLOT_H

#include <QWidget>
#include <qcustomplot.h>
#include <QList>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QColor>


namespace Ui {
class FormPlot;
}

class FormPlot : public QWidget
{
    Q_OBJECT

public:
    explicit FormPlot(QWidget *parent = 0);
    ~FormPlot();
    void FormPlot::setInput(QString inputxvg);
protected:
    void showEvent(QShowEvent *ev);
public slots:
    void contextMenuRequest(const QPoint &pos);

private slots:
    void on_pushButton_clicked();

    void on_xlabel_lineEdit_editingFinished();

    void on_ylabel_lineEdit_editingFinished();

//    void on_ExportCSV_pushButton_clicked();

    void on_openInputData_toolButton_clicked();

    void on_linecolor_toolButton_clicked();
    void removeSelectedGraph();
    void clearAllGraphs();
    void addGraph();
    void changeColor();


private:
    Ui::FormPlot *ui;
//    void FormPlot::PlotXvg(QFile filename);
    void FormPlot::plotXVG(QString filename);
    QString FormPlot::saveFileDialog();

};

#endif // FORMPLOT_H
