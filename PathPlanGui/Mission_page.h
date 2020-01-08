#pragma once

#include <QMainWindow>
#include "MainWindow.h"
#include <string>
#include <QComboBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include "ui_Mission_page.h"

class Mission_page : public QMainWindow
{
	Q_OBJECT

public:
	Mission_page(QWidget *parent = Q_NULLPTR);
	~Mission_page();
	
private slots:
	void save();
	void del();
	void add();
	void show_point();
	void added_mission();
	void show_mission();
signals:
	void sign_point();
private:
	Ui::Mission_page ui;
	QStringList mission_list;
};
