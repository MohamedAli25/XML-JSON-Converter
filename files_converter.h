#ifndef FILES_CONVERTER_H_
#define FILES_CONVERTER_H_

#include <QString>
#include <QFile>
#include <QString>
#include <QTextStream>
#include "Block/block.h"
#include "Block/object_block.h"
#include "Block/string_block.h"

using namespace std;

class FilesConverter
{
private:
    QFile *inFileStreamPtr = nullptr;
    QFile *outFileStreamPtr = nullptr;
    QString jsonString;
    QTextStream* out;
    Block *root = nullptr;

    void generateJSONObject(Block *currentBlock);

public:
    FilesConverter(QFile *inFileStreamPtr, QFile *outFileStreamPtr);
    FilesConverter(const QString inFileName, const QString outFileName);
    void setInFileStreamPtr(QFile *inFileStreamPtr);
    QFile *getInFileStreamPtr();
    void setOutFileStreamPtr(QFile *outFileStreamPtr);
    QFile *getOutFileStreamPtr();
    void readXMLFile();
    void readJSONFile();
    void generateXMLFile();
    void generateJSONFile();
    ~FilesConverter();
};

#endif /* FILES_CONVERTER_H_ */
