#include "inventory.h"
#include "ui_inventory.h"
#include <Qdebug>
#include <Qfile>
#include <QMessageBox>

Inventory::Inventory(QStandardItemModel* inventory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inventory)
{
    ui->setupUi(this);
    SetupInventory(inventory);
}


Inventory::~Inventory()
{
    delete ui;
}

void Inventory::SetupInventory(QStandardItemModel* inventory)
{
    m_inventory = inventory;
    m_inventory->setHorizontalHeaderLabels(INVENTORY_HEADERS);
    ui->tableView->setModel(m_inventory);
}

//---------------------------------------------------------------------------------------------
// @name                : On button clicked: Add
//
// @description         : Adds a new empty entry in which user can fill in details.
//---------------------------------------------------------------------------------------------
void Inventory::on_btnAdd_clicked()
{
    QList<QStandardItem *> items;
    items.append(new QStandardItem(""));
    items.append(new QStandardItem(""));
    items.append(new QStandardItem(""));
    items.append(new QStandardItem(""));

    m_inventory->appendRow(items);
}


//---------------------------------------------------------------------------------------------
// @name                : On button clicked: Save
//
// @description         : Save inventory details to disk
//---------------------------------------------------------------------------------------------
void Inventory::on_btnSave_clicked()
{
    QFile file(INVENTORY_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream fileOutStream(&file);
        int numColumns = INVENTORY_HEADERS.size();
        for (int row = 0; row < m_inventory->rowCount(); row++)
        {
            for (int col = 0; col < numColumns; col++)
            {
                QString item = m_inventory->item(row, col)->text();
                if (item.size())
                {
                    fileOutStream << item.toUtf8() << "\n";
                }
                else
                {
                    fileOutStream << "-\n";
                }
            }
        }

        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("Inventory Update");
        msg.setText("Inventory file updated with " + QString::number(m_inventory->rowCount()) + " entries");
        msg.exec();
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Inventory Update");
        msg.setText("Error in updating inventory file");
        msg.exec();
    }
}

//---------------------------------------------------------------------------------------------
// @name                : On button clicked: Remove
//
// @description         : Remove selected rows
//---------------------------------------------------------------------------------------------
void Inventory::on_btnRemove_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.size())
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle("Remove item");
        msg.setText("You are about to delete " + QString::number(selectedRows.size()) + " entrie(s). Continue?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        int result = msg.exec();

        if (result == QMessageBox::Yes)
        {
            // QPersistanceModelIndex class keeps valid state of indices even though
            // it has changed on removal of any row.
            QList<QPersistentModelIndex> rowsToRemove;
            foreach(const QModelIndex & selectedRow, selectedRows)
            {
                rowsToRemove << selectedRow;
            }

            foreach (const QPersistentModelIndex &i, rowsToRemove)
            {
                m_inventory->removeRow(i.row());
            }
        }
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("Remove item");
        msg.setText("No row selected");
        msg.exec();
    }
}
