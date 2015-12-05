#include "clinicdailyreport.h"
#include "connectsql.h"
extern ConnectSql sql;
ClinicDailyReport::ClinicDailyReport(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.tableWidget->setStyleSheet("QTableWidget{border: 1px solid gray;	background-color: transparent;	selection-color: white;}");
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:white;color: black;padding-left: 4px;border: 1px solid #6c6c6c;};"
		"color: white;padding-left: 4px;border: 1px solid #6c6c6c;}"
		"QHeaderView::section:checked{background-color: white;color: black;}");	
	ui.tableWidget->installEventFilter(this);
	installEventFilter(this);
	initUI();
}
void ClinicDailyReport::initUI()
{
	ui.sheetmakerEdit->setEnabled(false);
	ui.sheetmakerEdit->setText(sql.struser);
	QDateTime current_date_time = QDateTime::currentDateTime();
	ui.makedateTimeEdit->setDateTime(current_date_time);

	ui.startdateTimeEdit->setDateTime(current_date_time);
	QTime time= QTime::fromString("00:00:00", "hh:mm:ss");
	ui.startdateTimeEdit->setTime(time);
	ui.endateTimeEdit->setDateTime(current_date_time);
	time= QTime::fromString("23:59:59", "hh:mm:ss");
	ui.endateTimeEdit->setTime(time);


	ui.discardButton->setEnabled(false);
	ui.queryButton->setEnabled(false);
	ui.saveButton->setEnabled(false);
	ui.deleteButton->setEnabled(false);
	ui.editButton->setEnabled(false);
}
int  ClinicDailyReport::sheetNo()
{
	QSqlQuery query(*sql.db);		
	query.exec("select * from mz_dailysummary");
	int isheetcount=0;
	isheetcount=0;
	while(query.next())
	{
		isheetcount=query.value(0).toInt();
	}
	return isheetcount;
}
void ClinicDailyReport::on_saveButton_clicked()
{
	QSqlQuery query(*sql.db);
	int rows = ui.tableWidget->model()->rowCount();   //行总数

	if ( ui.tableWidget->item(0,0)==NULL) return;
	int isheetcount=sheetNo();
	isheetcount++;
	query.prepare("INSERT INTO mz_dailysummary VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
	query.bindValue(0, isheetcount);
	query.bindValue(1, ui.sheetNoEdit->text());
	query.bindValue(2, ui.makedateTimeEdit->dateTime());
	query.bindValue(3, NULL);//ui.casherEdit->text()
	query.bindValue(4,ui.startdateTimeEdit->dateTime());
	query.bindValue(5, ui.endateTimeEdit->dateTime() );
	query.bindValue(6,"");//
	query.bindValue(7, ui.sheetmakerEdit->text());
	query.bindValue(8, "");//审核
	query.bindValue(9, "");//开票日期
	query.bindValue(10, "");
	query.bindValue(11, ui.tableWidget->item(rows-1,2)->text().toDouble());
	query.bindValue(12, ui.tableWidget->item(rows-1,3)->text().toDouble());
	query.bindValue(13, ui.tableWidget->item(rows-1,4)->text().toDouble());	
	if(query.exec())
	{
		QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("保存成功！"));
		ui.discardButton->setEnabled(false);
		ui.queryButton->setEnabled(false);
		ui.saveButton->setEnabled(false);
		ui.editButton->setEnabled(true);
	}
}
void ClinicDailyReport::on_discardButton_clicked()
{
	ui.sheetNoEdit->setText("");
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->clearContents();
}
void ClinicDailyReport::on_addButton_clicked()
{
	ui.discardButton->setEnabled(true);
	ui.queryButton->setEnabled(true);
	ui.saveButton->setEnabled(true);
	int isheetcount=sheetNo();
	isheetcount++;
	QString strSheetNo= "MZRJ"+QString::number(isheetcount, 10);
	ui.sheetNoEdit->setText(strSheetNo);
}
void ClinicDailyReport::on_queryButton_clicked()
{
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->clearContents();
	QString startDate = ui.startdateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString endDate = ui.endateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	QSqlQuery query(*sql.db);		
	query.exec("select * from mz_chargesheet where date between  TIMESTAMP '"+startDate+"' and TIMESTAMP '"+endDate+"'");
	QStringList list;
	int irowCount=0;
	double totaldueincome=0.0;
	double totalrealincome=0.0;
	double dueincome=0.0;
	double realincome=0.0;
	while(query.next())
	{
		ui.tableWidget->insertRow(irowCount);
		QString str = query.value(1).toString();
		ui.tableWidget->setItem(irowCount,0,new QTableWidgetItem(str));
		str = query.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
		ui.tableWidget->setItem(irowCount,1,new QTableWidgetItem(str));
		str = query.value(13).toString();
		totaldueincome+=query.value(13).toDouble();
		dueincome=query.value(13).toDouble();
		ui.tableWidget->setItem(irowCount,2,new QTableWidgetItem(str));
		str = query.value(14).toString();
		totalrealincome+=query.value(14).toDouble();
		realincome=query.value(14).toDouble();
		ui.tableWidget->setItem(irowCount,3,new QTableWidgetItem(str));
		ui.tableWidget->setItem(irowCount,4,new QTableWidgetItem(QString::number(dueincome-realincome)));
		ui.tableWidget->setItem(irowCount,5,new QTableWidgetItem(query.value(13).toString()));
		irowCount++;
	}
	ui.tableWidget->insertRow(irowCount);
	ui.tableWidget->setItem(irowCount,0,new QTableWidgetItem(QString::fromLocal8Bit("合计")));
	ui.tableWidget->setItem(irowCount,2,new QTableWidgetItem(QString::number(totaldueincome)));
	ui.tableWidget->setItem(irowCount,3,new QTableWidgetItem(QString::number(totalrealincome)));
	ui.tableWidget->setItem(irowCount,4,new QTableWidgetItem(QString::number(totaldueincome-totalrealincome)));
}
void ClinicDailyReport::on_editButton_clicked()
{
	ui.deleteButton->setEnabled(true);
	ui.saveButton->setEnabled(true);
	ui.discardButton->setEnabled(true);
}
void ClinicDailyReport::on_deleteButton_clicked()
{
	QSqlQuery query(*sql.db);		
	QString strSheetNo=ui.sheetNoEdit->text();
	QString strsql = "delete from mz_dailysummary where sheetno ='"+strSheetNo+"'"; 
	query.exec(strsql);
	on_discardButton_clicked();
	ui.saveButton->setEnabled(false);
	ui.editButton->setEnabled(false);
	ui.discardButton->setEnabled(false);
	ui.deleteButton->setEnabled(false);
}
bool ClinicDailyReport::eventFilter(QObject*obj,QEvent*event)
{
	if(obj == this)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent*keyEvent=static_cast<QKeyEvent*>(event);//将事件转化为键盘事件
			if(keyEvent->key() == Qt::Key_Return)
			{
				focusNextChild();
				return true;
			}
		}
	}
}
ClinicDailyReport::~ClinicDailyReport()
{

}
