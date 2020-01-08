#pragma once

#include <QWidget>
#include <QPushButton>
#include "MainWindow.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <iostream>
#include "ui_Pw_values.h"

class Pw_values_page : public QMainWindow
{
	Q_OBJECT

public:
	Pw_values_page(QMainWindow *parent = Q_NULLPTR);
	~Pw_values_page();
private slots:
	void show_pw_v();
	void save();
	void add();
	void del();
private:
	Ui::Pw_values ui;
};
