#ifndef PHARMACYSTATISTICS_H
#define PHARMACYSTATISTICS_H

#include <QWidget>
#include "ui_pharmacystatistics.h"

class PharmacyStatistics : public QWidget
{
	Q_OBJECT

public:
	PharmacyStatistics(QWidget *parent = 0);
	~PharmacyStatistics();
	void initUI();
	int iRow;
private:
	Ui::PharmacyStatistics ui;
public slots:
	void on_printButton_clicked();
	void on_findButton_clicked();
	void on_excelButton_clicked();
	void on_previewButton_clicked();
	void on_radioButton_clicked();
	void print(QPrinter* printer);
	void filePrintPreview();
	void DrugName(const QString &);
	void getItem(int row,int column);
};

#endif // PHARMACYSTATISTICS_H
