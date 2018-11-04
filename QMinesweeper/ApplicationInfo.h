#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include <QtCore/QString>
#include <QtGui/QIcon>

class ApplicationInfo
{
public:
    static QIcon icon();
    static QString displayName();
    static QString internalName();
    static QString companyName();
    static QString companyDomain();
    static QString version();
    static QString copyrights();

};

#endif // APPLICATIONINFO_H
