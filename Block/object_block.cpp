#include "object_block.h"

ObjectBlock::ObjectBlock(QString name) : Block{name} {}

QVector<Block *> *ObjectBlock::getValue()
{
    return &blocks;
}

void ObjectBlock::addBlock(Block *blockPtr)
{
    blocks.push_back(blockPtr);
}
