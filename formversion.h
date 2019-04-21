#ifndef FORMVERSION_H
#define FORMVERSION_H

#include <QWidget>

namespace Ui {
class FormVersion;
}

class FormVersion : public QWidget
{
    Q_OBJECT

public:
    explicit FormVersion(QWidget *parent = 0);
    ~FormVersion();

private:
    Ui::FormVersion *ui;
};

#endif // FORMVERSION_H
