#pragma once

#include <QMainWindow>
#include "ui_tech_page.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>

class tech_page : public QMainWindow
{
	Q_OBJECT

public:
	tech_page(QWidget *parent = Q_NULLPTR);
	~tech_page();
private slots:
	void save_ecmstrategy_tech();
	void show_ecm_tech();
	void save_ecm_tech();
	void added_ecmstrategy_tech();
	void added_ecm_tech();
	void add_tech();
	void del_tech();
	void show_ecmstragegy_tech();

private:
	Ui::tech_page ui;
	QStringList tech_list;
	
};
