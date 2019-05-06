#ifndef FORMPLOT_H
#define FORMPLOT_H

#include <QWidget>
#include <qcustomplot.h>
#include <QList>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <source/dataplot.h>

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

    void on_openInputData_toolButton_clicked();
    void removeSelectedGraph();
    void clearAllGraphs();
    void addGraph();
    void changeColor();
    void exportToCSV();
    void saveGraphToImage();
    void titleDoubleClick(QMouseEvent *event);
    void axisLabelDoubleClicked(QCPAxis *axis, QCPAxis::SelectablePart part);
    void changeLabelFont();
    void legendDoubleClicked(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChange();
    void mousePress();
    void mouseWheel();

private:
    Ui::FormPlot *ui;
    void FormPlot::plotData(QString filename);
    QString FormPlot::saveFileDialog();
    void FormPlot::setPlotTitle(QString titleText);
    QCPTextElement *title;

};

#endif // FORMPLOT_H
