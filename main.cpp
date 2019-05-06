#include "group.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("GroUP");
    a.setOrganizationDomain("www.khabibkhumaini.com");
    a.setApplicationName("GroUP");

//    QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    GroUP w;
    w.show();

    return a.exec();
}
