#include "QtGis.h"
#include <QtWidgets/QApplication>
#include "qgsapplication.h"
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //可执行程序的当前目录
    QString appDir = QCoreApplication::applicationDirPath();
    //设置QSetting环境
    QCoreApplication::setOrganizationName("QtQgis");
    QCoreApplication::setApplicationName("QtQgis Application");

    //初始化QGIS环境
    QgsApplication::init(appDir);
    QgsApplication::initQgis();
    QgsApplication::setMaxThreads(8);

    //// 设置全局字体为支持中文的字体
    //QFont font("Microsoft YaHei"); // 或者其他支持中文的字体
    //QApplication::setFont(font);

    QtGis w;
    w.showMaximized();
    return a.exec();
}
