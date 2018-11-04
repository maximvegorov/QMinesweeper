#include "ApplicationInfo.h"

QIcon ApplicationInfo::icon()
{
    QIcon result(":/res/icons/MainIcon/MainIcon16x16.png");
    result.addFile(":/res/icons/MainIcon/MainIcon24x24.png");
    result.addFile(":/res/icons/MainIcon/MainIcon32x32.png");
    result.addFile(":/res/icons/MainIcon/MainIcon48x48.png");
    return (result);
}

QString ApplicationInfo::displayName()
{ return (QObject::tr("QMinesweeper", "ApplicationInfo")); }

QString ApplicationInfo::internalName()
{ return ("QMinesweeper"); }

QString ApplicationInfo::companyName()
{ return ("Lab19"); }

QString ApplicationInfo::companyDomain()
{ return ("lab19.ru"); }

QString ApplicationInfo::version()
{  return ("1.0.0");  }

QString ApplicationInfo::copyrights()
{ return (QObject::tr("(C) Maxim V Egorov 2011", "ApplicationInfo")); }
