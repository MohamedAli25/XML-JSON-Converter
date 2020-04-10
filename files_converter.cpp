#include "files_converter.h"
#include <stack>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QStack>

FilesConverter::FilesConverter(QFile *inFileStreamPtr, QFile *outFileStreamPtr) : inFileStreamPtr{inFileStreamPtr}, outFileStreamPtr{outFileStreamPtr}, out{new QTextStream(outFileStreamPtr)}
{
}

FilesConverter::FilesConverter(const QString inFileName, const QString outFileName)
{
    outFileStreamPtr = new QFile(outFileName);
    outFileStreamPtr->open(QIODevice::WriteOnly | QIODevice::Text);
    inFileStreamPtr = new QFile(inFileName);
    inFileStreamPtr->open(QIODevice::ReadOnly | QIODevice::Text);
    out = new QTextStream(outFileStreamPtr);
}

void FilesConverter::setInFileStreamPtr(QFile *inFileStreamPtr)
{
    this->inFileStreamPtr = inFileStreamPtr;
}

QFile *FilesConverter::getInFileStreamPtr()
{
    return inFileStreamPtr;
}

void FilesConverter::setOutFileStreamPtr(QFile *outFileStreamPtr)
{
    this->outFileStreamPtr = outFileStreamPtr;
}

QFile *FilesConverter::getOutFileStreamPtr()
{
    return outFileStreamPtr;
}

void FilesConverter::readXMLFile()
{
    QString fileText = "";
    QString temp;
    QTextStream in(inFileStreamPtr);
    while (!in.atEnd()) {
        temp = in.readLine();
        fileText.append(temp);
    }
    // Split the file into parts
    QVector<QString> parts;
    temp = "";
    bool letterExist = false;
    QChar currentLetter;
    unsigned long length = fileText.length();
    for (unsigned long i = 0; i < length; i++)
    {
        currentLetter = fileText.at(i);
        if (currentLetter == '<' && temp != "")
        {
            parts.push_back(temp);
            temp = "<";
            letterExist = false;
        }
        else if (currentLetter == '>')
        {
            temp += '>';
            parts.push_back(temp);
            temp = "";
            letterExist = false;
        }
        else
        {
            if (letterExist || (currentLetter != ' ' && currentLetter != '\t' && currentLetter != '\n'))
            {
                temp += fileText.at(i);
                letterExist = true;
            }
        }
    }
    qDebug() << fileText;
    // Build the tree from the parts
    root = new ObjectBlock("root");
    QStack<Block *> blocksStack;
    blocksStack.push(root);
    QString part;
    for (int i = 0; i < parts.size(); i++)
    {
        part = parts[i];
        if (part.length() == 1) {
            blocksStack.top()->getValue()->push_back(new StringBlock(part));
        } else {
            if (part.mid(0, 2) == "</")
            {
                if (parts[i - 1][0] == '<' && parts[i - 1][1] != '/') {
                    blocksStack.top()->getValue()->push_back(new StringBlock(""));
                }
                blocksStack.pop();
            }
            else if (part[0] == '<')
            {
                Block *temp;
                temp = new ObjectBlock(parts[i].mid(1, parts[i].length() - 2));
                blocksStack.top()->getValue()->push_back(temp);
                blocksStack.push(temp);
            }

            else
            {
                blocksStack.top()->getValue()->push_back(new StringBlock(part));
            }
        }
    }
}

void FilesConverter::readJSONFile()
{
}

void FilesConverter::generateXMLFile()
{
}

void FilesConverter::generateJSONFile()
{
    (*out) << '{' << endl;
    indentCounter = 1;
    for (int i = 0; i < root->getValue()->size(); i++)
    {
        if (i == root->getValue()->size() - 1) {
            generateJSONObject((*(root->getValue()))[i], true);
        } else {
            generateJSONObject((*(root->getValue()))[i], false);
        }
    }
    (*out) << '}' << endl;
}

void FilesConverter::generateJSONObject(Block *currentBlock, bool isLast)
{
    if (currentBlock->getValue() == nullptr)
    {
        (*out) << '"' << currentBlock->getName() << "\"" << (isLast ? "" : ",") << endl;
    }
    else
    {
        if (currentBlock->getValue()->size() == 1 && (*(currentBlock->getValue()))[0]->getValue() == nullptr)
        {
            for (unsigned i = 0; i < indentCounter; i++) {
                (*out) << '\t';
            }
            (*out) << '"' << currentBlock->getName() << "\": ";
            generateJSONObject((*(currentBlock->getValue()))[0], isLast);
        }
        else
        {
            for (unsigned int i = 0; i < indentCounter; i++) {
                (*out) << '\t';
            }
            (*out) << '"' << currentBlock->getName() << "\": {" << endl;
            indentCounter++;
            for (int i = 0; i < currentBlock->getValue()->size(); i++)
            {

                if (i == currentBlock->getValue()->size() - 1) {
                    generateJSONObject((*(currentBlock->getValue()))[i], true);
                } else {
                    generateJSONObject((*(currentBlock->getValue()))[i], false);
                }
            }
            indentCounter--;
            for (unsigned int i = 0; i < indentCounter; i++) {
                (*out) << '\t';
            }
            (*out) << "}" << (isLast ? "" : ",") << endl;
        }
    }
}

FilesConverter::~FilesConverter()
{

    if (inFileStreamPtr != nullptr) {

        inFileStreamPtr->close();
    delete inFileStreamPtr;
    }
    if (outFileStreamPtr != nullptr) {

        outFileStreamPtr->close();
    delete outFileStreamPtr;}
    if (out != nullptr)
        delete out;

}
