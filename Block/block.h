#ifndef BLOCK_H_
#define BLOCK_H_

#include <QString>
#include <QVector>

using namespace std;

class Block
{
protected:
    QString name;

public:
    Block(QString name);
    void setName(QString name);
    QString getName();
    virtual QVector<Block *> *getValue() = 0;
};

#endif /* BLOCK_H_ */
