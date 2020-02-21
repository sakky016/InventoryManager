#ifndef INVENTORY_H
#define INVENTORY_H

#include <inventory_utils.h>
#include <QDialog>
#include <QStandardItemModel>
#include <map>

enum
{
    COLUMN_NAME,
    COLUMN_VARIANT,
    COLUMN_QTY,
    COLUMN_MRP,
    COLUMN_PRICE
};

static QString INVENTORY_FILE = "inventory.txt";
static QStringList INVENTORY_HEADERS = {"Name", "Variant", "Available Qty", "MRP", "Selling Price"};

namespace Ui {
class Inventory;
}

class Inventory : public QDialog
{
    Q_OBJECT

public:
    Inventory(QStandardItemModel* inventory, QWidget *parent);
    ~Inventory();
    void SetupInventory(QStandardItemModel* inventory);

private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnRemove_clicked();

private:
    Ui::Inventory *ui;
    QStandardItemModel* m_inventory;
};

#endif // INVENTORY_H
