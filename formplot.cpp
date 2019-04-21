#include "formplot.h"
#include "ui_formplot.h"

FormPlot::FormPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPlot)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Drawer);
     ui->linecolor_toolButton->setStyleSheet("background = blue");
     ui->CustomPlot_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |QCP::iSelectLegend | QCP::iSelectPlottables | QCP::iSelectItems);
     ui->CustomPlot_widget->legend->setVisible(true);
     // setup policy and connect slot for context menu popup:
     ui->CustomPlot_widget->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(ui->CustomPlot_widget,SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(contextMenuRequest(const QPoint &)));
}

FormPlot::~FormPlot()
{
    delete ui;
}

class XVG
{
public:
    QVector <double> x, y;
    QString label_x, label_y, legendname;
    void setx(QVector <double> value)
    {
        x << value;
    }
    void sety(QVector <double> value)
    {
        y << value;
    }
    void setlabelx(QString label)
    {
        label_x = label;
    }
    void setlabely(QString label)
    {
        label_y = label;
    }
    void setlegend(QString label)
    {
        QString legend = label;
    }

    QVector <double> x_value()
    {
        return x;
    }
    QVector <double> y_value()
    {
        return y;
    }
    QString label(QString axis)
    {
        if (axis == "x")
            return label_x;
        else if (axis == "y")
            return label_y;
        else
            return "";
    }
    QString legend(QString nama)
    {
        return legendname;
    }
};

void FormPlot::plotXVG(QString filename)
{
    QFile file(filename);
    XVG xvg_data;
    if (!file.open(QIODevice::ReadOnly))
        return;
    QTextStream in(&file);
    QString line;
    QStringList fieldX, fieldY;
    int index;
    while (!in.atEnd())
    {
        line = in.readLine();
        if (line.startsWith("#"))
        {

        } else if (line.startsWith("@"))
        {
            if (line.contains("xaxis  label"))
//                xlab = line.split("\"").at(1);
                xvg_data.setlabelx(line.split("\"").at(1));

            if (line.contains("yaxis  label"))
//                ylab = line.split("\"").at(1) ;
                xvg_data.setlabely(line.split("\"").at(1));

        }
        else
        {
            index = line.indexOf(".") + 9 ;
            fieldX << line.left(index).replace(" ", "");
            fieldY << line.right(index).replace(" ","");

        }
    }
    file.close();
//    QVector <double> x;
//    QVector <double> y;
    int dimmension = fieldX.count();
    for (int i = 0; i < dimmension; i++)
    {
//        x << fieldX.value(i).toDouble();
        xvg_data.x << fieldX.value(i).toDouble();
    }
    for (int i = 0; i < dimmension; i++)
    {
//        y << fieldY.value(i).toDouble();
        xvg_data.y << fieldY.value(i).toDouble();
    }

//    ui->CustomPlot_widget->setInteraction(QCP::iRangeZoom, true);
//    ui->CustomPlot_widget->setInteraction(QCP::iRangeDrag, true);
//    ui->CustomPlot_widget->setInteraction(QCP::iSelectPlottables, true);
    QStringList colorlist;
    colorlist << "black" << "red" << "blue" << "green" << "yellow" << "orange" << "violet";
    ui->CustomPlot_widget->addGraph();
    int amountGraph = ui->CustomPlot_widget->graphCount();
    ui->CustomPlot_widget->graph(amountGraph - 1)->setData(xvg_data.x_value(), xvg_data.y_value());
    QPen graphPen;
    if (amountGraph == 1)
    {
        ui->CustomPlot_widget->xAxis->setLabel(xvg_data.label("x"));
        ui->CustomPlot_widget->yAxis->setLabel(xvg_data.label("y"));
    }
    graphPen.setColor(QColor (colorlist.value(amountGraph - 1)));
    ui->CustomPlot_widget->graph(amountGraph - 1)->setPen(graphPen);
    ui->CustomPlot_widget->rescaleAxes(true);
    ui->CustomPlot_widget->replot();
    ui->xlabel_lineEdit->setText(xvg_data.label("x"));
    ui->ylabel_lineEdit->setText(xvg_data.label("y"));
}

void FormPlot::setInput(QString inputxvg)
{
    ui->fileXVG_lineEdit->setText(inputxvg);
}

void FormPlot::showEvent(QShowEvent *ev)
{
    plotXVG(ui->fileXVG_lineEdit->text());
}

QString FormPlot::saveFileDialog()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("save filenames ... "), QDir::currentPath(),tr("Images File (*.png)"));
    return filename;
}




void FormPlot::on_pushButton_clicked()
{
//    QString filetype = "Images File (*.png)";
    QString filename = saveFileDialog();
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {

    }
    else
    {
        ui->CustomPlot_widget->savePng(filename);
    }
}

void FormPlot::on_xlabel_lineEdit_editingFinished()
{
    ui->CustomPlot_widget->xAxis->setLabel(ui->xlabel_lineEdit->text());
    ui->CustomPlot_widget->replot();
}

void FormPlot::on_ylabel_lineEdit_editingFinished()
{
    ui->CustomPlot_widget->yAxis->setLabel(ui->ylabel_lineEdit->text());
    ui->CustomPlot_widget->replot();
}

//void FormPlot::on_ExportCSV_pushButton_clicked()
//{
//    QString filetype = "CSV file (*.csv)";
//    QString filename = saveFileDialog(filetype);
//    QFile file(filename);
//    if (file.open(QFile::WriteOnly | QIODevice::Text))
//    {
//        QTextStream out(&file);

//    }

//}

void FormPlot::on_openInputData_toolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("open input data file"),QDir::currentPath(),tr("xvg files (*.xvg)"));
    ui->fileXVG_lineEdit->setText(filename);
    plotXVG(filename);
}

void FormPlot::on_linecolor_toolButton_clicked()
{
    ui->linecolor_toolButton->setAutoFillBackground(true);
    QColor col = QColorDialog::getColor(Qt::blue);
    ui->linecolor_toolButton->setStyleSheet("background: " + col.name());
    ui->CustomPlot_widget->selectedGraphs().first()->setPen(QPen(col));
    ui->CustomPlot_widget->replot();
}
void FormPlot::removeSelectedGraph()
{
    if(ui->CustomPlot_widget->selectedGraphs().size()>0)
    {
        ui->CustomPlot_widget->removeGraph(ui->CustomPlot_widget->selectedGraphs().first());
        ui->CustomPlot_widget->replot();
    }
}

void FormPlot::clearAllGraphs()
{
    ui->CustomPlot_widget->clearGraphs();
    ui->CustomPlot_widget->replot();
    ui->fileXVG_lineEdit->clear();
}

void FormPlot::contextMenuRequest(const QPoint &pos)
{
    QMenu menu(tr("menu"),this);
    menu.setAttribute(Qt::WA_DeleteOnClose);
    menu.addAction("Add graph", this, SLOT(addGraph()));
    menu.addSeparator();
    if (ui->CustomPlot_widget->selectedGraphs().size()>0)
    {
        menu.addAction("Change Line Color", this, SLOT(changeColor()));
        menu.addAction("Remove Selected Graph", this, SLOT(removeSelectedGraph()));
        menu.addSeparator();
    }
    if (ui->CustomPlot_widget->graphCount()>0)
            menu.addAction("Clear All Graphs", this, SLOT(clearAllGraphs()));
    menu.exec(ui->CustomPlot_widget->mapToGlobal(pos));
}
void FormPlot::addGraph()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("open input data file"),QDir::currentPath(),tr("xvg files (*.xvg)"));
    plotXVG(filename);
}

void FormPlot::changeColor()
{
    QColor col = QColorDialog::getColor();
    QPen graphPen;
    graphPen.setColor(col);
    ui->CustomPlot_widget->selectedGraphs().first()->setPen(graphPen);
    ui->CustomPlot_widget->replot();
}
