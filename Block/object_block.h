#ifndef OBJECT_BLOCK_H_
#define OBJECT_BLOCK_H_

#include "block.h"
#include <QString>
#include <QVector>

class ObjectBlock : public Block
{
private:
    QVector<Block *> blocks;

public:
    ObjectBlock(QString name);
    virtual QVector<Block *> *getValue();
    void addBlock(Block *blockPtr);
};

#endif /* OBJECT_BLOCK_H_ */
