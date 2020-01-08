#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_point_page.h"

class point_page : public QMainWindow
{
	Q_OBJECT

public:
	point_page(QWidget *parent = Q_NULLPTR);
	~point_page();
private slots:
	void show_point();
	void add();
	void del();
	void save();
private:
	Ui::point_page ui;
	
};
