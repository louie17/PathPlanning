#pragma once

#include <QWidget>
#include "MainWindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTableWidget>
#include "ui_Pri_values.h"

class Pri_values_page : public QMainWindow
{
	Q_OBJECT

public:
	Pri_values_page(QMainWindow *parent = Q_NULLPTR);
	~Pri_values_page();
private slots:
	void save_pri_v();
	void show_pri_v();
	void add_pri_v();
	void del_pri_v();
private:
	QTableWidget *tableWidget = new QTableWidget(this);
	Ui::Pri_values ui;
};
