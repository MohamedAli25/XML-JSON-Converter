#include "string_block.h"

StringBlock::StringBlock(QString name) : Block{name} {}

QVector<Block *> *StringBlock::getValue()
{
    return nullptr;
}
