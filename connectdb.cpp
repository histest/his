#include "connectdb.h"
#include "ui_connectdb.h"
#include "connectsql.h"
ConnectSql sql;
ConnectDB::ConnectDB(QWidget *parent): 
QDialog(parent),
	ui(new Ui::ConnectDB)
{
	ui->setupUi(this);
	initUI();
	ui->widget->setStyleSheet("QWidget{background-color: rgb(85,170,255);}"
		"QPushButton{background-color: rgb(85,170,255);color: white;border-style: outset;}"
		"QPushButton:hover{	background-color: rgb(3,110,184);color: white;}");
	ui->label->setStyleSheet("QLabel{ background-color: transparent;color: white;qproperty-alignment: AlignCenter;border-color:rgb(27, 118, 150);border-width: 1px;border-radius: 5px;}");
}
void ConnectDB::initUI()
{
	setWindowFlags(Qt::FramelessWindowHint);
	QString strfile;
	strfile = QCoreApplication::applicationDirPath();
	QString sDbNm = strfile+"/DACS.accdb";  
	QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access};DBQ=%1;").arg(sDbNm);//连接字符串  
	IsAdmin=false;
	QSettings setting("posgresql.ini",QSettings::IniFormat);//读配置文件
	setting.beginGroup("config");
	ui->lineEdit->setText(setting.value("hostname").toString());
	Isconnect =on_okButton_clicked();
}
ConnectDB::~ConnectDB()
{
	delete ui;
}
bool ConnectDB::on_okButton_clicked()
{

	bool isconnect =sql.connect(ui->lineEdit->text().trimmed());
	return isconnect;

}
void ConnectDB::on_loginButton_clicked()
{

	/*bool isconnect = on_okButton_clicked();
	if (!isconnect) return;*/
	if (!Isconnect)
	{
		Isconnect =on_okButton_clicked();
	}
	if (!Isconnect) return;
	QString user = ui->comboBox_2->currentText().trimmed();
	QString pwd =ui->lineEdit_2->text().trimmed();
	QString str = str.fromLocal8Bit("警告");
	if(user==NULL)
	{
		QString str2 = str.fromLocal8Bit("请输入用户名！");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		return;
	}
	if(pwd==NULL)
	{
		QString str2 = str.fromLocal8Bit("请输入密码！");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		return;
	}

	//sql.connect(ui->comboBox_2->currentIndex());

	QSqlQuery query(*sql.db);
	query.exec("select * from sys_users where Name='"+user+"' and Password='"+pwd+"'");

	QString strIsAuthorized;
	if(query.next())
	{
		if (user=="admin")
		{
			IsAdmin=true;
		}
		//IsAuthorized=query.value(3).toBool();
		sql.struser=user;
		accept();
	}
	else
	{
		QString str2 = str.fromLocal8Bit("用户名或密码错误！");
		QMessageBox::warning(this,str,str2,QMessageBox::Ok);
		ui->lineEdit_2->clear();
		ui->comboBox_2->setFocus();
		return;
	}
}
void ConnectDB::on_cancelButton_2_clicked()
{
	this->close();
}
void ConnectDB::on_exitButton_clicked()
{
	this->close();
}
void ConnectDB::on_cancelButton_clicked()
{
	this->close();
}
void ConnectDB::onChanged(int index)
{

}
void ConnectDB:: on_closeButton_clicked()
{
	this->close();
}
void ConnectDB:: on_openButton_clicked()
{
}
void ConnectDB::keyPressEvent(QEvent*event)
{
	if(event->type() == QEvent::KeyPress)
	{
		QKeyEvent*keyEvent=static_cast<QKeyEvent*>(event);//将事件转化为键盘事件
		if(keyEvent->key() == Qt::Key_Tab)
		{
			focusNextChild();
			return;
		}
		else
		{
		}
	}
}
bool ConnectDB::eventFilter(QObject*obj,QEvent*event)
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