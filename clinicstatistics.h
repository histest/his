#ifndef CLINICSTATISTICS_H
#define CLINICSTATISTICS_H
#include <QtGui>
#include <QWidget>
#include "ui_clinicstatistics.h"

class ClinicStatistics : public QWidget
{
	Q_OBJECT

public:
	ClinicStatistics(QWidget *parent = 0);
	~ClinicStatistics();
	void initUI();
	QStringList words;
	QListView *listView; 
	QStringListModel *model; 
	void keyPressEvent(QKeyEvent *e);
private:
	Ui::ClinicStatistics ui;
	private slots:
		void on_queryButton_clicked();
		void on_clearButton_clicked();
		void on_queryButton_2_clicked();
		void on_clearButton_2_clicked();
		void on_queryButton_3_clicked();
		void on_clearButton_3_clicked();
		void getItem(int row,int column);
		void on_radioButton_clicked();
		void on_radioButton_2_clicked();
		void on_radioButton_3_clicked();
		void on_radioButton_4_clicked();
		void completeText(const QModelIndex &index); // 点击完成列表中的项，使用此项自动完成输入的单词
		void setCompleter(const QString &text); // 动态的显示完成列表
};

#endif // CLINICSTATISTICS_H
