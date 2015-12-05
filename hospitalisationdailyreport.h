#ifndef HOSPITALISATIONDAILYREPORT_H
#define HOSPITALISATIONDAILYREPORT_H

#include <QWidget>
#include "ui_hospitalisationdailyreport.h"

class Hospitalisationdailyreport : public QWidget
{
	Q_OBJECT

public:
	Hospitalisationdailyreport(QWidget *parent = 0);
	~Hospitalisationdailyreport();
	void initUI();
private:
	Ui::Hospitalisationdailyreport ui;
	private slots:
		void on_addButton_clicked();
		void on_saveButton_clicked();
		void on_discardButton_clicked();
		void on_dailyreportqueryButton_clicked();
		void on_editButton_clicked();
		void on_deleteButton_clicked();
};

#endif // HOSPITALISATIONDAILYREPORT_H
