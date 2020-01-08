#pragma once

#include <QWidget>
#include "MainWindow.h"
#include <QPushbutton>
#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QComboBox>
#include "ui_Rf.h"

class Rf_page : public QMainWindow
{
	Q_OBJECT

public:

	Rf_page(QMainWindow *parent = Q_NULLPTR);
	~Rf_page();
	QComboBox *rftype = new QComboBox();
	//QComboBox *rftype2 = new QComboBox();
private slots:
	void rf_show();
	void show_rf_value();
	void save();
/*	void add();
	void del()*/;
	void add_new();
	void add_new_Rf();
signals:
	void sign_rf_value();
	void sign_add_new();
private:
	Ui::Rf ui;
	QStringList rftype_name;
};
