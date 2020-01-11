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

	QObject::connect(&w, SIGNAL(sign_mission()), &mission, SLOT(show_mission()));
	QObject::connect(&w, SIGNAL(rada_mode()), &rada, SLOT(rada_show()));

	/*QObject::connect(&w, SIGNAL(sign_added_esmstrategy_section()), &section, SLOT(added_esmstrategy_section()));
	QObject::connect(&w, SIGNAL(sign_added_ecmstrategy_section()), &section, SLOT(added_ecmstrategy_section()));*/
	QObject::connect(&w, SIGNAL(sign_ecmstrategy_section()), &section, SLOT(show_ecmstrategy_section()));
	QObject::connect(&w, SIGNAL(sign_esmstrategy_section()), &section, SLOT(show_esmstrategy_section()));

	QObject::connect(&w, SIGNAL(sign_ecm_tech()), &tech, SLOT(show_ecm_tech()));

	QObject::connect(&section, SIGNAL(sign_added_dwellsquence()), &dwellsquence, SLOT(added_dwellsquence()));
	QObject::connect(&section, SIGNAL(sign_dwellsquence()), &dwellsquence, SLOT(show_dwellsquence()));

	QObject::connect(&section, SIGNAL(sign_added_ecmstrategy_tech()), &tech, SLOT(added_ecmstrategy_tech()));
	QObject::connect(&section, SIGNAL(sign_tech()), &tech, SLOT(show_ecmstragegy_tech()));

	QObject::connect(&mission, SIGNAL(sign_point()), &point, SLOT(show_point()));

	QObject::connect(&rada, SIGNAL(sign_erp()), &erp, SLOT(show_erp()));

	QObject::connect(&rada, SIGNAL(sign_scan()), &sc, SLOT(show_scan()));

	QObject::connect(&rada, SIGNAL(sign_pri()), &pri, SLOT(show_pri()));

	QObject::connect(&rada, SIGNAL(sign_pw()), &pw, SLOT(show_pw()));

	QObject::connect(&rada, SIGNAL(sign_rf()), &rf, SLOT(rf_show()));

	QObject::connect(&rf, SIGNAL(sign_add_new()), &rf_v, SLOT(show_rf_v()));
	QObject::connect(&rf, SIGNAL(sign_rf_value()), &rf_v, SLOT(show_rf_v()));

	QObject::connect(&pw, SIGNAL(sign_add()), &pw_v, SLOT(show_pw_v()));
	QObject::connect(&pw, SIGNAL(sign_pw_v()), &pw_v, SLOT(show_pw_v()));

	QObject::connect(&pri, SIGNAL(sign_add_new()), &pri_v, SLOT(show_pri_v()));
	QObject::connect(&pri, SIGNAL(sign_pri_values()), &pri_v, SLOT(show_pri_v()));
	w.show();
	return a.exec();
}
