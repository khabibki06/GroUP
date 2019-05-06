#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>

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
    QString FormSetting::getGMXExec();
    QString FormSetting::getGMXLib();
    QString FormSetting::getPackmol();
    void FormSetting::loadSettings();

signals:
    QString sendSetting(QString VMDPATH, QString GMXPATH);

private slots:

    void on_VMDPath_toolButton_clicked();

    void on_saveSetting_pushButton_clicked();
    void on_GMXPath_toolButton_clicked();

    void on_Load_pushButton_clicked();

    void on_GMXLIB_toolButton_clicked();

    void on_GMXPath_lineEdit_textChanged(const QString &arg1);

    void on_packmolPath_toolButton_clicked();

private:
    Ui::FormSetting *ui;
    void FormSetting::saveSettings();

};

//void saveSettings(const QString &key, const QVariant &value, const QString &group);
//QVariant loadSettings(const QString &key, const QVariant &defaultValue, const QString &group);

#endif // FORMSETTING_H
