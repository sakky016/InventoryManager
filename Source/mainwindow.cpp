#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Qdebug>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_autoCompleter = nullptr;
    m_inventory = new QStandardItemModel(this);
    UpdateModelFromFile(INVENTORY_FILE);
    UpdateAutoCompleter();
    ui->inpItemSearch->setFocus();

    SetupInvoiceTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupInvoiceTable()
{
    m_invoiceItems = new QStandardItemModel(this);
    m_invoiceItems->setHorizontalHeaderLabels({"Item", "MRP", "Price"});
    ui->opInvoiceTable->setModel(m_invoiceItems);
    ui->undoView->setModel(m_inventory);
}

//---------------------------------------------------------------------------------------------
// @name                : UpdateModelFromFile
//
// @description         : Fetches inventory details from specified file and updates the
//                        inventory model.
//---------------------------------------------------------------------------------------------
void MainWindow::UpdateModelFromFile(const QString & filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream fileStream(&file);
        int numColumns = INVENTORY_HEADERS.size();
        QString line;

        do
        {
            QList<QStandardItem *> items;
            for (int col = 0; col < numColumns; col++)
            {
                line = fileStream.readLine();
                if (line.size())
                {
                    items.append(new QStandardItem(line));
                }
            }

            if (items.size())
            {
                m_inventory->appendRow(items);
            }
        }while (!line.isNull()); // End of file

        ui->statusBar->showMessage("Loaded " + QString::number(m_inventory->rowCount()) + " entries from Inventory file");
    }
    else
    {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("Inventory load");
        msg.setText("Inventory file not present");
        msg.exec();
    }
}

//---------------------------------------------------------------------------------------------
// @name                : UpdateAutoCompleter
//
// @description         : Update autocompleter as per the inventory
//---------------------------------------------------------------------------------------------
void MainWindow::UpdateAutoCompleter()
{
    if (m_autoCompleter)
    {
        delete m_autoCompleter;
    }

    m_autoCompleter = new QCompleter(m_inventory);
    m_autoCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->inpItemSearch->setCompleter(m_autoCompleter);
}

//---------------------------------------------------------------------------------------------
// @name                : OnOpenInventory
//
// @description         : Launches a modal dialog which allows user to view and edit the inventory.
//---------------------------------------------------------------------------------------------
void MainWindow::OpenInventory()
{
    int oldNumEntries = m_inventory->rowCount();
    m_inventoryDialog = new Inventory(m_inventory, this);
    m_inventoryDialog->setWindowTitle("Inventory");
    m_inventoryDialog->setWindowModality(Qt::ApplicationModal);
    m_inventoryDialog->exec();

    int newNumEntries = m_inventory->rowCount();
    if (newNumEntries != oldNumEntries)
    {
        ui->statusBar->showMessage("Items were added/deleted from the inventory");
    }
    else
    {
        ui->statusBar->clearMessage();
    }
}

//---------------------------------------------------------------------------------------------
// @name                : SearchItem
//
// @description         : Look for specified item in the inventory
//---------------------------------------------------------------------------------------------
void MainWindow::SearchItem()
{
    QString item = ui->inpItemSearch->text();
    if (!item.isEmpty())
    {
        QList<QStandardItem *> foundItems = m_inventory->findItems(item, Qt::MatchWildcard); // Look in 1st column (Name)
        if (foundItems.empty())
        {
            ui->statusBar->showMessage("No item found in inventory");
        }
        else
        {
            // Only 1 item is processed
            QModelIndex modelIndex = m_inventory->indexFromItem(foundItems.back());
            QList<QStandardItem*> items;

            QStandardItem* item = new QStandardItem(m_inventory->item(modelIndex.row(), COLUMN_NAME)->text());
            items.append(item);

            item = new QStandardItem(m_inventory->item(modelIndex.row(), COLUMN_MRP)->text());
            items.append(item);

            item = new QStandardItem(m_inventory->item(modelIndex.row(), COLUMN_PRICE)->text());
            items.append(item);

            m_invoiceItems->appendRow(items);
            ui->statusBar->showMessage("Found [" + QString::number(foundItems.size()) + "] item(s)");
        }
    }
}

//---------------------------------------------------------------------------------------------
// On menu button clicked: Open Inventory
//---------------------------------------------------------------------------------------------
void MainWindow::on_actionOpen_Inventory_triggered()
{
    OpenInventory();
}

//---------------------------------------------------------------------------------------------
// On menu button clicked: New invoice
//---------------------------------------------------------------------------------------------
void MainWindow::on_actionNew_Invoice_triggered()
{

}

//---------------------------------------------------------------------------------------------
// On menu button clicked: Add
//---------------------------------------------------------------------------------------------
void MainWindow::on_pushButton_clicked()
{
    SearchItem();
    ui->inpItemSearch->setFocus();
    ui->inpItemSearch->clear();
}

//---------------------------------------------------------------------------------------------
// On keypress: Enter
//---------------------------------------------------------------------------------------------
void MainWindow::on_inpItemSearch_returnPressed()
{
    SearchItem();
    //ui->inpItemSearch->setFocus();
    ui->inpItemSearch->clear();
}

