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
    FilesConverter filesConverter(QString("G:\\College\\Projects\\XML to JSON Converter\\text.txt"), QString("G:\\College\\Projects\\XML to JSON Converter\\new.txt"));
    filesConverter.readXMLFile();
    filesConverter.generateJSONFile();
    qDebug() << "Hello World" << endl;

    return a.exec();
}
