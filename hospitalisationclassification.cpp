#include "hospitalisationclassification.h"
#include "connectsql.h"
#include <QTreeWidgetItem>
#include <QTextCodec>
extern ConnectSql sql;
Hospitalisationclassification::Hospitalisationclassification(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.treeWidget->setColumnCount(1);

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setStyleSheet("QTableWidget{border: 1px solid gray;	background-color: transparent;	selection-color: grey;}");
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");	
	ui.treeWidget->setStyleSheet("QTreeWidget{border: 1px solid gray;color: black;	background-color: white;selection-color: grey;}");
	//connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(showTable(QTreeWidgetItem*,int)));
	initUI();
}
void Hospitalisationclassification::initUI()
{
	ui.treeWidget->clear();
	ui.tableWidget->setRowCount(0);
	QStringList headers;
	headers << QString::fromLocal8Bit("住院分类");
	ui.treeWidget->setHeaderLabels(headers);

	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

	QString strsql;
	strsql = "select * from zy_classification order by id";
	QSqlQuery query(*sql.db);	
	query.exec(strsql);
	int row = 0;
	while(query.next())
	{	
		QStringList strlist;
		strlist <<query.value(2).toString();
		QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget, strlist);
		ui.treeWidget->addTopLevelItem(item);
		ui.tableWidget->insertRow(row);
		ui.tableWidget->setItem(row,0,new QTableWidgetItem(query.value(0).toString()));
		ui.tableWidget->setItem(row,1,new QTableWidgetItem(query.value(1).toString()));
		ui.tableWidget->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));
		ui.tableWidget->setItem(row,3,new QTableWidgetItem(query.value(3).toString()));
		ui.tableWidget->setItem(row,4,new QTableWidgetItem(query.value(4).toString()));
		row++;
	}
	ui.treeWidget->expandAll();
	ui.saveButton->setEnabled(false);
	ui.deleteButton->setEnabled(false);
}
void Hospitalisationclassification::showTable(QTreeWidgetItem*item, int column)
{
	ui.tableWidget->setRowCount(0);
	QString strText = item->text(0);
	QString strsql;
	strsql = "select * from zy_classification where name  = '"+strText+"'  order by id";
	QSqlQuery query(*sql.db);	
	query.exec(strsql);
	int row = 0;
	while(query.next())
	{	
		ui.tableWidget->insertRow(row);
		ui.tableWidget->setItem(row,0,new QTableWidgetItem(query.value(0).toString()));
		ui.tableWidget->setItem(row,1,new QTableWidgetItem(query.value(1).toString()));
		ui.tableWidget->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));
		ui.tableWidget->setItem(row,3,new QTableWidgetItem(query.value(3).toString()));
		ui.tableWidget->setItem(row,4,new QTableWidgetItem(query.value(4).toString()));
		row++;
	}
}
void Hospitalisationclassification::on_addButton_clicked()
{
	initUI();
	ui.saveButton->setEnabled(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);
	ui.tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(row+1)));
}
void Hospitalisationclassification::on_editButton_clicked()
{
	ui.tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
	ui.saveButton->setEnabled(true);
	ui.deleteButton->setEnabled(true);
}
void Hospitalisationclassification::on_saveButton_clicked()
{
	QSqlQuery query(*sql.db);		
	QString strsql = "delete from zy_classification"; 
	query.exec(strsql);
	int row = ui.tableWidget->model()->rowCount();
	for (int i= 0;i<row;i++)
	{
		query.prepare("INSERT INTO zy_classification VALUES (?,?, ?, ?, ?)");
		query.bindValue(0, ui.tableWidget->item(i,0)->text().toInt());
		for (int j=1;j<5;j++)
		{
			if (ui.tableWidget->item(i,j)==NULL) 
				query.bindValue(j, NULL);
			else
				query.bindValue(j, ui.tableWidget->item(i,j)->text());
		}
// 		query.bindValue(1, ui.tableWidget->item(i,1)->text());
// 		query.bindValue(2, ui.tableWidget->item(i,2)->text());
// 		query.bindValue(3, ui.tableWidget->item(i,3)->text());
// 		query.bindValue(4, ui.tableWidget->item(i,4)->text());
		query.exec();
	}
	initUI();
	ui.saveButton->setEnabled(false);
	ui.editButton->setEnabled(true);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void Hospitalisationclassification::on_deleteButton_clicked()
{

	QMessageBox box(QMessageBox::Warning,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("确认删除？"));
	box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
	box.setButtonText (QMessageBox::Ok,QString::fromLocal8Bit("确 定"));
	box.setButtonText (QMessageBox::Cancel,QString::fromLocal8Bit("取 消"));
	if(box.exec()==QMessageBox::Ok)
	{
		QList<QTableWidgetItem*> list =  ui.tableWidget->selectedItems();
		if (list.at(0)==NULL)
		{
			return;
		}
		QString strName = ui.tableWidget->item(list.at(0)->row(),2)->text();
		if (strName==NULL)
		{
			return;
		}
		QSqlQuery query(*sql.db);
		query.exec("delete  from zy_classification where name = '"+strName+"'");
		initUI();
	}
}
void Hospitalisationclassification::on_exitButton_clicked()
{

	QMessageBox box(QMessageBox::Warning,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("确认退出？"));
	box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
	box.setButtonText (QMessageBox::Ok,QString::fromLocal8Bit("确 定"));
	box.setButtonText (QMessageBox::Cancel,QString::fromLocal8Bit("取 消"));
	if(box.exec()==QMessageBox::Ok)
	{
		this->close();
	}
}

Hospitalisationclassification::~Hospitalisationclassification()
{

}
