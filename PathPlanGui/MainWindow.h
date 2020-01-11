#pragma once

#include <map>
#include <vector>
#include <string>

#include <QtWidgets/QMainWindow>
#include <QLabel>
#include <Qtxml>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QPointer>
#include <QTextTableFormat>
#include <QPainter>
#include <QPen>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#include <QColor>
#include <QBarSet>

#include "Scenario.hpp"
#include "xml_prase.h"

#include <Eigen/Eigen>
#include "MarkovEvaluate.h"

#include "ui_MainWindow.h"

extern int choice_emitter;
extern int choice_radar;
extern int choice_esmstrategy;
extern int choice_ecmstrategy;
extern int choice_section;
extern int choice_ecm;
extern int choice_ownplatform;
extern int choice_point;

extern QDomDocument dom;
extern QFile file;
extern sce::Scenario scenario;

class MyTab
{
public:
	MyTab() {}
	~MyTab() {}
	QPointer<QPushButton> add;
	QPointer<QPushButton> del;
	QPointer<QPushButton> save;
	int myindex;
	QPointer<QTableWidget>  my_table;
};


//日志处理类
class logger : public QObject
{
	Q_OBJECT
public:
	~logger() {}
	static logger *_instance;
	static logger *instance();

public:
	void loggerMaster(QtMsgType type, const QString &);

signals:
	void G_sndMsg(QtMsgType type, const QString &);

private:
	//将默认构造函数设置为私有
	logger(QObject *parent = 0) :
		QObject(parent) {}
};

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

class PathPlanGui : public QMainWindow
{
	Q_OBJECT
private:
	QLabel *LabCurFile = new QLabel("file");    //当前文件
	QLabel *LabCellPos = new QLabel("Position");   //当前单元格行列号
	QLabel *LabCellText = new QLabel("Text"); //当前单元格内容

	//QChart *preResult_chart = new QChart();
	//QChart *lastResult_chart = new QChart();

	size_t open_flag{ 0 };//文件打开次数标志位
	size_t m_status_count{0}; //状态信息条数

	sce::Site_WeaponRange_relation swRelation;
	std::map<std::string, double> CofRada;
	
	bool isfinished;

	QVector<MyTab*> vTab;
	QVector<QTableWidget*> route_v;
	QPushButton * tab_btn = NULL;
	int route_index = -1;

	QStringList platformtype_list{ "Land" , "Air" , "Surface" };
	QStringList ownplatformtype_list{ "Air" };

	//QStandardItemModel * VertexModel; //数据模型
	//QStandardItemModel * PlatformModel; //数据模型
	//QStandardItemModel * EmitterModel; //数据模型
	//QStandardItemModel * SiteModel; //数据模型
	//QStandardItemModel * WeaponModel; 
	//QStandardItemModel * OwnPlatformModel;
	//QStandardItemModel * MissionModel;
	//QStandardItemModel * PointModel;
	//
	//QStandardItemModel * LocationModel;

	//QStandardItemModel * EsmModel;
	//QStandardItemModel * EcmModel;
	//QStandardItemModel * EsmStrategyModel;
	//QStandardItemModel * EcmStrategyModel;
	//QStandardItemModel * EsmStrategySectionModel;
	//QStandardItemModel * EcmStrategySectionModel;
	//QStandardItemModel * DwellSquenceModel;

	//QStandardItemModel * RouteModel;
	//QStandardItemModel * WayPointModel;

	//QStandardItemModel * RfModel;
	//QStandardItemModel * Rf_valuesModel;
	//QStandardItemModel * PwModel;
	//QStandardItemModel * Pw_valuesModel;
	//QStandardItemModel * PriModel;
	//QStandardItemModel * Pri_valuesModel;
	//QStandardItemModel * ScanModel;
	//QStandardItemModel * ErpModel;
	//QStandardItemModel * RadarModeModel;

	//QStandardItemModel * PSRelationModel;
	//QStandardItemModel * PERelationModel;
	//QStandardItemModel * PWRelationModel;
	//QStandardItemModel * OPEsmRelationModel;
	//QStandardItemModel * EsmESRelationModel;
	//QStandardItemModel * OPEcmRelationModel;
	//QStandardItemModel * EcmESRelationModel;
	//QStandardItemModel * OPRRelationModel;

	//QItemSelectionModel *theSelection; //选择模型


	
public:
	PathPlanGui(QWidget *parent = Q_NULLPTR);
	void listDom(QDomElement & docelem);

	//std::vector<std::shared_ptr<sce::Route>> routes;
	//std::vector<std::shared_ptr<sce::EsmStrategy>> esmstrategys;
	//std::vector<std::shared_ptr<sce::EcmStrategy>> ecmstrategys;
	//std::vector<sce::PlatformSiteRelation> PSRs;
	//std::vector<sce::PlatformEmitterRelation> PERs;
	//std::vector<sce::PlatformWeaponRelation> PWRs;
	//std::vector<sce::OwnPlatformEsmRelation> OPEsmRs;
	//std::vector<sce::EsmEsmStrategyRelation> EsmESRs;
	//std::vector<sce::OwnPlatformEcmRelation> OPEcmRs;
	//std::vector<sce::EcmEcmStrategyRelation> EcmESRs;
	//std::vector<sce::OwnPlatformRouteRelation> OPRRs;

	void draw_survival_rate(MatrixXd stateProbs);
	void show_Vertex_data();
	void show_Platform_data();
	void show_Emitter_data();
	void show_Weapon_data();
	void show_Site_data();
	void show_OwnPlatform_data();
	void show_Ecm_data();
	void show_Esm_data();
	void show_EsmStratgy_data();
	void show_EcmStratgy_data();
	void show_Route_data();
	void show_PSRs_data();
	void show_PERs_data();
	void show_PWRs_data();
	void show_OPEsmRs_data();
	void show_EsmESRs_data();
	void show_OPEcmRs_data();
	void show_EcmESRs_data();
	void show_OPRRs_data();
//	QVector<MyTab*> vTab;
	
signals:
	void go_next();//显示下一页
	void rada_mode();
	void sign_add_rada();
	void sign_mission();
	void sign_added_mission();
	void sign_show_xml_data();
	void sign_esmstrategy_section();
	void sign_ecmstrategy_section();
	void sign_ecm_tech();
	//void outputMsg(QtMsgType type, const QString &msg);

private slots:
	//当前选择单元格发生变化
	//void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

	void save_to_file();

	void save_PlatformSiteRelation();

	void save_PlatformEmitterRelation();

	void save_PlatformWeaponRelation();

	void save_OwnPlatformEsmRelation();

	void save_EsmEsmStrategyRelation();

	void save_OwnPlatformRouteRelation();

	void save_EcmEcmStrategyRelation();

	void save_OwnPlatformEcmRelation();

	void save_Platform();

	void save_Weapon();

	void save_Site();

	void save_OwnPlatform();

	void save_Ecm();

	void save_ECMStrategy();

	void save_ESMStrategy();

	void save_Esm();

	void save_Emitter();
	void save_Vertex();
	void add_RouteTab();
	void del_RouteTab();
	void on_actOpen_triggered();
	void show_xml_data();
	void setOpenFileFlag();
	void add_Vertex();
	void del_Vertex();
	void add_PlatformSiteRelation();
	void del_PlatformSiteRelation();
	void add_PlatformEmitterRelation();
	void del_PlatformEmitterRelation();
	void add_PlatformWeaponRelation();
	void del_PlatformWeaponRelation();
	void add_OwnPlatformEsmRelation();
	void del_OwnPlatformEsmRelation();
	void add_EsmEsmStrategyRelation();
	void del_EsmEsmStrategyRelation();
	void add_OwnPlatformRouteRelation();
	void del_OwnPlatformRouteRelation();
	void add_OwnPlatformEcmRelation();
	void del_OwnPlatformEcmRelation();
	void add_EcmEcmStrategyRelation();
	void del_EcmEcmStrategyRelation();
	void add_EcmStrategy();
	void del_EcmStrategy();
	void add_Esm();
	void del_Esm();
	void add_Ecm();
	void del_Ecm();
	void add_EsmStrategy();
	void del_EsmStrategy();
	void add_OwnPlatform();
	void del_OwnPlatform();
	void add_Site();
	void del_Site();
	void add_Weapon();
	void del_Weapon();
	void add_Platform();
	void del_Platform();
	void add_Emitter();
	void del_Emitter();
	void on_actSave_triggered();
	void show_rada();
	void show_mission();
	void ecm_tech();
	void show_esmstrategy_section();
	void show_ecmstrategy_section();
	void setLabelText(int);
	void setLabelText1(int);
	void setSeleteRoute(int);
	void run_algorithm();
	void route_evaluate();

	void backTab();
	void nextTab();

	void onStatusInfo(QtMsgType type, QString msg);

private:
	Ui::PathPlanGuiClass ui;
};