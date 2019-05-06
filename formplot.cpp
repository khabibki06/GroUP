#include "formplot.h"
#include "ui_formplot.h"

FormPlot::FormPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPlot)
{
    ui->setupUi(this);
     ui->CustomPlot_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |QCP::iSelectLegend | QCP::iSelectPlottables | QCP::iSelectItems);
     ui->CustomPlot_widget->legend->setVisible(true);
     //ful box
     ui->CustomPlot_widget->axisRect()->setupFullAxesBox();
     // setup policy and connect slot for context menu popup:
     ui->CustomPlot_widget->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(ui->CustomPlot_widget,SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(contextMenuRequest(const QPoint &)));
     //set tile plot
     ui->CustomPlot_widget->plotLayout()->insertRow(0);
     title = new QCPTextElement(ui->CustomPlot_widget, "Title", QFont("sans", 17, QFont::Bold));
     ui->CustomPlot_widget->plotLayout()->addElement(0, 0, title);
     connect(title,SIGNAL(doubleClicked(QMouseEvent*)),this,SLOT(titleDoubleClick(QMouseEvent*)));
     //change label by double clicked
     connect(ui->CustomPlot_widget,SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),this, SLOT(axisLabelDoubleClicked(QCPAxis*,QCPAxis::SelectablePart)));
     //change legend item name by double clicked
     connect(ui->CustomPlot_widget,SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)),this,SLOT(legendDoubleClicked(QCPLegend*,QCPAbstractLegendItem*)));
     //legend box cannot be selected
     ui->CustomPlot_widget->legend->setSelectableParts(QCPLegend::spItems);
     //dynamic selection
     connect(ui->CustomPlot_widget,SIGNAL(selectionChangedByUser()),this,SLOT(selectionChange()));
     //zoom and drag behaviour
     connect(ui->CustomPlot_widget,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(mousePress()));
     connect(ui->CustomPlot_widget,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(mouseWheel()));
     // make bottom and left axes transfer their ranges to top and right axes:
     connect(ui->CustomPlot_widget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot_widget->xAxis2, SLOT(setRange(QCPRange)));
     connect(ui->CustomPlot_widget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->CustomPlot_widget->yAxis2, SLOT(setRange(QCPRange)));

}

FormPlot::~FormPlot()
{
    delete ui;
}

void FormPlot::plotData(QString filename)
{
    DataPlot xvg_data;
    if (filename.endsWith("xvg",Qt::CaseInsensitive))
        xvg_data.importXvg(filename);
    else
        xvg_data.importCsv(filename);
    ui->CustomPlot_widget->addGraph();
    int amountGraph = ui->CustomPlot_widget->graphCount();
    ui->CustomPlot_widget->graph(amountGraph - 1)->setData(xvg_data.x_value(), xvg_data.y_value());
    //set axis label and plot title
    if (amountGraph == 1)
    {
        setPlotTitle(xvg_data.title());
        ui->CustomPlot_widget->xAxis->setLabel(xvg_data.label("x"));
        ui->CustomPlot_widget->yAxis->setLabel(xvg_data.label("y"));
    }
    //set color
    QStringList colorlist;
    colorlist << "black" << "red" << "blue" << "green" << "yellow" << "orange" << "violet";
    QPen graphPen;
    if (amountGraph < 8)
        graphPen.setColor(QColor (colorlist.value(amountGraph - 1)));
    else
        graphPen.setColor(QColor ("orange"));
    ui->CustomPlot_widget->graph(amountGraph - 1)->setPen(graphPen);
    //set name of legend items
    ui->CustomPlot_widget->plottable()->setName(filename.section("/",-1,-1));
    //rescale and replot
    ui->CustomPlot_widget->rescaleAxes(true);
    ui->CustomPlot_widget->replot();
}


void FormPlot::setInput(QString inputxvg)
{
    ui->fileXVG_lineEdit->setText(inputxvg);
}

void FormPlot::showEvent(QShowEvent *ev)
{
    if (!ui->fileXVG_lineEdit->text().isEmpty())
        plotData(ui->fileXVG_lineEdit->text());
}

QString FormPlot::saveFileDialog()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("save filenames ... "), QDir::currentPath(),tr("Images File (*.png)"));
    return filename;
}

void FormPlot::on_openInputData_toolButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("open input data file"),QDir::currentPath(),tr("xvg files (*.xvg *.XVG);;csv files(*.csv *.CSV)"));
    if (filename.isEmpty())
        return;
    ui->fileXVG_lineEdit->setText(filename);
    plotData(filename);
}


void FormPlot::removeSelectedGraph()
{
    if(ui->CustomPlot_widget->selectedGraphs().size()>0)
    {
        ui->CustomPlot_widget->removeGraph(ui->CustomPlot_widget->selectedGraphs().first());
    }
    if (ui->CustomPlot_widget->graphCount() == 0)
    {
        title->setText("Title");
        ui->CustomPlot_widget->xAxis->setLabel("x-axis Label");
        ui->CustomPlot_widget->yAxis->setLabel("y-axis Label");
    }
    ui->CustomPlot_widget->replot();
}

void FormPlot::clearAllGraphs()
{
    ui->CustomPlot_widget->clearGraphs();
    ui->fileXVG_lineEdit->clear();
    title->setText("Title");
    ui->CustomPlot_widget->xAxis->setLabel("x-axis Label");
    ui->CustomPlot_widget->yAxis->setLabel("y-axis Label");
    ui->CustomPlot_widget->replot();
}

void FormPlot::contextMenuRequest(const QPoint &pos)
{
    QMenu menu(tr("menu"),this);
    menu.setAttribute(Qt::WA_DeleteOnClose);
    //
    menu.addAction("Add graph", this, SLOT(addGraph()));
    menu.addSeparator();
    if (ui->CustomPlot_widget->xAxis->selectTest(pos,false) >= 0 || ui->CustomPlot_widget->yAxis->selectTest(pos,false) >= 0)
    {
        menu.addAction("Change Label Font",this, SLOT(changeLabelFont()));
        menu.addSeparator();
    }

    if (ui->CustomPlot_widget->selectedGraphs().size()>0)
    {
        menu.addAction("Change Line Color", this, SLOT(changeColor()));
        menu.addAction("Export Selected Graph to CSV",this,SLOT(exportToCSV()));
        menu.addAction("Remove Selected Graph", this, SLOT(removeSelectedGraph()));
        menu.addSeparator();
    }
    if (ui->CustomPlot_widget->graphCount()>0)
    {
        menu.addAction("Export Plot to Image", this, SLOT(saveGraphToImage()));
        menu.addAction("Clear All Graphs", this, SLOT(clearAllGraphs()));
        menu.addSeparator();
    }
    menu.exec(ui->CustomPlot_widget->mapToGlobal(pos));
}
void FormPlot::addGraph()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("open input data file"),QDir::currentPath(),tr("xvg files (*.xvg)"));
    if (filename.isEmpty())
        return;
    plotData(filename);
}

void FormPlot::changeColor()
{
    QColor col = QColorDialog::getColor(Qt::blue,this);
    if (col.isValid())
    {
        QPen graphPen;
        graphPen.setColor(col);
        ui->CustomPlot_widget->selectedGraphs().first()->setPen(graphPen);
        ui->CustomPlot_widget->replot();
    }
}

void FormPlot::exportToCSV()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("save filenames ... "), QDir::currentPath(),tr("CSV file (*.csv)"));
    if (filename.isEmpty())
        return;
    QFile file(filename);
    QStringList datalist;
    int x,y;
    for (int i = 0;i < ui->CustomPlot_widget->selectedGraphs().first()->dataCount();i++)
    {
        x = ui->CustomPlot_widget->selectedGraphs().first()->dataMainKey(i);
        y = ui->CustomPlot_widget->selectedGraphs().first()->dataMainValue(i);
        datalist << QString::number(x) + ";" + QString::number(y);
    }
    if (file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ui->CustomPlot_widget->xAxis->label()<< ";" << ui->CustomPlot_widget->yAxis->label()<<endl;
        out << datalist.join("\n");
        file.close();
    }
}

void FormPlot::saveGraphToImage()
{
    QString filename = saveFileDialog();
    if (filename.isEmpty())
        return;
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        ui->CustomPlot_widget->savePng(filename);
    }

}

void FormPlot::titleDoubleClick(QMouseEvent *event)
{
    Q_UNUSED(event)
    if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
    {
        bool ok;
        QString newTitle = QInputDialog::getText(this, "Input New Plot Title","New Plot Title : ", QLineEdit::Normal, title->text(), &ok);
        if (ok && !newTitle.isEmpty())
        {
            title->setText(newTitle);
            ui->CustomPlot_widget->replot();
        }
    }
}

void FormPlot::setPlotTitle(QString titleText)
{
    if (!titleText.isEmpty())
    {
        title->setText(titleText);
    }

}

void FormPlot::axisLabelDoubleClicked(QCPAxis *axis, QCPAxis::SelectablePart part)
{
    if(part == QCPAxis::spAxisLabel)
    {
        bool ok;
        QString newLabel = QInputDialog::getText(this,"Input New Axis Label", "New Axis Label :",QLineEdit::Normal,axis->label(),&ok);
        if (ok && !newLabel.isEmpty())
        {
            axis->setLabel(newLabel);
            ui->CustomPlot_widget->replot();
        }
    }
}

void FormPlot::changeLabelFont()
{
     ui->CustomPlot_widget->selectedAxes().first()->setLabelFont(QFontDialog::getFont(0, ui->CustomPlot_widget->selectedAxes().first()->labelFont()));
     ui->CustomPlot_widget->replot();
}

//change legend label by double clicked
void FormPlot::legendDoubleClicked(QCPLegend *legend, QCPAbstractLegendItem *item)
{
    Q_UNUSED(legend)
    if (item)
    {
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
        bool ok;
        QString newLegendLabel = QInputDialog::getText(this,"Input New Legend Label", "New Legend Label :",QLineEdit::Normal,plItem->plottable()->name(),&ok);
        if (ok && !newLegendLabel.isEmpty())
        {
            plItem->plottable()->setName(newLegendLabel);
            ui->CustomPlot_widget->replot();
        }

    }

}
//selection change behaviour
 void FormPlot::selectionChange()
 {
     // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
      if (ui->CustomPlot_widget->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->CustomPlot_widget->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          ui->CustomPlot_widget->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->CustomPlot_widget->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        ui->CustomPlot_widget->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->CustomPlot_widget->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }
      // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
      if (ui->CustomPlot_widget->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->CustomPlot_widget->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          ui->CustomPlot_widget->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->CustomPlot_widget->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        ui->CustomPlot_widget->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->CustomPlot_widget->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }
     for (int i = 0;i < ui->CustomPlot_widget->graphCount();i++)
     {
         QCPGraph *graph = ui->CustomPlot_widget->graph(i);
         QCPPlottableLegendItem *item = ui->CustomPlot_widget->legend->itemWithPlottable(graph);
         if (item->selected() || graph->selected())
         {
             item->setSelected(1);
             graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
         }
     }
 }

 //if an axis selected, only allow the direction of the axis to be dragged
 //otherwise, both direction may be dragged
void FormPlot::mousePress()
{
    if(ui->CustomPlot_widget->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->CustomPlot_widget->axisRect()->setRangeDrag(ui->CustomPlot_widget->xAxis->orientation());
    else if (ui->CustomPlot_widget->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->CustomPlot_widget->axisRect()->setRangeDrag(ui->CustomPlot_widget->yAxis->orientation());
    else
        ui->CustomPlot_widget->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void FormPlot::mouseWheel()
{
    if(ui->CustomPlot_widget->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->CustomPlot_widget->axisRect()->setRangeZoom(ui->CustomPlot_widget->xAxis->orientation());
    else if (ui->CustomPlot_widget->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->CustomPlot_widget->axisRect()->setRangeZoom(ui->CustomPlot_widget->yAxis->orientation());
    else
        ui->CustomPlot_widget->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
