#pragma once

#include <QMainWindow>
#include "MainWindow.h"
#include <QPushbutton>
#include <QMessageBox>
#include <QComboBox>
#include <QVBoxLayout>
#include "ui_Pw.h"

class Pw_page : public QMainWindow
{
	Q_OBJECT

public:
	Pw_page(QMainWindow *parent = Q_NULLPTR);
	~Pw_page();
	QComboBox *pwtype = new QComboBox();
private slots:
	void show_pw();
	//void show_pw_2();
	void show_pw_v();
	void add_button();
	void save();
	//void add();
	//void del();
	void add_new_pw();
signals:
	void sign_pw_values();
	void sign_pw_v();
	void sign_add();
private:
	Ui::Pw ui;
	QStringList pwtype_list;
};
