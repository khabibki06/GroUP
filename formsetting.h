#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class FormSetting;
}

class FormSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormSetting(QWidget *parent = 0);
    ~FormSetting();
    QString FormSetting::getVMDPath();

private slots:

    void on_VMDPath_toolButton_clicked();

private:
    Ui::FormSetting *ui;
};

#endif // FORMSETTING_H
