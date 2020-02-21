#ifndef INVENTORY_UTILS_H
#define INVENTORY_UTILS_H

#include <QString>

// Structure identifying an item and all relevant details
typedef struct Item_tag
{
    int id;
    QString name;
    QString variant;
    QString mrp;
    QString sellingPrice;
}Item_t;

#endif // INVENTORY_UTILS_H
