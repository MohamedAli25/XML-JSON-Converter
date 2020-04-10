#include <QCoreApplication>
#include <iostream>
#include <QString>
#include <QDebug>
#include "files_converter.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Hello World" << endl;
    FilesConverter filesConverter(QString("text.txt"), QString("new.txt"));
    filesConverter.readXMLFile();
    filesConverter.generateJSONFile();
//    qDebug() << "Hello World" << endl;

    return a.exec();
}
