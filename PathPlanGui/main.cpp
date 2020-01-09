#include "stdafx.h"
#include "MainWindow.h"
#include <QtGlobal>
#include <QtWidgets/QApplication>

#include "Section_page.h"
#include "DwellSquence_page.h"
#include "tech_page.h"
#include "Radar_mode_page.h"
#include "Mission_page.h"
#include "Rf_page.h"
#include "Pw_page.h"
#include "Pri_page.h"
#include "Scan_page.h"
#include "Erp_page.h"
#include "Pri_values_page.h"
#include "Rf_values_page.h"
#include "Pw_values_page.h"
#include "point_page.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qInstallMessageHandler(outputMessage); //注册MsgHandler回调函数

	PathPlanGui w;
	//子页面
	Radar_mode_page rada;
	Rf_page rf;
	Pw_page pw;
	Pri_page pri;
	Scan_page sc;
	Erp_page erp;
	Pri_values_page pri_v;
	Rf_values_page rf_v;
	Pw_values_page pw_v;
	Section_page section;
	DwellSquence_page dwellsquence;
	tech_page tech;
	Mission_page mission;
	point_page point;

	w.show();
	return a.exec();
}
