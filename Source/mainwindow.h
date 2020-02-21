#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <inventory.h>
#include <inventory_utils.h>
#include <QCompleter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void UpdateModelFromFile(const QString & filename);
    void UpdateAutoCompleter();
    void OpenInventory();
    void SearchItem();
    void SetupInvoiceTable();

private slots:
    void on_actionOpen_Inventory_triggered();

    void on_actionNew_Invoice_triggered();

    void on_pushButton_clicked();

    void on_inpItemSearch_returnPressed();

private:
    Ui::MainWindow *ui;
    Inventory *m_inventoryDialog;
    QCompleter *m_autoCompleter;
    QStandardItemModel *m_inventory;
    QStandardItemModel *m_foundItems;
    QStandardItemModel *m_invoiceItems;
};
#endif // MAINWINDOW_H
