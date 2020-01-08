#pragma once

#include <QWidget>
#include <QMainWindow>
#include "ui_Section_page.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

class Section_page : public QMainWindow
{
	Q_OBJECT

public:
	Section_page(QMainWindow *parent = Q_NULLPTR);
	~Section_page();
private slots:
	void show_esmstrategy_section();

	void show_ecmstrategy_section();

	void show_dwellsquence();

	void show_dwellsquence2();

	void show_ecmstragegy_tech();

	void show_ecmstragegy_tech2();

	void append_ecmstrategy_section();

	void del_ecmstrategy_section();

	void append_esmstrategy_section();

	void del_esmstrategy_section();

	void added_esmstrategy_section();

	void save_esmstrategy_section();

	void added_dwellsquence();

	void added_ecmstrategy_section();

	void added_ecmstrategy_tech();

	void save_ecmstrategy_section();
signals:
	void sign_added_esmstrategy_startlocation();
	void sign_added_esmstrategy_endlocation();
	void sign_added_ecmstrategy_startlocation();
	void sign_added_ecmstrategy_endlocation();
	void sign_added_dwellsquence();
	void sign_added_ecmstrategy_tech();
	void sign_esm_startlocation();
	void sign_esm_endlocation();
	void sign_ecm_startlocation();
	void sign_ecm_endlocation();
	void sign_dwellsquence();
	void sign_tech();

private:
	Ui::Section_page ui;
	QStringList headers_esmstrategy_section;
	QStringList headers_ecmstrategy_section;
};
