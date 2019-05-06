#include "dataplot.h"

DataPlot::DataPlot(QObject *parent) : QObject(parent)
{
    label_x = "x axis label";
    label_y = "y axis label";
    graphtitle = "Title";
}

void DataPlot::setx(QVector <double> value)
{
//    x = value;
    for (int i = 0;i<value.size();i++)
        dataframe.insert("x",value.value(i));
}
void DataPlot::sety(QVector <double> value)
{
//    y = value;
    for (int i = 0;i<value.size();i++)
        dataframe.insert("y",value.value(i));
}
void DataPlot::setlabelx(QString label)
{
    label_x = label;
}
void DataPlot::setlabely(QString label)
{
    label_y = label;
}
void DataPlot::appendX(double x_value)
{
//    x.append(x_value);
    dataframe.insert("x",x_value);
}
void DataPlot::appendY(double y_value)
{
//    y.append(y_value);
    dataframe.insert("y",y_value);
}
void DataPlot::setTitle (QString titletext)
{
    graphtitle = titletext;
}
void DataPlot::setlegend(QString label)
{
    QString legend = label;
}

QVector <double> DataPlot::x_value()
{
//    return x;
    return dataframe.values("x").toVector();
}
QVector <double> DataPlot::y_value()
{
//    return y;
    return dataframe.values("y").toVector();
}
QString DataPlot::title()
{
    return graphtitle;
}

QString DataPlot::label(QString axis)
{
    if (axis == "x")
        return label_x;
    else if (axis == "y")
        return label_y;
    else
        return "";
}
QString DataPlot::legend(QString name)
{
    return legendname;
}

//import xvg data plot
void DataPlot::importXvg(QString filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    QString line;
    int index;
    QStringList lineList;
    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.startsWith("#"))
        {

        } else if (line.startsWith("@"))
        {
            if (line.contains("title"))
                setTitle(line.split("\"").at(1));
            if (line.contains("xaxis  label"))
                setlabelx(line.split("\"").at(1));

            if (line.contains("yaxis  label"))
                setlabely(line.split("\"").at(1));

        }
        else
        {
            index = line.indexOf(".") + 9 ;
            lineList = line.split(" ", QString::SkipEmptyParts);
            appendX(lineList.value(0).toDouble());
            appendY(lineList.value(1).toDouble());

        }
    }
    file.close();
}

//import CSV file
void DataPlot::importCsv(QString filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    QString line;
    double x_val,y_val;
    int pos;
    bool ok1,ok2;
    while (!in.atEnd())
    {
        line = in.readLine();
        pos = line.indexOf(";");
        x_val  = line.left(pos).replace(" ", "").toDouble(&ok1);
        y_val =   line.right(pos).replace(" ", "").toDouble(&ok2);
        if (ok1 && ok2)
        {
            appendX(x_val);
            appendY(y_val);
        }
    }
    file.close();
}

//avergae data
QVector <double>  DataPlot::averageData(QVector <double> dataset)
{
    QVector <double> average;
    //average of first value is same
    average.append(dataset.value(0));
    //avergae of two next value
    for (int i = 1;i<dataset.size();i++)
        average.append((dataset.value(i)+dataset.value(i-1))/2);
    return average;
}
