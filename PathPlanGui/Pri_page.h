#pragma once

#include <QWidget>
#include "MainWindow.h"
#include <QPushbutton>
#include <QMainWindow>
#include <QComboBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include "ui_Pri.h"

class Pri_page : public QMainWindow
{
	Q_OBJECT

public:
	Pri_page(QMainWindow *parent = Q_NULLPTR);
	~Pri_page();
	QComboBox *pritype = new QComboBox();
private slots:
	void show_pri();
	void show_pri_va();
	void save();
	/*void add();
	void del();*/
	void emit_add();
	void add_new_pri();
signals:
	void sign_pri_values();
	void sign_add_new();
private:
	QStringList pritype_list;
	Ui::Pri ui;
};
