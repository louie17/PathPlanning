#pragma once

#include <QWidget>
#include "MainWindow.h"
#include "ui_Radar_mode.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QPushbutton>
#include <iostream>
#include <QComboBox>
#include <QVBoxLayout>

class Radar_mode_page : public QMainWindow
{
	Q_OBJECT

public:
	Radar_mode_page(QMainWindow *parent = Q_NULLPTR);
	~Radar_mode_page();
	
private slots:
	void rada_show();
	void save_radar();
	void show_rf();
	void show_pw();
	void show_pri();
	void show_scan();
	void show_erp();
	void add_radar();
	void del_radar();
signals:
	void sign_rf();
	void sign_pw();
	void sign_pri();
	void sign_scan();
	void sign_erp();
private:
	Ui::Radar_mode ui;
	QStringList modetype_list;
};
