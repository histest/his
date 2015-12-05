#include "druginventory.h"
#include <QtGui>
#include "connectsql.h"
#include "clinicdrugfilter.h"
#include "package.h"

static int icount;
extern ConnectSql sql;
DrugInventory::DrugInventory(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initUI();
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setStyleSheet("QTableWidget{border: 1px solid gray;	background-color: transparent;	selection-color: grey;}");
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");	
	ui.tableWidget->installEventFilter(this);//ע���¼�������
	
	connect(ui.tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(getItem(int,int)));
	connect(ui.lineEdit_code,SIGNAL(textChanged(const QString &)),this,SLOT(findbyname(const QString &)));
//	connect(ui.lineEdit_manufacturer,SIGNAL(textChanged(const QString &)),this,SLOT(findbymanufacturer(const QString &)));
}

DrugInventory::~DrugInventory()
{
	
}
void DrugInventory::findbyname(const QString &)
{
	QString strText =  ui.lineEdit_code->text();

	QSqlQuery query(*sql.db);	
	QString strsql= "select * from sys_drugdictionary where abbr='"+strText+"'";//;//where AbbrName = '"+strName+"'
	query.exec(strsql);
	while(query.next())
	{
		ui.lineEdit_name->setText(query.value(1).toString());
	}


}

void DrugInventory::getItem(int row,int column)//�������
{
	 ui.lineEdit_name->setText(ui.tableWidget->item(row,0)->text());
//	 ui.lineEdit_manufacturer->setText(ui.tableWidget->item(row,2)->text());	
}

void DrugInventory::initUI()
{
	iRow=0;  
	QSqlQuery query(*sql.db);		
	QDateTime current_date_time = QDateTime::currentDateTime();

	//ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
//	item(i, colunm)->setFlags(Qt::NoItemFlags);
	//ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //����ѡ�еķ�ʽ

	iRow = 0;
	icount = 0;
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->clearContents();
	//ui.tableWidget->insertRow(0);

}

void DrugInventory::SetEdit(bool IsEdit)
{
	if (IsEdit==true)
	{
		
		ui.tableWidget->setEnabled(true);
		ui.lineEdit_code->setEnabled(true);
		ui.lineEdit_name->setEnabled(true);
		//ui.lineEdit_manufacturer->setEnabled(true);

	}
	else
	{
		ui.lineEdit_code->setEnabled(false);
		ui.lineEdit_name->setEnabled(false);
		ui.tableWidget->setEnabled(false);
		//ui.lineEdit_manufacturer->setEnabled(false);
	}
}


void DrugInventory::on_FindAllButton_clicked()
{
	QSqlQuery query(*sql.db);
	iRow = 0;
	icount = 0;
	int rows = ui.tableWidget->model()->rowCount();   //������
	for (int i=0;i<rows+1;i++)
	{
		ui.tableWidget->removeRow(rows - i);
	}
	QString strsql1= "select * from yk_inventory";//;//where AbbrName = '"+strName+"'
	query.exec(strsql1);
					
	while(query.next())
	{
		int rows = ui.tableWidget->model()->rowCount();   //������
		ui.tableWidget->insertRow(rows);
		iRow=rows;
		ui.tableWidget->setItem(iRow,0,new QTableWidgetItem(query.value(2).toString()));
		ui.tableWidget->setItem(iRow,1,new QTableWidgetItem(query.value(3).toString()));
		ui.tableWidget->setItem(iRow,2,new QTableWidgetItem(query.value(4).toString()));
		ui.tableWidget->setItem(iRow,4,new QTableWidgetItem(query.value(6).toString()));
		ui.tableWidget->setItem(iRow,5,new QTableWidgetItem(query.value(7).toString()));
		ui.tableWidget->setItem(iRow,6,new QTableWidgetItem(query.value(8).toString()));
	}

}

void DrugInventory::on_FindButton_clicked()
{
	QString strName;
	strName = ui.lineEdit_name->text();
	QString strManufacturer;
//	strManufacturer = ui.lineEdit_manufacturer->text();	
	
	ui.tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);

	int rows = ui.tableWidget->model()->rowCount();   //������
	for (int i=0;i<rows+1;i++)
	{
		ui.tableWidget->removeRow(rows - i);
	}

	std::string strStd1= strName.toStdString();
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	 // ���codeΪ0����ʾ�����еĻ�����û��װgb18030�ַ���������һ�������windowsϵͳ�ж���װ����һ�ַ���
	if (code)   
		strStd1= code->fromUnicode(strName).data();

	//std::string strStd2= strManufacturer.toStdString();
	// // ���codeΪ0����ʾ�����еĻ�����û��װgb18030�ַ���������һ�������windowsϵͳ�ж���װ����һ�ַ���
	//if (code)   
	//	strStd2= code->fromUnicode(strManufacturer).data();

	if(ui.radioButton_1->isChecked() && strStd1!="")//����ҩƷ���Ʋ���
	{
		QSqlQuery query(*sql.db);	
		QString strsql1= "select * from yk_inventory where name like '"+strName+"'";//;//where AbbrName = '"+strName+"'
		query.exec(strsql1);
		
		while(query.next())
		{
			int rows = ui.tableWidget->model()->rowCount();   //������
			ui.tableWidget->insertRow(rows);
			iRow=rows;
			ui.tableWidget->setItem(iRow,0,new QTableWidgetItem(query.value(2).toString()));
			ui.tableWidget->setItem(iRow,1,new QTableWidgetItem(query.value(3).toString()));
			ui.tableWidget->setItem(iRow,2,new QTableWidgetItem(query.value(4).toString()));
			ui.tableWidget->setItem(iRow,4,new QTableWidgetItem(query.value(6).toString()));
			ui.tableWidget->setItem(iRow,5,new QTableWidgetItem(query.value(7).toString()));
			ui.tableWidget->setItem(iRow,6,new QTableWidgetItem(query.value(8).toString()));
		}
		ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	}
	//else if(ui.radioButton_2->isChecked()&& strStd2!="")//�����������̲���
	//{
	//	QSqlQuery query(*sql.db);	
	//	QString strsql1= "select * from yk_inventory where manufacturer like '"+strManufacturer+"'";//;//where AbbrName = '"+strName+"'
	//	query.exec(strsql1);
	//				
	//	while(query.next())
	//	{
	//		int rows = ui.tableWidget->model()->rowCount();   //������
	//		ui.tableWidget->insertRow(rows);
	//		iRow=rows;
	//		ui.tableWidget->setItem(iRow,0,new QTableWidgetItem(query.value(2).toString()));
	//		ui.tableWidget->setItem(iRow,1,new QTableWidgetItem(query.value(3).toString()));
	//		ui.tableWidget->setItem(iRow,2,new QTableWidgetItem(query.value(4).toString()));
	//		ui.tableWidget->setItem(iRow,4,new QTableWidgetItem(query.value(6).toString()));
	//		ui.tableWidget->setItem(iRow,5,new QTableWidgetItem(query.value(7).toString()));
	//		ui.tableWidget->setItem(iRow,6,new QTableWidgetItem(query.value(8).toString()));
	//	}
	//	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//}
	else if(!ui.radioButton_1->isChecked())
	{
		QSqlQuery query(*sql.db);	
		QString strsql1= "select * from yk_inventory";//;//where AbbrName = '"+strName+"'
		query.exec(strsql1);
					
		while(query.next())
		{
			int rows = ui.tableWidget->model()->rowCount();   //������
			ui.tableWidget->insertRow(rows);
			iRow=rows;
			ui.tableWidget->setItem(iRow,0,new QTableWidgetItem(query.value(2).toString()));
			ui.tableWidget->setItem(iRow,1,new QTableWidgetItem(query.value(3).toString()));
			ui.tableWidget->setItem(iRow,2,new QTableWidgetItem(query.value(4).toString()));
			ui.tableWidget->setItem(iRow,4,new QTableWidgetItem(query.value(6).toString()));
			ui.tableWidget->setItem(iRow,5,new QTableWidgetItem(query.value(7).toString()));
			ui.tableWidget->setItem(iRow,6,new QTableWidgetItem(query.value(8).toString()));
		}
		ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	}
}