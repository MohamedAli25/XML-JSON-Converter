#include "block.h"

Block::Block(QString name) : name{name} {}

void Block::setName(QString name)
{
    this->name = name;
}

QString Block::getName()
{
    return name;
}
