#ifndef DRUGINVENTORY_H
#define DRUGINVENTORY_H

#include <QWidget>
#include "ui_druginventory.h"

class DrugInventory : public QWidget
{
	Q_OBJECT

public:
	DrugInventory(QWidget *parent = 0);
	~DrugInventory();
	void initUI();
	int SheetNo();
	int InventorySheetNo();
	//void printTableWidget(const QTableWidget* view,QString stitile,QPrinter *printer);
	void SetEdit(bool IsEdit);
	int iRow;
private:
	Ui::DrugInventory ui;
public slots:
	void on_FindButton_clicked();
	void on_FindAllButton_clicked();
	void getItem(int row,int column);
	void findbyname(const QString &);
//	void findbymanufacturer(const QString &);
};

#endif // DRUGINVENTORY_H
