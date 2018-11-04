#include "MainWindow.h"

#include <QtGui/QApplication>

#include "ApplicationInfo.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(ApplicationInfo::icon());
    app.setOrganizationName(ApplicationInfo::companyName());
    app.setOrganizationDomain(ApplicationInfo::companyDomain());
    app.setApplicationName(ApplicationInfo::internalName());

    MainWindow mainWindow;
    mainWindow.showMaximized();

    return (app.exec());
}
