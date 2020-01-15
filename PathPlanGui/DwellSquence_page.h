#pragma once

#include <QMainWindow>
#include "ui_DwellSquence_page.h"
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

class DwellSquence_page : public QMainWindow
{
	Q_OBJECT

public:
	DwellSquence_page(QWidget *parent = Q_NULLPTR);
	~DwellSquence_page();
private slots:
	void show_dwellsquence();
	void add_dwellSquence();

	void del_dwellSquence();

	void save_dwellSquence();

	void added_dwellsquence();

private:
	Ui::DwellSquence_page ui;
};
