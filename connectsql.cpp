#include "connectsql.h"
#include <QtGui/QApplication>
#include "QSettings"
ConnectSql::ConnectSql()
{
	strtitle =QString::fromLocal8Bit("HIS");
}

ConnectSql::~ConnectSql()
{
   /* QString DBname=db->connectionName();*/
  // db->close();
   // QSqlDatabase::removeDatabase(DBname);

}

bool ConnectSql::connect(QString  strip)
{
	db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL" ,"psql" ));

	QSettings setting("posgresql.ini",QSettings::IniFormat);//读配置文件
	setting.beginGroup("config");
	db->setHostName(strip);
	db->setDatabaseName(setting.value("databasename").toString());
	db->setUserName(setting.value("username").toString());
	db->setPassword(setting.value("password").toString());
	db->setPort(setting.value("port").toInt());

	if(!db->open())
	{
		QString str = str.fromLocal8Bit("提示");
		QString str2 = str.fromLocal8Bit("连接数据库失败！");
		QMessageBox::information(NULL, str,str2, QMessageBox::Ok);
		return false;
	}
	return true;
}

bool ConnectSql::connect2()
{
	db2=QSqlDatabase::addDatabase("QODBC");  
	/*db2.setDatabaseName(QString("DRIVER={SQL SERVER};"  
	"SERVER=%1;"  
	"DATABASE=%2;"  
	"UID=%3;"  
	"PWD=%4;"
	"PORT=%5;").arg("192.168.1.104")  
	.arg("HIS")  
	.arg("sa")  
	.arg("928920")
	.arg("1433"));*/
	QSettings setting("sqlserver.ini",QSettings::IniFormat);//读配置文件
	setting.beginGroup("config");
	db2.setDatabaseName(setting.value("databasename").toString());
	db2.setUserName(setting.value("username").toString());
	db2.setPassword(setting.value("password").toString());

	if(!db2.open())  
	{  
		QMessageBox::information(NULL, "",db2.lastError().text(), QMessageBox::Ok);
	}
	return true; 
}
bool ConnectSql::disconnect2()
{
	db2.close();
	return true; 
}
void ConnectSql::InputProduct()
{

}
void ConnectSql::AddProduct()
{

}
void ConnectSql::DeleteProduct()
{

}
void ConnectSql::InputOriginalData()
{

}
void ConnectSql::AddOriginalData()
{

}
void ConnectSql::DeleteOriginalData()
{

}
void ConnectSql::InputComponentData()
{

}
void ConnectSql::AddComponentData()
{

}
void ConnectSql::DeleteComponentData()
{

}