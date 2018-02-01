#include "showitem.h"

ShowItem::ShowItem(bool isCheck, unsigned int item, QPen penForThisItem, int queueSize) : sq(queueSize)
{
    IsCheck = isCheck;
    Item = item;
    PenForThisItem = penForThisItem;
}

ShowItem::~ShowItem()
{
}
