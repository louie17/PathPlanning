#pragma once

#include <QWidget>
#include "MainWindow.h"
#include <QPushbutton>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QMessageBox>
#include "ui_Scan.h"

class Scan_page : public QMainWindow
{
	Q_OBJECT

public:
	Scan_page(QMainWindow *parent = Q_NULLPTR);
	~Scan_page();
	QComboBox *scantype = new QComboBox();
private slots:
	void show_scan();
	void save();
	//void add();
	//void del();

private:
	Ui::Scan ui;
	QStringList scantype_name;
};
