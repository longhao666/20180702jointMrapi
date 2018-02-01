/*
 * 定义显示曲线
 */

#ifndef SHOWITEM_H
#define SHOWITEM_H

#include <QPen>
#include "showqueue.h"

class ShowItem
{
public:
    bool IsCheck; // Is displayed
    unsigned int Item; // the name of item
    QPen PenForThisItem; // Defines the properties of the brush
    ShowQueue sq; // save the data

    ShowItem(bool isCheck, unsigned int item, QPen penForThisItem, int queueSize);
    ~ShowItem();
};

#endif // SHOWITEM_H
