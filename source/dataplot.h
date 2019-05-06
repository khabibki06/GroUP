#ifndef DATAPLOT_H
#define DATAPLOT_H

#include <QObject>
#include <QVector>
#include <QDir>
#include <QTextStream>
#include <QMultiMap>

class DataPlot : public QObject
{
    Q_OBJECT
public:
    explicit DataPlot(QObject *parent = 0);
    void DataPlot::setx(QVector <double> value);
    void DataPlot::sety(QVector <double> value);
    void DataPlot::appendX(double x_value);
    void DataPlot::appendY(double y_value);
    void DataPlot::setlabelx(QString label);
    void DataPlot::setlabely(QString label);
    void DataPlot::setTitle (QString titletext);
    void DataPlot::setlegend(QString label);
    QVector <double> DataPlot::x_value();
    QVector <double> DataPlot::y_value();
    QString DataPlot::title();
    QString DataPlot::label(QString axis);
    QString DataPlot::legend(QString name);
    void DataPlot::importXvg(QString filepath);
    void DataPlot::importCsv(QString filepath);
    QVector <double>  DataPlot::averageData(QVector <double> dataset);

signals:

public slots:

private:
    QString label_x, label_y, graphtitle, legendname;
//    QVector <double> x;
//    QVector <double> y;
    QMultiMap <QString, double> dataframe;

};

#endif // DATAPLOT_H
