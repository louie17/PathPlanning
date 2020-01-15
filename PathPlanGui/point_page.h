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
	void add_tar_point();
	void del_tar_point();
	void save_tar_point();
private:
	Ui::point_page ui;
	
};
