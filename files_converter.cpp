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
        }
        else if (currentLetter == '>')
        {
            temp += '>';
            parts.push_back(temp);
            temp = "";
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
        if (part.mid(0, 2) == "</")
        {
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

void FilesConverter::readJSONFile()
{
}

void FilesConverter::generateXMLFile()
{
}

void FilesConverter::generateJSONFile()
{
    (*out) << "Hello World" << endl;
    (*out) << '{' << endl;
    for (int i = 0; i < root->getValue()->size(); i++)
    {
        generateJSONObject((*(root->getValue()))[i]);
    }
    (*out) << endl
                        << "}" << endl;
    // (*outFileStreamPtr) << "{" << endl;
    // stack<Block *> blocks;
    // blocks.push(root);
    // while (!blocks.empty())
    // {
    //     Block *temp = blocks.top();
    //     blocks.pop();
    //     vector<Block *> *objectBlocks = temp->getValue();
    //     if (objectBlocks == nullptr)
    //     {
    //         (*outFileStreamPtr) << '"' << temp->getName() << '",' << endl;
    //     }
    //     else
    //     {
    //         if (objectBlocks->size() == 1 && ((*objectBlocks)[0]->getValue() == nullptr))
    //         {
    //             (*outFileStreamPtr) << '"' << temp->getName() << ": ";
    //         }
    //     }
    // }
}

void FilesConverter::generateJSONObject(Block *currentBlock)
{
    if (currentBlock->getValue() == nullptr)
    {
        (*out) << '"' << currentBlock->getName() << "\"," << endl;
    }
    else
    {
        if (currentBlock->getValue()->size() == 1 && (*(currentBlock->getValue()))[0]->getValue() == nullptr)
        {
            (*out) << '"' << currentBlock->getName() << "\": ";
            generateJSONObject((*(currentBlock->getValue()))[0]);
        }
        else
        {
            (*out) << '"' << currentBlock->getName() << "\": {" << endl;
            for (int i = 0; i < currentBlock->getValue()->size(); i++)
            {
                generateJSONObject((*(currentBlock->getValue()))[i]);
            }
            (*out) << endl
                                << "}" << endl;
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
