#include "myphotoshop.h"
#include <QApplication>
#include <QFileInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPhotoShop w;

    //如果用户有自己的css文件，那么调用用户的css文件，否则用我们的已有资源
    QFileInfo Css_File("./CSS/SetBackGround.css");
    if(Css_File.exists())
    {
        QFile qss("./CSS/SetBackGround.css");
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
    else
    {
        QFile qss(":/CSS/BG_CSS");
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
    w.show();

    return a.exec();
}
