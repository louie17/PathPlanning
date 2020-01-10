#include "stdafx.h"
#include "MainWindow.h"
#include "de_types.hpp"
#include "DE_main.hpp"
#include "A_STAR.h"

#pragma execution_character_set("utf-8")

int choice_emitter = 0;
int choice_radar = 0;
int choice_esmstrategy = 0;
int choice_ecmstrategy = 0;
int choice_section = 0;
int choice_ecm = 0;
int choice_ownplatform = 0;
int choice_point = 1;
sce::Scenario scenario;
QDomDocument dom;
QFile file;

//-----------------------------------------日志处理部分开始

//初始化静态变量
logger * logger::_instance = 0;

//构建单实例日志对象
logger * logger::instance()
{
	if (!logger::_instance)
		logger::_instance = new logger;
	return logger::_instance;
}


void logger::loggerMaster(QtMsgType type, const QString & msg)
{
	//在日志信息中加入时间标记
	//QString newLog = QDateTime::currentDateTime().toString(QLatin1String("MM-dd hh:mm:ss:zzz")) + QLatin1Char(' ') + msg;
	//发送处理后的日志信息
	emit G_sndMsg(type, msg);
}

//日志处理函数
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();

	QString text;
	switch (type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;

	case QtWarningMsg:
		text = QString("Warning:");
		break;

	case QtCriticalMsg:
		text = QString("Critical:");
		break;

	case QtFatalMsg:
		text = QString("Fatal:");
	}

	QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

	QFile file("log.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();

	logger::instance()->loggerMaster(type, message);
	//onStatusInfo(type, msg);
	//emit outputMsg(type, msg);
	mutex.unlock();
}
//------------------------------------------日志处理部分结束

PathPlanGui::PathPlanGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.SceTab->setCurrentIndex(0);
	ui.SceTab->setTabPosition(QTabWidget::North);//设置选项卡的方位，默认在上方(东南西北)
	ui.SceTab->setTabShape(QTabWidget::Triangular);//设置选项卡的形状 Rounded
	ui.SceTab->usesScrollButtons();//选项卡滚动
	//setCentralWidget(ui->splitter);
	//VertexModel = new QStandardItemModel(this); //数据模型
	//theSelection = new QItemSelectionModel(VertexModel);//选择模型
	//connect(theSelection, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(on_currentChanged(QModelIndex, QModelIndex)));
	//ui.tableView_Vertex->setModel(VertexModel); //设置数据模型
	//ui.tableView_Vertex->setSelectionModel(theSelection); //设置选择模型
	//ui.tableView_Vertex->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//ui.tableView_Vertex->setSelectionBehavior(QAbstractItemView::SelectItems);

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(on_actOpen_triggered()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(setOpenFileFlag()));
	connect(this, SIGNAL(sign_show_xml_data()), this, SLOT(show_xml_data()));
	connect(ui.actionRefresh, SIGNAL(triggered()), this, SLOT(show_xml_data()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save_to_file()));

	//connect(ui.actionInsert, SIGNAL(triggered()), this, SLOT(on_actInsert_triggered()));
	//connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(on_actDelete_triggered()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(on_actSave_triggered()));
	connect(ui.pushButton_back, SIGNAL(clicked()), this, SLOT(backTab()));
	connect(ui.pushButton_next, SIGNAL(clicked()), this, SLOT(nextTab()));
	connect(logger::instance(),SIGNAL(G_sndMsg(QtMsgType, QString)), this, SLOT(onStatusInfo(QtMsgType, QString)));
	connect(ui.pushButton_algrun, SIGNAL(clicked()), this, SLOT(run_algorithm()));

	connect(ui.comboBox_OPS, SIGNAL(currentIndexChanged(int)), this, SLOT(setLabelText(int)));
	connect(ui.comboBox_OPSel, SIGNAL(currentIndexChanged(int)), this, SLOT(setLabelText1(int)));
	connect(ui.comboBox_OPSel, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeleteRoute(int)));
	connect(ui.pushButton_Eval, SIGNAL(clicked()), this, SLOT(route_evaluate()));

	/*xml各个二级节点的添加删除以及保存功能函数与按钮的connect*/
	connect(ui.pushButton_VaddRow, SIGNAL(clicked()), this, SLOT(add_Vertex()));
	connect(ui.pushButton_VdelRow, SIGNAL(clicked()), this, SLOT(del_Vertex()));
	connect(ui.pushButton_EaddRow, SIGNAL(clicked()), this, SLOT(add_Emitter()));
	connect(ui.pushButton_EdeRow, SIGNAL(clicked()), this, SLOT(del_Emitter()));
	connect(ui.pushButton_PaddRow, SIGNAL(clicked()), this, SLOT(add_Platform()));
	connect(ui.pushButton_PdelRow, SIGNAL(clicked()), this, SLOT(del_Platform()));
	connect(ui.pushButton_WaddRow, SIGNAL(clicked()), this, SLOT(add_Weapon()));
	connect(ui.pushButton_WdelRow, SIGNAL(clicked()), this, SLOT(del_Weapon()));
	connect(ui.pushButton_SaddRow, SIGNAL(clicked()), this, SLOT(add_Site()));
	connect(ui.pushButton_SdelRow, SIGNAL(clicked()), this, SLOT(del_Site()));
	connect(ui.pushButton_OPadd, SIGNAL(clicked()), this, SLOT(add_OwnPlatform()));
	connect(ui.pushButton_OPdel, SIGNAL(clicked()), this, SLOT(del_OwnPlatform()));
	connect(ui.pushButton_Ecmadd, SIGNAL(clicked()), this, SLOT(add_Ecm()));
	connect(ui.pushButton_Ecmdel, SIGNAL(clicked()), this, SLOT(del_Ecm()));
	connect(ui.pushButton_Esmadd, SIGNAL(clicked()), this, SLOT(add_Esm()));
	connect(ui.pushButton_Esmdel, SIGNAL(clicked()), this, SLOT(del_Esm()));
	connect(ui.pushButton_Ecmsadd, SIGNAL(clicked()), this, SLOT(add_EcmStrategy()));
	connect(ui.pushButton_Ecmsdel, SIGNAL(clicked()), this, SLOT(del_EcmStrategy()));
	connect(ui.pushButton_Esmsadd, SIGNAL(clicked()), this, SLOT(add_EsmStrategy()));
	connect(ui.pushButton_Esmsdel, SIGNAL(clicked()), this, SLOT(del_EsmStrategy()));
	connect(ui.pushButton_PSRadd, SIGNAL(clicked()), this, SLOT(add_PlatformSiteRelation()));
	connect(ui.pushButton_PSRdel, SIGNAL(clicked()), this, SLOT(del_PlatformSiteRelation()));
	connect(ui.pushButton_PERadd, SIGNAL(clicked()), this, SLOT(add_PlatformEmitterRelation()));
	connect(ui.pushButton_PERdel, SIGNAL(clicked()), this, SLOT(del_PlatformEmitterRelation()));
	connect(ui.pushButton_PWRadd, SIGNAL(clicked()), this, SLOT(add_PlatformWeaponRelation()));
	connect(ui.pushButton_PWRdel, SIGNAL(clicked()), this, SLOT(del_PlatformWeaponRelation()));
	connect(ui.pushButton_OEsadd, SIGNAL(clicked()), this, SLOT(add_OwnPlatformEsmRelation()));
	connect(ui.pushButton_OEsdel, SIGNAL(clicked()), this, SLOT(del_OwnPlatformEsmRelation()));
	connect(ui.pushButton_EsmESadd, SIGNAL(clicked()), this, SLOT(add_EsmEsmStrategyRelation()));
	connect(ui.pushButton_EsmESdel, SIGNAL(clicked()), this, SLOT(del_EsmEsmStrategyRelation()));
	connect(ui.pushButton_ORRadd, SIGNAL(clicked()), this, SLOT(add_OwnPlatformRouteRelation()));
	connect(ui.pushButton_ORRdel, SIGNAL(clicked()), this, SLOT(del_OwnPlatformRouteRelation()));
	connect(ui.pushButton_OEcRadd, SIGNAL(clicked()), this, SLOT(add_OwnPlatformEcmRelation()));
	connect(ui.pushButton_OEcRdel, SIGNAL(clicked()), this, SLOT(del_OwnPlatformEcmRelation()));
	connect(ui.pushButton_EcmESadd, SIGNAL(clicked()), this, SLOT(add_EcmEcmStrategyRelation()));
	connect(ui.pushButton_EcmESdel, SIGNAL(clicked()), this, SLOT(del_EcmEcmStrategyRelation()));
	connect(ui.pushButton_addTab, SIGNAL(clicked()), this, SLOT(add_RouteTab()));
	connect(ui.pushButton_delTab, SIGNAL(clicked()), this, SLOT(del_RouteTab()));
	connect(ui.pushButton_Vsave, SIGNAL(clicked()), this, SLOT(save_Vertex()));
	connect(ui.pushButton_Esave, SIGNAL(clicked()), this, SLOT(save_Emitter()));
	connect(ui.pushButton_Psave, SIGNAL(clicked()), this, SLOT(save_Platform()));
	connect(ui.pushButton_Wsave, SIGNAL(clicked()), this, SLOT(save_Weapon()));
	connect(ui.pushButton_Ssave, SIGNAL(clicked()), this, SLOT(save_Site()));
	connect(ui.pushButton_OPsave, SIGNAL(clicked()), this, SLOT(save_OwnPlatform()));
	connect(ui.pushButton_Ecmsave, SIGNAL(clicked()), this, SLOT(save_Ecm()));
	connect(ui.pushButton_Esmsave, SIGNAL(clicked()), this, SLOT(save_Esm()));
	connect(ui.pushButton_Esmssave, SIGNAL(clicked()), this, SLOT(save_ESMStrategy()));
	connect(ui.pushButton_Ecmssave, SIGNAL(clicked()), this, SLOT(save_ECMStrategy()));

	connect(ui.pushButton_PSRsave, SIGNAL(clicked()), this, SLOT(save_PlatformSiteRelation()));
	connect(ui.pushButton_PERsave, SIGNAL(clicked()), this, SLOT(save_PlatformEmitterRelation()));
	connect(ui.pushButton_PWRsave, SIGNAL(clicked()), this, SLOT(save_PlatformWeaponRelation()));
	connect(ui.pushButton_OEssave, SIGNAL(clicked()), this, SLOT(save_OwnPlatformEsmRelation()));
	connect(ui.pushButton_EsmESsave, SIGNAL(clicked()), this, SLOT(save_EsmEsmStrategyRelation()));
	connect(ui.pushButton_ORRsave, SIGNAL(clicked()), this, SLOT(save_OwnPlatformRouteRelation()));
	connect(ui.pushButton_OEcRsave, SIGNAL(clicked()), this, SLOT(save_OwnPlatformEcmRelation()));
	connect(ui.pushButton_EcmESsave, SIGNAL(clicked()), this, SLOT(save_EcmEcmStrategyRelation()));

	//创建状态栏组件，
	LabCurFile->setAlignment(Qt::AlignLeft);
	LabCurFile->setMinimumSize(LabCurFile->sizeHint());
	//LabCurFile->setFrameShape(QFrame::WinPanel); //设置标签形状
	//LabCurFile->setFrameShadow(QFrame::Sunken); //设置标签阴影
	ui.statusBar->addWidget(LabCurFile);

	LabCellPos->setMinimumSize(LabCellPos->sizeHint());
	ui.statusBar->addWidget(LabCellPos);

	LabCellText->setMinimumSize(LabCellText->sizeHint());
	ui.statusBar->addWidget(LabCellText);
}
void PathPlanGui::save_to_file() {
	if (!file.open(QFile::WriteOnly | QFile::Truncate)) //先读进来，再重写，如果不用truncate就是在后面追加内容，就无效了
		return;
	QTextStream out_stream(&file);
	dom.save(out_stream, 4); //缩进4格
	file.close();
	QMessageBox::about(this, tr("Tip"), tr("Save to file successfully"));
}
void PathPlanGui::save_PlatformSiteRelation() {
	int num = ui.tableWidget_PSR->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_PSR->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_PSR->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::PlatformSiteRelation new_relation;
	for (auto x : scenario.getAllPlatform())
	{
		if (x->getPlatformName() == re.toStdString())
		{
			new_relation.setPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllSite())
	{
		if (x->getName() == re_2.toStdString())
		{
			new_relation.setSite(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllPlatformSiteRelation().size())
	{
		for (auto x : scenario.getAllPlatformSiteRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getSiteName() == new_relation.getSiteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addPlatformSiteRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("PlatformSiteRelation");
		QDomElement third_1 = dom.createElement("Platform");
		QDomElement third_2 = dom.createElement("Site");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllPlatformSiteRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getSiteName() == new_relation.getSiteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllPlatform())
		{
			if (x->getPlatformName() == re.toStdString())
			{
				scenario.getAllPlatformSiteRelation()[num].setPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllSite())
		{
			if (x->getName() == re_2.toStdString())
			{
				scenario.getAllPlatformSiteRelation()[num].setSite(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("PlatformSiteRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_PlatformEmitterRelation() {
	int num = ui.tableWidget_PER->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_PER->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_PER->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::PlatformEmitterRelation new_relation;
	for (auto x : scenario.getAllPlatform())
	{
		if (x->getPlatformName() == re.toStdString())
		{
			new_relation.setPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllEmitter())
	{
		if (x->getName() == re_2.toStdString())
		{
			new_relation.setEmitter(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllPlatformEmitterRelation().size())
	{
		for (auto x : scenario.getAllPlatformEmitterRelation())
		{
			if ((x.getEmitterName() == new_relation.getEmitterName()) && (x.getPlatformName() == new_relation.getPlatformName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addPlatformEmitterRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("PlatformEmitterRelation");
		QDomElement third_1 = dom.createElement("Platform");
		QDomElement third_2 = dom.createElement("Emitter");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllPlatformEmitterRelation())
		{
			if ((x.getEmitterName() == new_relation.getEmitterName()) && (x.getPlatformName() == new_relation.getPlatformName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllPlatform())
		{
			if (x->getPlatformName() == re.toStdString())
			{
				scenario.getAllPlatformEmitterRelation()[num].setPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllEmitter())
		{
			if (x->getName() == re_2.toStdString())
			{
				scenario.getAllPlatformEmitterRelation()[num].setEmitter(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("PlatformEmitterRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_PlatformWeaponRelation() {
	int num = ui.tableWidget_PWR->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_PWR->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_PWR->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::PlatformWeaponRelation new_relation;
	for (auto x : scenario.getAllPlatform())
	{
		if (x->getPlatformName() == re.toStdString())
		{
			new_relation.setPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllWeapon())
	{
		if (x->getName() == re_2.toStdString())
		{
			new_relation.setWeapon(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllPlatformWeaponRelation().size()) {
		for (auto x : scenario.getAllPlatformWeaponRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getWeaponName() == new_relation.getWeaponName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addPlatformWeaponRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("PlatformWeaponRelation");
		QDomElement third_1 = dom.createElement("Platform");
		QDomElement third_2 = dom.createElement("Weapon");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllPlatformWeaponRelation())
		{
			if ((x.getPlatformName() == new_relation.getPlatformName()) && (x.getWeaponName() == new_relation.getWeaponName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllPlatform())
		{
			if (x->getPlatformName() == re.toStdString())
			{
				scenario.getAllPlatformWeaponRelation()[num].setPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllWeapon())
		{
			if (x->getName() == re_2.toStdString())
			{
				scenario.getAllPlatformWeaponRelation()[num].setWeapon(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("PlatformWeaponRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_OwnPlatformEsmRelation() {
	int num = ui.tableWidget_OEs->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_OEs->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_OEs->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::OwnPlatformEsmRelation new_relation;
	for (auto x : scenario.getAllOwnPlatform())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setOwnPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllEsm())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEsm(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllOwnPlatformEsmRelation().size()) {
		for (auto x : scenario.getAllOwnPlatformEsmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEsmName() == new_relation.getEsmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addOwnPlatformEsmRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("OwnPlatformEsmRelation");
		QDomElement third_1 = dom.createElement("OwnPlatform");
		QDomElement third_2 = dom.createElement("Esm");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllOwnPlatformEsmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEsmName() == new_relation.getEsmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllOwnPlatform())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllOwnPlatformEsmRelation()[num].setOwnPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllEsm())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllOwnPlatformEsmRelation()[num].setEsm(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatformEsmRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_EsmEsmStrategyRelation() {
	int num = ui.tableWidget_EsmES->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_EsmES->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_EsmES->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::EsmEsmStrategyRelation new_relation;
	for (auto x : scenario.getAllEsm())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setEsm(x);
			break;
		}
	}
	for (auto x : scenario.getAllEsmStrategy())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEsmStrategy(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllEsmEsmStrategyRelation().size())
	{
		for (auto x : scenario.getAllEsmEsmStrategyRelation())
		{
			if ((x.getEsmName() == new_relation.getEsmName()) && (x.getEsmStrategyName() == new_relation.getEsmStrategyName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addEsmEsmStrategyRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("EsmEsmStrategyRelation");
		QDomElement third_1 = dom.createElement("Esm");
		QDomElement third_2 = dom.createElement("EsmStrategy");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllEsmEsmStrategyRelation())
		{
			if ((x.getEsmName() == new_relation.getEsmName()) && (x.getEsmStrategyName() == new_relation.getEsmStrategyName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllEsm())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllEsmEsmStrategyRelation()[num].setEsm(x);
				break;
			}
		}
		for (auto x : scenario.getAllEsmStrategy())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllEsmEsmStrategyRelation()[num].setEsmStrategy(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("EsmEsmStrategyRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_OwnPlatformRouteRelation() {
	int num = ui.tableWidget_ORR->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_ORR->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_ORR->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::OwnPlatformRouteRelation new_relation;
	for (auto x : scenario.getAllOwnPlatform())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setOwnPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllRoute())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setRoute(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllOwnPlatformRouteRelation().size())
	{
		for (auto x : scenario.getAllOwnPlatformRouteRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getRouteName() == new_relation.getRouteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addOwnPlatformRouteRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("OwnPlatformRouteRelation");
		QDomElement third_1 = dom.createElement("OwnPlatform");
		QDomElement third_2 = dom.createElement("Route");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllOwnPlatformRouteRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getRouteName() == new_relation.getRouteName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllOwnPlatform())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllOwnPlatformRouteRelation()[num].setOwnPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllRoute())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllOwnPlatformRouteRelation()[num].setRoute(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatformRouteRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_EcmEcmStrategyRelation() {
	int num = ui.tableWidget_EcmES->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_EcmES->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_EcmES->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::EcmEcmStrategyRelation new_relation;
	for (auto x : scenario.getAllEcm())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setEcm(x);
			break;
		}
	}
	for (auto x : scenario.getAllEcmStrategy())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEcmStrategy(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllEcmEcmStrategyRelation().size())
	{
		for (auto x : scenario.getAllEcmEcmStrategyRelation())
		{
			if ((x.getEcmName() == new_relation.getEcmName()) && (x.getEcmStrategyName() == new_relation.getEcmStrategyName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addEcmEcmStrategyRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("EcmEcmStrategyRelation");
		QDomElement third_1 = dom.createElement("Ecm");
		QDomElement third_2 = dom.createElement("EcmStrategy");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);

	}
	else
	{
		for (int i = 0; i<scenario.getAllEcmEcmStrategyRelation().size(); i++)
		{
			for (int j = i; j<scenario.getAllEcmEcmStrategyRelation().size(); j++)
			{
				if ((scenario.getAllEcmEcmStrategyRelation()[i].getEcmName() == scenario.getAllEcmEcmStrategyRelation()[j].getEcmName()) && (scenario.getAllEcmEcmStrategyRelation()[i].getEcmStrategyName() == scenario.getAllEcmEcmStrategyRelation()[j].getEcmStrategyName()))
				{
					QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
					return;
				}
			}
		}
		for (auto x : scenario.getAllEcm())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllEcmEcmStrategyRelation()[num].setEcm(x);
				break;
			}
		}
		for (auto x : scenario.getAllEcmStrategy())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllEcmEcmStrategyRelation()[num].setEcmStrategy(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("EcmEcmStrategyRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_OwnPlatformEcmRelation() {
	int num = ui.tableWidget_OPEcmR->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_OPEcmR->cellWidget(num, 0));
	QString re = cell->currentText();
	QComboBox* cell_2 = qobject_cast<QComboBox *>(ui.tableWidget_OPEcmR->cellWidget(num, 1));
	QString re_2 = cell_2->currentText();
	sce::OwnPlatformEcmRelation new_relation;
	for (auto x : scenario.getAllOwnPlatform())
	{
		if (x->getName() == re.toStdString()) {
			new_relation.setOwnPlatform(x);
			break;
		}
	}
	for (auto x : scenario.getAllEcm())
	{
		if (x->getName() == re_2.toStdString()) {
			new_relation.setEcm(x);
			break;
		}
	}
	if (num + 1 > scenario.getAllOwnPlatformEcmRelation().size()) {
		for (auto x : scenario.getAllOwnPlatformEcmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEcmName() == new_relation.getEcmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		scenario.addOwnPlatformEcmRelation(new_relation);
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomElement root = dom.documentElement();
		QDomElement relation = dom.createElement("OwnPlatformEcmRelation");
		QDomElement third_1 = dom.createElement("OwnPlatform");
		QDomElement third_2 = dom.createElement("Ecm");
		QDomText text1 = dom.createTextNode(re);
		QDomText text2 = dom.createTextNode(re_2);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		relation.appendChild(third_1);
		relation.appendChild(third_2);
		root.appendChild(relation);
	}
	else {
		for (auto x : scenario.getAllOwnPlatformEcmRelation())
		{
			if ((x.getOwnPlatformName() == new_relation.getOwnPlatformName()) && (x.getEcmName() == new_relation.getEcmName()))
			{
				QMessageBox::warning(this, tr("Tip"), tr("The Relation already exists"));
				return;
			}
		}
		for (auto x : scenario.getAllOwnPlatform())
		{
			if (x->getName() == re.toStdString()) {
				scenario.getAllOwnPlatformEcmRelation()[num].setOwnPlatform(x);
				break;
			}
		}
		for (auto x : scenario.getAllEcm())
		{
			if (x->getName() == re_2.toStdString()) {
				scenario.getAllOwnPlatformEcmRelation()[num].setEcm(x);
				break;
			}
		}
		QMessageBox::about(this, tr("Tip"), tr("Save Relation successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatformEcmRelation");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(re);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(re_2);
	}
}
void PathPlanGui::save_Platform() {
	int num = ui.tableWidget_Platform->currentRow();
	QString a = ui.tableWidget_Platform->item(num, 0)->text();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_Platform->cellWidget(num, 1));
	int re = cell->currentIndex();
	if (num + 1 > scenario.getAllPlatform().size()) {
		sce::Platform new_data;
		new_data.setPlatformName(a.toStdString());
		new_data.setPlatformType(sce::PlatformType(re));
		scenario.addPlatform(make_shared<sce::Platform>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Platform successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Platform");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Type");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(platformtype_list[re]);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		second.appendChild(third_1);
		second.appendChild(third_2);
		root.appendChild(second);
	}
	else {
		scenario.getAllPlatform()[num]->setPlatformName(a.toStdString());
		scenario.getAllPlatform()[num]->setPlatformType(sce::PlatformType(re));
		QMessageBox::about(this, tr("Tip"), tr("Save Platform successfully"));
		QDomNodeList list = dom.elementsByTagName("Platform");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(a);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(platformtype_list[re]);
	}
}
void PathPlanGui::save_Weapon() {
	int num = ui.tableWidget_Weapon->currentRow();
	QString a = ui.tableWidget_Weapon->item(num, 0)->text();
	QString b = ui.tableWidget_Weapon->item(num, 1)->text();
	QString c = ui.tableWidget_Weapon->item(num, 2)->text();
	if (num + 1 > scenario.getAllWeapon().size()) {
		sce::Weapon new_data(a.toStdString(), b.toDouble(), c.toDouble());
		scenario.addWeapon(make_shared<sce::Weapon>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Weapon successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Weapon");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("CEPR");
		QDomElement third_3 = dom.createElement("WEAPONAREACOVERAGE");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		root.appendChild(second);
	}
	else {
		scenario.getAllWeapon()[num]->setName(a.toStdString());
		scenario.getAllWeapon()[num]->setCEPR(b.toInt());
		scenario.getAllWeapon()[num]->setWeaponAreaCoverage(c.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save Weapon successfully"));
		QDomNodeList list = dom.elementsByTagName("Weapon");
		int flag_weapon = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement e = list.at(i).toElement();
			if (e.parentNode().nodeName() == "Scenario")
			{
				if (flag_weapon == num)
				{
					e.firstChild().firstChild().setNodeValue(a);
					e.firstChild().nextSibling().firstChild().setNodeValue(b);
					e.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
					break;
				}
				flag_weapon++;
			}
		}
	}
}
void PathPlanGui::save_Site() {
	int num = ui.tableWidget_Site->currentRow();
	QString a = ui.tableWidget_Site->item(num, 0)->text();
	QString b = ui.tableWidget_Site->item(num, 1)->text();
	QString c = ui.tableWidget_Site->item(num, 2)->text();
	QString d = ui.tableWidget_Site->item(num, 3)->text();
	if (num + 1 > scenario.getAllSite().size()) {
		sce::Site new_data(a.toStdString(), b.toDouble(), c.toDouble(), d.toDouble());
		scenario.addSite(make_shared<sce::Site>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Site successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Site");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Altitude");
		QDomElement third_3 = dom.createElement("Latitude");
		QDomElement third_4 = dom.createElement("Longitude");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		root.appendChild(second);
	}
	else {
		scenario.getAllSite()[num]->setName(a.toStdString());
		scenario.getAllSite()[num]->setAltitude(b.toDouble());
		scenario.getAllSite()[num]->setLatitude(c.toDouble());
		scenario.getAllSite()[num]->setLongitude(d.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save Site successfully"));
		QDomNodeList list = dom.elementsByTagName("Site");
		int flag_site = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement e = list.at(i).toElement();
			if (e.parentNode().nodeName() == "Scenario")
			{
				if (flag_site == num)
				{
					e.firstChild().firstChild().setNodeValue(a);
					e.firstChild().nextSibling().firstChild().setNodeValue(b);
					e.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
					e.firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
					break;
				}
				flag_site++;
			}
		}
	}
}
void PathPlanGui::save_OwnPlatform() {
	int num = ui.tableWidget_OPlatform->currentRow();
	QString a = ui.tableWidget_OPlatform->item(num, 0)->text();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget_OPlatform->cellWidget(num, 1));
	int re = cell->currentIndex();
	QString b = ui.tableWidget_OPlatform->item(num, 2)->text();
	QString c = ui.tableWidget_OPlatform->item(num, 3)->text();
	QString d = ui.tableWidget_OPlatform->item(num, 4)->text();
	QString e = ui.tableWidget_OPlatform->item(num, 5)->text();
	QString f = ui.tableWidget_OPlatform->item(num, 6)->text();
	QString g = ui.tableWidget_OPlatform->item(num, 7)->text();
	if (num + 1 > scenario.getAllOwnPlatform().size())
	{
		sce::OwnPlatform new_data(a.toStdString(), sce::OwnPlatformType(re), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble(), f.toDouble(), g.toDouble());
		scenario.addOwnPlatform(make_shared<sce::OwnPlatform>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add OwnPlatform successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("OwnPlatform");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Type");
		QDomElement third_3 = dom.createElement("MaxAcceleration");
		QDomElement third_4 = dom.createElement("MaxDeceleration");
		QDomElement third_5 = dom.createElement("MaxClimbRate");
		QDomElement third_6 = dom.createElement("MaxDiveRate");
		QDomElement third_7 = dom.createElement("MaxSpeed");
		QDomElement third_8 = dom.createElement("MaxTurnRadius");
		QDomElement third_9 = dom.createElement("Mission");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(ownplatformtype_list[0]);
		QDomText text3 = dom.createTextNode(b);
		QDomText text4 = dom.createTextNode(c);
		QDomText text5 = dom.createTextNode(d);
		QDomText text6 = dom.createTextNode(e);
		QDomText text7 = dom.createTextNode(f);
		QDomText text8 = dom.createTextNode(g);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		third_6.appendChild(text6);
		third_7.appendChild(text7);
		third_8.appendChild(text8);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		second.appendChild(third_5);
		second.appendChild(third_6);
		second.appendChild(third_7);
		second.appendChild(third_8);
		second.appendChild(third_9);
		root.appendChild(second);
	}
	else {
		scenario.getAllOwnPlatform()[num]->setName(a.toStdString());
		scenario.getAllOwnPlatform()[num]->setType(sce::OwnPlatformType(re));
		scenario.getAllOwnPlatform()[num]->setMaxAcceleration(b.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxDeceleration(c.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxClimbRate(d.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxDiveRate(e.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxSpeed(f.toDouble());
		scenario.getAllOwnPlatform()[num]->setMaxTurnRadius(g.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save OwnPlatform successfully"));
		QDomNodeList list = dom.elementsByTagName("OwnPlatform");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					for (QDomNode OP_son = ele.firstChild(); !OP_son.isNull(); OP_son = OP_son.nextSibling())
					{
						if (OP_son.nodeName() == "Name")
						{
							OP_son.firstChild().setNodeValue(a);
						}
						if (OP_son.nodeName() == "Type")
						{
							OP_son.firstChild().setNodeValue(ownplatformtype_list[0]);
						}
						if (OP_son.nodeName() == "MaxAcceleration")
						{
							OP_son.firstChild().setNodeValue(b);
						}
						if (OP_son.nodeName() == "MaxDeceleration")
						{
							OP_son.firstChild().setNodeValue(c);
						}
						if (OP_son.nodeName() == "MaxClimbRate")
						{
							OP_son.firstChild().setNodeValue(d);
						}
						if (OP_son.nodeName() == "MaxDiveRate")
						{
							OP_son.firstChild().setNodeValue(e);
						}
						if (OP_son.nodeName() == "MaxSpeed")
						{
							OP_son.firstChild().setNodeValue(f);
						}
						if (OP_son.nodeName() == "MaxTurnRadius")
						{
							OP_son.firstChild().setNodeValue(g);
						}
					}
					/*ele.firstChild().firstChild().setNodeValue(a);
					ele.firstChild().nextSibling().firstChild().setNodeValue(ownplatformtype_list[0]);
					ele.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(b);
					ele.firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(c);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(e);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(f);
					ele.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(g);*/
					break;
				}
				flag++;
			}
		}
	}

}
void PathPlanGui::save_Ecm() {
	int num = ui.tableWidget_Ecm->currentRow();
	QString a = ui.tableWidget_Ecm->item(num, 0)->text();
	QString b = ui.tableWidget_Ecm->item(num, 1)->text();
	QString c = ui.tableWidget_Ecm->item(num, 2)->text();
	QString d = ui.tableWidget_Ecm->item(num, 3)->text();
	QString e = ui.tableWidget_Ecm->item(num, 4)->text();
	if (num + 1 > scenario.getAllEcm().size())
	{
		sce::Ecm new_data(a.toStdString(), b.toDouble(), c.toInt(), d.toInt(), e.toInt());
		scenario.addEcm(std::make_shared<sce::Ecm>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save Ecm successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Ecm");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Pt");
		QDomElement third_3 = dom.createElement("Gain");
		QDomElement third_4 = dom.createElement("Rfmin");
		QDomElement third_5 = dom.createElement("Rfmax");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		QDomText text5 = dom.createTextNode(e);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		second.appendChild(third_5);
		root.appendChild(second);
	}
	else {
		scenario.getAllEcm()[num]->setName(a.toStdString());
		scenario.getAllEcm()[num]->setPt(b.toDouble());
		scenario.getAllEcm()[num]->setGain(c.toInt());
		scenario.getAllEcm()[num]->setRfMin(d.toInt());
		scenario.getAllEcm()[num]->setRfMax(e.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save Ecm successfully"));
		QDomNodeList list = dom.elementsByTagName("Ecm");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "Name")
						{
							qd.firstChild().setNodeValue(a);
						}
						if (qd.nodeName() == "Pt")
						{
							qd.firstChild().setNodeValue(b);
						}
						if (qd.nodeName() == "Gain")
						{
							qd.firstChild().setNodeValue(c);
						}
						if (qd.nodeName() == "Rfmin")
						{
							qd.firstChild().setNodeValue(d);
						}
						if (qd.nodeName() == "Rfmax")
						{
							qd.firstChild().setNodeValue(e);
						}
					}
					break;
				}
				flag++;
			}
		}
	}
}
void PathPlanGui::save_ECMStrategy() {
	int num = ui.tableWidget_ECMStra->currentRow();
	QString a = ui.tableWidget_ECMStra->item(num, 0)->text();
	//vector<EsmStrategySection>
	sce::EcmStrategy new_data;
	new_data.setName(a.toStdString());
	int n = scenario.getAllEcmStrategy().size();
	if (num + 1 > n)
	{
		//sce::EcmStrategySection sce1;
		//sce::EcmStrategySection sce2;
		//std::vector<std::shared_ptr<sce::EcmStrategySection>>sce{ std::make_shared<sce::EcmStrategySection>(sce1), std::make_shared<sce::EcmStrategySection>(sce2) };
		//new_data.setAllPtr2Sections(sce);
		scenario.addEcmStrategy(std::make_shared<sce::EcmStrategy>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save EcmStrategy successfully"));
		QDomElement root = dom.documentElement();
		QDomElement es_node = dom.createElement("EcmStrategy");
		QDomElement name_node = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(a);
		name_node.appendChild(text1);;
		es_node.appendChild(name_node);
		root.appendChild(es_node);
	}
	else
	{
		scenario.getAllEcmStrategy()[num]->setName(a.toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save EcmStrategy successfully"));
		QDomNodeList list = dom.elementsByTagName("EcmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					ele.firstChild().firstChild().setNodeValue(a);
					break;
				}
				flag++;
			}
		}
	}
}
void PathPlanGui::save_ESMStrategy() {
	int num = ui.tableWidget_ESMStra->currentRow();
	QString a = ui.tableWidget_ESMStra->item(num, 0)->text();
	sce::EsmStrategy new_data(a.toStdString());
	int n = scenario.getAllEsmStrategy().size();
	if (num + 1 > n)
	{
		//sce::EsmStrategySection sce1;
		//sce::EsmStrategySection sce2;
		//std::vector<std::shared_ptr<sce::EsmStrategySection>>sce  {std::make_shared<sce::EsmStrategySection>(sce1), std::make_shared<sce::EsmStrategySection>(sce2)};
		//new_data.setAllPtr2Sections(sce);
		scenario.addEsmStrategy(std::make_shared<sce::EsmStrategy>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save EsmStrategy successfully"));
		QDomElement root = dom.documentElement();
		QDomElement es_node = dom.createElement("EsmStrategy");
		QDomElement name_node = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(a);
		name_node.appendChild(text1);;
		es_node.appendChild(name_node);
		root.appendChild(es_node);
	}
	else {
		scenario.getAllEsmStrategy()[num]->setName(a.toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save EsmStrategy successfully"));
		QDomNodeList list = dom.elementsByTagName("EsmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement e = list.at(i).toElement();
			if (e.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					e.firstChild().firstChild().setNodeValue(a);
					break;
				}
				flag++;
			}
		}
	}
}
void PathPlanGui::save_Esm() {

	int num = ui.tableWidget_Esm->currentRow();
	QString a = ui.tableWidget_Esm->item(num, 0)->text();
	QString b = ui.tableWidget_Esm->item(num, 1)->text();
	QString c = ui.tableWidget_Esm->item(num, 2)->text();
	QString d = ui.tableWidget_Esm->item(num, 3)->text();
	QString e = ui.tableWidget_Esm->item(num, 4)->text();
	QString f = ui.tableWidget_Esm->item(num, 5)->text();
	QString g = ui.tableWidget_Esm->item(num, 6)->text();
	if (num + 1 > scenario.getAllEsm().size())
	{
		sce::Esm new_data(a.toStdString(), b.toDouble(), c.toInt(), d.toInt(), e.toInt(), f.toInt(), g.toInt());
		scenario.addEsm(std::make_shared<sce::Esm>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save Esm successfully"));
		QDomElement root = dom.documentElement();
		QDomElement second = dom.createElement("Esm");
		QDomElement third_1 = dom.createElement("Name");
		QDomElement third_2 = dom.createElement("Dwell_Freq_Resolution");
		QDomElement third_3 = dom.createElement("Tuning_Step");
		QDomElement third_4 = dom.createElement("RfCovmin");
		QDomElement third_5 = dom.createElement("RfCovmax");
		QDomElement third_6 = dom.createElement("Num_Pulses_Acquisition");
		QDomElement third_7 = dom.createElement("Num_Pulses_Alarm");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		QDomText text5 = dom.createTextNode(e);
		QDomText text6 = dom.createTextNode(f);
		QDomText text7 = dom.createTextNode(g);
		third_1.appendChild(text1);
		third_2.appendChild(text2);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		third_6.appendChild(text6);
		third_7.appendChild(text7);
		second.appendChild(third_1);
		second.appendChild(third_2);
		second.appendChild(third_3);
		second.appendChild(third_4);
		second.appendChild(third_5);
		second.appendChild(third_6);
		second.appendChild(third_7);
		root.appendChild(second);
	}
	else {
		scenario.getAllEsm()[num]->setName(a.toStdString());
		scenario.getAllEsm()[num]->setDwellFreqResolution(b.toDouble());
		scenario.getAllEsm()[num]->setTuningStep(c.toInt());
		scenario.getAllEsm()[num]->setRfCovMin(d.toInt());
		scenario.getAllEsm()[num]->setRfCovMax(e.toInt());
		scenario.getAllEsm()[num]->setNumPulsesAcquisition(f.toInt());
		scenario.getAllEsm()[num]->setNumPulsesAlarm(g.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save Esm successfully"));
		QDomNodeList list = dom.elementsByTagName("Esm");
		for (QDomNode qd = list.at(num).firstChild(); !qd.isNull(); qd = qd.nextSibling())
		{
			if (qd.nodeName() == "Name")
			{
				qd.firstChild().setNodeValue(a);
			}
			if (qd.nodeName() == "Dwell_Freq_Resolution")
			{
				qd.firstChild().setNodeValue(b);
			}
			if (qd.nodeName() == "Tuning_Step")
			{
				qd.firstChild().setNodeValue(c);
			}
			if (qd.nodeName() == "RfCovmin")
			{
				qd.firstChild().setNodeValue(d);
			}
			if (qd.nodeName() == "RfCovmax")
			{
				qd.firstChild().setNodeValue(e);
			}
			if (qd.nodeName() == "Num_Pulses_Acquisition")
			{
				qd.firstChild().setNodeValue(f);
			}
			if (qd.nodeName() == "Num_Pulses_Alarm")
			{
				qd.firstChild().setNodeValue(g);
			}
		}
	}
}
void PathPlanGui::save_Emitter() {
	int num = ui.tableWidget_Emitter->currentRow();
	if (num + 1 > scenario.getAllEmitter().size()) {
		sce::Emitter new_data(ui.tableWidget_Emitter->item(num, 0)->text().toStdString());
		scenario.addEmitter(make_shared<sce::Emitter>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Emitter successfully"));

		QDomElement root = dom.documentElement();
		QDomElement ver = dom.createElement("Emitter");
		QDomElement third_1 = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(ui.tableWidget_Emitter->item(num, 0)->text());
		third_1.appendChild(text1);
		ver.appendChild(third_1);
		root.appendChild(ver);
	}
	else {
		scenario.getAllEmitter()[num]->setName(ui.tableWidget_Emitter->item(num, 0)->text().toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save Emitter successfully"));
		QDomNodeList list = dom.elementsByTagName("Emitter");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == num)
				{
					ele.firstChild().firstChild().setNodeValue(ui.tableWidget_Emitter->item(num, 0)->text());
					break;
				}
				flag++;
			}
		}
	}
}
void PathPlanGui::save_Vertex() {
	int num = ui.tableWidget_Vertex->currentRow();
	if (num + 1 > scenario.getAllVertex().size()) {
		QString a = ui.tableWidget_Vertex->item(num, 0)->text();
		QString b = ui.tableWidget_Vertex->item(num, 1)->text();
		QString c = ui.tableWidget_Vertex->item(num, 2)->text();
		sce::Vertex new_data(a.toDouble(), b.toDouble(),c.toDouble());
		scenario.addVertex(make_shared<sce::Vertex>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Add Vertex successfully"));
		QDomElement root = dom.documentElement();
		QDomElement ver = dom.createElement("Vertex");
		QDomElement la = dom.createElement("Latitude");
		QDomElement lo = dom.createElement("Longitude");
		QDomElement al = dom.createElement("Altitude");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		la.appendChild(text1);
		lo.appendChild(text2);
		al.appendChild(text3);
		ver.appendChild(la);
		ver.appendChild(lo);
		ver.appendChild(al);
		root.appendChild(ver);
	}
	else {
		QString a = ui.tableWidget_Vertex->item(num, 0)->text();
		QString b = ui.tableWidget_Vertex->item(num, 1)->text();
		QString c = ui.tableWidget_Vertex->item(num, 2)->text();
		scenario.getAllVertex()[num]->setLatitude(a.toDouble());
		scenario.getAllVertex()[num]->setLongitude(b.toDouble());
		scenario.getAllVertex()[num]->setAltitude(c.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save Vertex successfully"));
		QDomNodeList list = dom.elementsByTagName("Vertex");
		QDomElement e = list.at(num).toElement();
		e.firstChild().firstChild().setNodeValue(a);
		e.firstChild().nextSiblingElement().firstChild().setNodeValue(b);
		for (QDomNode son = e.firstChild(); !son.isNull(); son = son.nextSibling()) {
			if (son.nodeName() == "Latitude") son.firstChild().setNodeValue(a);
			if (son.nodeName() == "Longitude") son.firstChild().setNodeValue(b);
			if (son.nodeName() == "Altitude") son.firstChild().setNodeValue(c);
		}
	}
}
void PathPlanGui::add_RouteTab() {
	//设置对话框获取输入内容
	bool ok;
	QString d = QInputDialog::getText(this, tr("Tip"), tr("The name of tabpage："), QLineEdit::Normal, "Route", &ok);
	if (ok && !d.isEmpty()) {

		sce::Route new_route;
		new_route.setName(d.toStdString());
		scenario.addRoute(std::make_shared<sce::Route>(new_route));

		QDomElement root = dom.documentElement();
		QDomElement ver = dom.createElement("Route");
		QDomElement third_1 = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(d);
		third_1.appendChild(text1);
		ver.appendChild(third_1);
		root.appendChild(ver);

		//首先设置一个widget
		QWidget * tab_nn = new QWidget();
		QGridLayout* gridLayout_n = new QGridLayout();
		QPushButton *add = new QPushButton();
		QPushButton *del = new QPushButton();
		QPushButton *save = new QPushButton();
		add->setText("Add");
		del->setText("Del");
		save->setText("Save");
		QStringList headers;
		headers << QStringLiteral("Index") << QStringLiteral("Altitude") << QStringLiteral("Index") << QStringLiteral("Latitude") << QStringLiteral("Longitude") << QStringLiteral("Time") << QStringLiteral("Velocity") << QStringLiteral("Acceleration");
		//创建route的表格
		QTableWidget *new_table = new QTableWidget();
		new_table->setColumnCount(7);
		new_table->setLayoutDirection(Qt::LeftToRight);
		new_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		new_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		new_table->setDragEnabled(true);
		new_table->setSelectionBehavior(QAbstractItemView::SelectItems);
		new_table->setTextElideMode(Qt::ElideMiddle);
		new_table->horizontalHeader()->setCascadingSectionResizes(false);
		new_table->verticalHeader()->setVisible(true);
		new_table->setHorizontalHeaderLabels(headers);
		gridLayout_n->addWidget(add);
		gridLayout_n->addWidget(del);
		gridLayout_n->addWidget(save);
		gridLayout_n->addWidget(new_table);
		tab_nn->setLayout(gridLayout_n);
		//this->setCentralWidget(tab_nn);
		ui.tabWidget_Route->addTab(tab_nn, d);
		//测试tab类
		MyTab *tab = new MyTab();
		tab->add = add;
		tab->del = del;
		tab->save = save;
		tab->myindex = ui.tabWidget_Route->currentIndex();
		tab->my_table = new_table;
		vTab.push_back(tab);

		for (int i = 0; i < vTab.size(); i++)
		{
			connect(vTab[i]->add, &QPushButton::clicked, this, [=]()
			{
				vTab[i]->my_table->insertRow(vTab[i]->my_table->rowCount());

			});
			connect(vTab[i]->del, &QPushButton::clicked, this, [=]
			{
				int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
				if (ree == 0)
				{
					vTab[i]->my_table->removeRow(vTab[i]->my_table->currentRow());
				}
			});
			connect(vTab[i]->save, &QPushButton::clicked, this, [=]
			{
				int cur = vTab[i]->my_table->currentRow();
				QString a = vTab[i]->my_table->item(cur, 0)->text();
				QString b = vTab[i]->my_table->item(cur, 1)->text();
				QString c = vTab[i]->my_table->item(cur, 2)->text();
				QString d = vTab[i]->my_table->item(cur, 3)->text();
				QString e = vTab[i]->my_table->item(cur, 4)->text();
				QString f = vTab[i]->my_table->item(cur, 5)->text();
				QString g = vTab[i]->my_table->item(cur, 6)->text();
				if (cur + 1 > scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints().size())
				{
					sce::WayPoint new_wp(a.toInt(), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble(), f.toDouble(), g.toDouble());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->addWayPoint(new_wp);
					QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

					QDomElement third = dom.createElement("WayPoints");

					QDomElement fourth_1 = dom.createElement("Index");
					QDomElement fourth_2 = dom.createElement("Altitude");
					QDomElement fourth_3 = dom.createElement("Latitude");
					QDomElement fourth_4 = dom.createElement("Longitude");
					QDomElement fourth_5 = dom.createElement("Time");
					QDomElement fourth_6 = dom.createElement("Velocity");
					QDomElement fourth_7 = dom.createElement("Acceleration");

					QDomText text1 = dom.createTextNode(a);
					QDomText text2 = dom.createTextNode(b);
					QDomText text3 = dom.createTextNode(c);
					QDomText text4 = dom.createTextNode(d);
					QDomText text5 = dom.createTextNode(e);
					QDomText text6 = dom.createTextNode(f);
					QDomText text7 = dom.createTextNode(g);

					fourth_1.appendChild(text1);
					fourth_2.appendChild(text2);
					fourth_3.appendChild(text3);
					fourth_4.appendChild(text4);
					fourth_5.appendChild(text5);
					fourth_6.appendChild(text6);
					fourth_7.appendChild(text7);

					third.appendChild(fourth_1);
					third.appendChild(fourth_2);
					third.appendChild(fourth_3);
					third.appendChild(fourth_4);
					third.appendChild(fourth_5);
					third.appendChild(fourth_6);
					third.appendChild(fourth_7);

					QDomNodeList list = dom.elementsByTagName("Route");
					int flag = 0;
					for (int i = 0; i < list.count(); i++)
					{
						QDomElement ele = list.at(i).toElement();
						if (ele.parentNode().nodeName() == "Scenario")
						{
							if (flag == ui.tabWidget_Route->currentIndex())
							{
								ele.appendChild(third);
								break;
							}
							flag++;
						}
					}
				}
				else
				{
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setIndex(a.toInt());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setAltitude(b.toDouble());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setLatitude(c.toDouble());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setLongitude(d.toDouble());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setTime(e.toDouble());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setVelocity(f.toDouble());
					scenario.getAllRoute()[ui.tabWidget_Route->currentIndex()]->getAllWayPoints()[cur].setAcceleration(g.toDouble());
					QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

					QDomNodeList list = dom.elementsByTagName("Route");
					int flag = 0;
					for (int i = 0; i < list.count(); i++)
					{
						QDomElement ele = list.at(i).toElement();
						if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的Route
						{
							if (flag == ui.tabWidget_Route->currentIndex())
							{
								int flag_2 = 0;
								for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
								{
									if (qd.nodeName() == "WayPoints")//找到具体的WayPoints
									{
										if (flag_2 == ui.tabWidget_Route->currentIndex())
										{
											for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
											{
												if (qdd.nodeName() == "Index")
												{
													qdd.firstChild().setNodeValue(a);
												}
												if (qdd.nodeName() == "Altitude")
												{
													qdd.firstChild().setNodeValue(b);
												}
												if (qdd.nodeName() == "Latitude")
												{
													qdd.firstChild().setNodeValue(c);
												}
												if (qdd.nodeName() == "Longitude")
												{
													qdd.firstChild().setNodeValue(d);
												}
												if (qdd.nodeName() == "Time")
												{
													qdd.firstChild().setNodeValue(e);
												}
												if (qdd.nodeName() == "Velocity")
												{
													qdd.firstChild().setNodeValue(f);
												}
												if (qdd.nodeName() == "Acceleration")
												{
													qdd.firstChild().setNodeValue(g);
												}
											}
											break;
										}
										flag_2++;
									}
								}
								break;
							}
							flag++;
						}
					}
				}
			});

		}

	}
}
void PathPlanGui::del_RouteTab() {
	int num = ui.tabWidget_Route->currentIndex();
	int re = QMessageBox::information(this, "Tip", "Delete the current TAB?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (re == 0) {
		ui.tabWidget_Route->removeTab(num);
		if (num < scenario.getAllRoute().size()) {
			scenario.deleteRoute(num);
			QMessageBox::about(this, "Tip", "Delete Route successfully");

			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Platform");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						QMessageBox::about(this, "Tip", "Delete Route in file successfully");
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Vertex()
{
	int row_count = ui.tableWidget_Vertex->rowCount(); //获取表单行数
	ui.tableWidget_Vertex->insertRow(row_count);//添加新的一行

}
void PathPlanGui::del_Vertex()		//删除列表数据
{
	int num = ui.tableWidget_Vertex->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else
	{
		ui.tableWidget_Vertex->removeRow(num);
		if (num < scenario.getAllVertex().size())
		{
			scenario.deleteVertex(num);
			QMessageBox::about(this, "Tip", "Delete Vertex successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Vertex");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_PlatformSiteRelation(){
	int row_count = ui.tableWidget_PSR->rowCount(); //获取表单行数
	ui.tableWidget_PSR->insertRow(row_count);//添加新的一行
	ui.tableWidget_PSR->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_PSR->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_PSR->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_PSR->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList platformname_List;
	QStringList sitename_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllSite().size(); j++) {
		sitename_List.append(QString::fromStdString(scenario.getAllSite()[j]->getName()));
	}
	combox->addItems(platformname_List);
	combox_1->addItems(sitename_List);
}
void PathPlanGui::del_PlatformSiteRelation() {
	int num = ui.tableWidget_PSR->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_PSR->removeRow(num);
		if (num < scenario.getAllPlatformSiteRelation().size()) {
			scenario.deletePlatformSiteRelation(num);
			QMessageBox::about(this, "Tip", "Delete PlatformSiteRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("PlatformSiteRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_PlatformEmitterRelation() {
	int row_count = ui.tableWidget_PER->rowCount(); //获取表单行数
	ui.tableWidget_PER->insertRow(row_count);//添加新的一行
	ui.tableWidget_PER->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_PER->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_PER->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_PER->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList platformname_List;
	QStringList Emittername_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllEmitter().size(); j++) {
		Emittername_List.append(QString::fromStdString(scenario.getAllEmitter()[j]->getName()));
	}
	combox->addItems(platformname_List);
	combox_1->addItems(Emittername_List);
}
void PathPlanGui::del_PlatformEmitterRelation() {
	int num = ui.tableWidget_PER->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_PER->removeRow(num);
		if (num < scenario.getAllPlatformEmitterRelation().size()) {
			scenario.deletePlatformEmitterRelation(num);
			QMessageBox::about(this, "Tip", "Delete PlatformEmitterRealtion successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("PlatformEmitterRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_PlatformWeaponRelation() {
	int row_count = ui.tableWidget_PWR->rowCount(); //获取表单行数
	ui.tableWidget_PWR->insertRow(row_count);//添加新的一行
	ui.tableWidget_PWR->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_PWR->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_PWR->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_PWR->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList platformname_List;
	QStringList Weaponname_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllWeapon().size(); j++) {
		Weaponname_List.append(QString::fromStdString(scenario.getAllWeapon()[j]->getName()));
	}
	combox->addItems(platformname_List);
	combox_1->addItems(Weaponname_List);
}
void PathPlanGui::del_PlatformWeaponRelation() {
	int num = ui.tableWidget_PWR->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_PWR->removeRow(num);
		if (num < scenario.getAllPlatformWeaponRelation().size()) {
			scenario.deletePlatformWeaponRelation(num);
			QMessageBox::about(this, "Tip", "Delete PlatformWeaponRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("PlatformWeaponRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_OwnPlatformEsmRelation() {
	int row_count = ui.tableWidget_OEs->rowCount(); //获取表单行数
	ui.tableWidget_OEs->insertRow(row_count);//添加新的一行
	ui.tableWidget_OEs->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_OEs->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_OEs->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_OEs->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ownplatformname_List;
	QStringList Esmname_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		Ownplatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	combox->addItems(Ownplatformname_List);
	combox_1->addItems(Esmname_List);
}
void PathPlanGui::del_OwnPlatformEsmRelation() {
	int num = ui.tableWidget_OEs->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_OEs->removeRow(num);
		if (num < scenario.getAllOwnPlatformEsmRelation().size()) {
			scenario.deleteOwnPlatformEsmRelation(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatformEsmRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatformEsmRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_EsmEsmStrategyRelation() {
	int row_count = ui.tableWidget_EsmES->rowCount(); //获取表单行数
	ui.tableWidget_EsmES->insertRow(row_count);//添加新的一行
	ui.tableWidget_EsmES->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_EsmES->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_EsmES->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_EsmES->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Esmname_List;
	QStringList EsmStrategyname_List;
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsmStrategy().size(); j++) {
		EsmStrategyname_List.append(QString::fromStdString(scenario.getAllEsmStrategy()[j]->getName()));
	}
	combox->addItems(Esmname_List);
	combox_1->addItems(EsmStrategyname_List);
}
void PathPlanGui::del_EsmEsmStrategyRelation() {
	int num = ui.tableWidget_EsmES->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_EsmES->removeRow(num);
		if (num < scenario.getAllEsmEsmStrategyRelation().size()) {
			scenario.deleteEsmEsmStrategyRelation(num);
			QMessageBox::about(this, "Tip", "Delete EsmEsmStrategyRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EsmEsmStrategyRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_OwnPlatformRouteRelation() {
	int row_count = ui.tableWidget_ORR->rowCount(); //获取表单行数
	ui.tableWidget_ORR->insertRow(row_count);//添加新的一行
	ui.tableWidget_ORR->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_ORR->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_ORR->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_ORR->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ownplatformname_List;
	QStringList Routename_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		Ownplatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllRoute().size(); j++) {
		Routename_List.append(QString::fromStdString(scenario.getAllRoute()[j]->getName()));
	}
	combox->addItems(Ownplatformname_List);
	combox_1->addItems(Routename_List);
}
void PathPlanGui::del_OwnPlatformRouteRelation() {
	int num = ui.tableWidget_ORR->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_ORR->removeRow(num);
		if (num < scenario.getAllOwnPlatformRouteRelation().size()) {
			scenario.deleteOwnPlatformRouteRelation(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatformRouteRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatformRouteRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_OwnPlatformEcmRelation() {
	int row_count = ui.tableWidget_OPEcmR->rowCount(); //获取表单行数
	ui.tableWidget_OPEcmR->insertRow(row_count);//添加新的一行
	ui.tableWidget_OPEcmR->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_OPEcmR->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_OPEcmR->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_OPEcmR->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ownplatformname_List;
	QStringList Ecmname_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		Ownplatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcm().size(); j++) {
		Ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	combox->addItems(Ownplatformname_List);
	combox_1->addItems(Ecmname_List);
}
void PathPlanGui::del_OwnPlatformEcmRelation() {
	int num = ui.tableWidget_OPEcmR->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_OPEcmR->removeRow(num);
		if (num < scenario.getAllOwnPlatformEcmRelation().size()) {
			scenario.deleteOwnPlatformEcmRelation(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatformEcmRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatformEcmRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_EcmEcmStrategyRelation() {
	int row_count = ui.tableWidget_EcmES->rowCount(); //获取表单行数
	ui.tableWidget_EcmES->insertRow(row_count);//添加新的一行
	ui.tableWidget_EcmES->setCellWidget(row_count, 0, new QComboBox());
	ui.tableWidget_EcmES->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_EcmES->cellWidget(row_count, 0);
	auto cellWidget_1 = ui.tableWidget_EcmES->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
	QStringList Ecmname_List;
	QStringList EcmStrategyname_List;
	for (int j = 0; j < scenario.getAllEcm().size(); j++) {
		Ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcmStrategy().size(); j++) {
		EcmStrategyname_List.append(QString::fromStdString(scenario.getAllEcmStrategy()[j]->getName()));
	}
	combox->addItems(Ecmname_List);
	combox_1->addItems(EcmStrategyname_List);
}
void PathPlanGui::del_EcmEcmStrategyRelation() {
	int num = ui.tableWidget_EcmES->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_EcmES->removeRow(num);
		if (num < scenario.getAllEcmEcmStrategyRelation().size()) {
			scenario.deleteEcmEcmStrategyRelation(num);
			QMessageBox::about(this, "Tip", "Delete EcmEcmStrategyRelation successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EcmEcmStrategyRelation");
			QDomElement ele = list_node.at(num).toElement();
			root.removeChild(ele);
		}
	}
}
void PathPlanGui::add_EcmStrategy()
{
	QPushButton *add = new QPushButton();
	add->setText("View");
	int row_count = ui.tableWidget_ECMStra->rowCount();
	ui.tableWidget_ECMStra->insertRow(row_count);
	ui.tableWidget_ECMStra->setCellWidget(row_count, 1, add);
	connect(add, SIGNAL(clicked()), this, SLOT(show_ecmstrategy_section()));
}
void PathPlanGui::del_EcmStrategy()
{
	int num = ui.tableWidget_ECMStra->currentRow();
	int ree = QMessageBox::information(this, "", "Delete?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_ECMStra->removeRow(num);
		if (num < scenario.getAllEcmStrategy().size()) {
			scenario.deleteEcmStrategy(num);
			QMessageBox::about(this, "Tip", "Delete EcmStrategy successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EcmStrategy");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						QMessageBox::about(this, "Tip", "Delete EcmStrategy in file successfully");
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Esm()
{
	int row_count = ui.tableWidget_Esm->rowCount();
	ui.tableWidget_Esm->insertRow(row_count);

}
void PathPlanGui::del_Esm()
{
	int num = ui.tableWidget_Esm->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_Esm->removeRow(num);
		if (num < scenario.getAllEsm().size()) {
			scenario.deleteEsm(num);
			QMessageBox::about(this, "Tip", "Delete Esm successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Esm");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Ecm()
{
	int row_count = ui.tableWidget_Ecm->rowCount(); //获取表单行数
	ui.tableWidget_Ecm->insertRow(row_count);//添加新的一行
	QPushButton *bt = new QPushButton();
	bt->setText("tech");
	ui.tableWidget_Ecm->setCellWidget(row_count, 5, bt);
	connect(bt, SIGNAL(clicked()), this, SLOT(ecm_tech()));

}
void PathPlanGui::del_Ecm()
{
	int num = ui.tableWidget_Ecm->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_Ecm->removeRow(num);
		if (num < scenario.getAllEcm().size()) {
			scenario.deleteEcm(num);
			QMessageBox::about(this, "Tip", "Delete Ecm successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Ecm");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_EsmStrategy()
{
	QPushButton *add = new QPushButton();
	add->setText("View");
	int row_count = ui.tableWidget_ESMStra->rowCount();
	ui.tableWidget_ESMStra->insertRow(row_count);
	ui.tableWidget_ESMStra->setCellWidget(row_count, 1, add);
	connect(add, SIGNAL(clicked()), this, SLOT(show_esmstrategy_section()));
}
void PathPlanGui::del_EsmStrategy()
{
	int num = ui.tableWidget_ESMStra->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_ESMStra->removeRow(num);
		if (num < scenario.getAllEsmStrategy().size()) {
			scenario.deleteEsmStrategy(num);
			QMessageBox::about(this, "Tip", "Delete EsmStrategy successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EsmStrategy");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						QMessageBox::about(this, "Tip", "Delete EsmStrategy in file successfully");
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_OwnPlatform() {
	int num = ui.tableWidget_OPlatform->rowCount();
	ui.tableWidget_OPlatform->insertRow(num);
	QPushButton *btn = new QPushButton();
	btn->setText("veiw");
	ui.tableWidget_OPlatform->setCellWidget(num, 1, new QComboBox());
	auto cell = ui.tableWidget_OPlatform->cellWidget(num, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cell);
	QStringList list;
	list << "Air";
	combox->addItems(list);
	ui.tableWidget_OPlatform->setCellWidget(num, 8, btn);
	connect(btn, SIGNAL(clicked()), this, SLOT(show_mission()));
}
void PathPlanGui::del_OwnPlatform()
{
	int num = ui.tableWidget_OPlatform->currentRow();
	int ree = QMessageBox::information(this, "Tip", "Confirm delrtion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_OPlatform->removeRow(num);
		if (num < scenario.getAllOwnPlatform().size())
		{
			scenario.deleteOwnPlatform(num);
			QMessageBox::about(this, "Tip", "Delete OwnPlatform successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatform");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Site()
{
	int row_count = ui.tableWidget_Site->rowCount(); //获取表单行数
	ui.tableWidget_Site->insertRow(row_count);//添加新的一行
}
void PathPlanGui::del_Site()		//删除列表数据
{
	int num = ui.tableWidget_Site->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_Site->removeRow(num);
		if (num < scenario.getAllSite().size()) {
			scenario.deleteSite(num);
			QMessageBox::about(this, "Tip", "Delete Site successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Site");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Weapon()
{
	int row_count = ui.tableWidget_Weapon->rowCount(); //获取表单行数
	ui.tableWidget_Weapon->insertRow(row_count);//添加新的一行
}
void PathPlanGui::del_Weapon()		//删除列表数据
{
	int num = ui.tableWidget_Weapon->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_Weapon->removeRow(num);
		if (num < scenario.getAllWeapon().size()) {
			scenario.deleteWeapon(num);
			QMessageBox::about(this, "Tip", "Delete Weapon successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Weapon");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Platform()
{
	int row_count = ui.tableWidget_Platform->rowCount(); //获取表单行数
	ui.tableWidget_Platform->insertRow(row_count);//添加新的一行
	ui.tableWidget_Platform->setCellWidget(row_count, 1, new QComboBox());
	auto cellWidget = ui.tableWidget_Platform->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QStringList list;
	list << "Land" << "Air" << "Surface";
	combox->addItems(list);
}
void PathPlanGui::del_Platform()		//删除列表数据
{
	int num = ui.tableWidget_Platform->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else
	{
		ui.tableWidget_Platform->removeRow(num);
		if (num < scenario.getAllPlatform().size())
		{
			scenario.deletePlatform(num);
			QMessageBox::about(this, "Tip", "Delete Platform successfully");

			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Platform");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
void PathPlanGui::add_Emitter() {
	int row_count = ui.tableWidget_Emitter->rowCount(); //获取表单行数
	ui.tableWidget_Emitter->insertRow(row_count);//添加新的一行
	QPushButton *but = new QPushButton();
	but->setText("View");
	ui.tableWidget_Emitter->setCellWidget(row_count, 1, but);
	connect(but, SIGNAL(clicked()), this, SLOT(show_rada()));
}
void PathPlanGui::del_Emitter()		//删除列表数据
{
	int num = ui.tableWidget_Emitter->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget_Emitter->removeRow(num);
		if (num < scenario.getAllEmitter().size()) {
			scenario.deleteEmitter(num);
			QMessageBox::about(this, "Tip", "Delete Emitter successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Emitter");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == num)
					{
						root.removeChild(ele);
						break;
					}
					flag++;
				}
			}
		}
	}
}
//void PathPlanGui::on_currentChanged(const QModelIndex &current, const QModelIndex &previous)
//{ //选择单元格变化时的响应
//	if (current.isValid())
//	{
//		LabCellPos->setText(QString::asprintf("当前单元格：％d 行，％d 列", current.row(), current.column()));
//		QStandardItem* aItem = VertexModel->itemFromIndex(current);
//		this->LabCellText->setText("单元格内容：" + aItem->text());
//		QFont font = aItem->font();
//		//ui.actFontBold->setChecked(font.bold());
//	}
//}

void PathPlanGui::on_actOpen_triggered()
{
	//打开文件
	//QString str;
	QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
	qDebug() << curPath;
	//调用打开文件对话框打开一个文件
	QString aFileName = QFileDialog::getOpenFileName(this, tr("Select file"), curPath, tr("xml(*.xml);;All file(*.*)"));
	file.setFileName(aFileName);//以文件方式读出

	if (aFileName.isEmpty())
		return; //如果未选择文件，退出
	QStringList fFileContent;//文件内容字符串列表
	//QFile file(aFileName);  //以文件方式读出
	if (!file.open(QIODevice::ReadWrite))
		return;

	this->LabCurFile->setText("Current file：" + aFileName);//状态栏显示

	if(!dom.setContent(&file))
	{
		QMessageBox::about(this, tr("Tips"), tr("Failed to Open file！"));
		file.close();
		return;
		
		//QTextStream aStream(&file); //用文本流读取文件
		//ui.plainTextEdit->clear();//清空
		//while (!aStream.atEnd())
		//{
		//	QString str = aStream.readLine();//读取文件的一行
		//	ui.plainTextEdit->appendPlainText(str); //添加到文本框显示
		//	fFileContent.append(str); //添加到 StringList
		//}
		//
		//ui.actAppend->setEnabled(true); //更新Actions的enable属性
		//ui.actInsert->setEnabled(true);
		//ui.actDelete->setEnabled(true);
		//ui.actSave->setEnabled(true);
		//initModelFromScenario();//从Scenario的内容初始化数据模型
	}
	file.close();//关闭文件

	//再次打开xml文件需要清空之前实例化的scenario对象，释放Vector容器所占用的内存
	if (open_flag != 0)
	{
		std::vector<std::shared_ptr<sce::Platform>> platforms_temp;
		std::vector<std::shared_ptr<sce::OwnPlatform>> ownplatforms_temp;
		std::vector<std::shared_ptr<sce::Esm>> esms_temp;
		std::vector<std::shared_ptr<sce::Ecm>> ecms_temp;
		std::vector<std::shared_ptr<sce::Vertex>> vertexs_temp;
		std::vector<std::shared_ptr<sce::Weapon>> weapons_temp;
		std::vector<std::shared_ptr<sce::Site>> sites_temp;
		std::vector<std::shared_ptr<sce::Emitter>> emitters_temp;
		std::vector<std::shared_ptr<sce::Route>> routes_temp;
		std::vector<std::shared_ptr<sce::EsmStrategy>> esmstrategys_temp;
		std::vector<std::shared_ptr<sce::EcmStrategy>> ecmstrategys_temp;
		std::vector<sce::PlatformSiteRelation> PSRs_temp;
		std::vector<sce::PlatformEmitterRelation> PERs_temp;
		std::vector<sce::PlatformWeaponRelation> PWRs_temp;
		std::vector<sce::OwnPlatformEsmRelation> OPEsmRs_temp;
		std::vector<sce::EsmEsmStrategyRelation> EsmESRs_temp;
		std::vector<sce::OwnPlatformEcmRelation> OPEcmRs_temp;
		std::vector<sce::EcmEcmStrategyRelation> EcmESRs_temp;
		std::vector<sce::OwnPlatformRouteRelation> OPRRs_temp;

		scenario.getAllPlatform().swap(platforms_temp);
		scenario.getAllOwnPlatform().swap(ownplatforms_temp);
		scenario.getAllEsm().swap(esms_temp);
		scenario.getAllEcm().swap(ecms_temp);
		scenario.getAllVertex().swap(vertexs_temp);
		scenario.getAllWeapon().swap(weapons_temp);
		scenario.getAllSite().swap(sites_temp);
		scenario.getAllEmitter().swap(emitters_temp);
		scenario.getAllRoute().swap(routes_temp);
		scenario.getAllEsmStrategy().swap(esmstrategys_temp);
		scenario.getAllEcmStrategy().swap(ecmstrategys_temp);
		scenario.getAllPlatformSiteRelation().swap(PSRs_temp);
		scenario.getAllPlatformEmitterRelation().swap(PERs_temp);
		scenario.getAllPlatformWeaponRelation().swap(PWRs_temp);
		scenario.getAllOwnPlatformEsmRelation().swap(OPEsmRs_temp);
		scenario.getAllEsmEsmStrategyRelation().swap(EsmESRs_temp);
		scenario.getAllOwnPlatformEcmRelation().swap(OPEcmRs_temp);
		scenario.getAllEcmEcmStrategyRelation().swap(EcmESRs_temp);
		scenario.getAllOwnPlatformRouteRelation().swap(OPRRs_temp);
	}

	QMessageBox::about(this, tr("Tip"), tr("File opened successfully"));
	QDomElement docelem = dom.documentElement();
	listDom(docelem);

	emit sign_show_xml_data();

	//释放Vector容器所占用的内存
	//std::vector<std::shared_ptr<sce::Route>>().swap(routes);
	//std::vector<std::shared_ptr<sce::EsmStrategy>>().swap(esmstrategys);
	//std::vector<std::shared_ptr<sce::EcmStrategy>>().swap(ecmstrategys);
	//std::vector<sce::PlatformSiteRelation>().swap(PSRs);
	//std::vector<sce::PlatformEmitterRelation>().swap(PERs);
	//std::vector<sce::PlatformWeaponRelation>().swap(PWRs);
	//std::vector<sce::OwnPlatformEsmRelation>().swap(OPEsmRs);
	//std::vector<sce::EsmEsmStrategyRelation>().swap(EsmESRs);
	//std::vector<sce::OwnPlatformEcmRelation>().swap(OPEcmRs);
	//std::vector<sce::EcmEcmStrategyRelation>().swap(EcmESRs);
	//std::vector<sce::OwnPlatformRouteRelation>().swap(OPRRs);
}

void PathPlanGui::listDom(QDomElement &docelem)
{
	bool f_vertex(false), f_platform(false), f_emitter(false), f_esm(false), f_ecm(false), f_op(false), f_route(false), f_site(false), f_weapon(false), f_esms(false), f_ecms(false);
	bool f_psr(false), f_per(false), f_pwr(false), f_oesm(false), f_oecm(false), f_esmesms(false), f_ecmecms(false), f_orr(false);
	QDomNode node = docelem.firstChild();//子节点
	if (node.isNull())
	{
		return;
	}
	else
	{
		while (!node.toElement().isNull())
		{
			QDomElement element = node.toElement();//Converts a QDomNode into a QDomElement
			if (!element.isNull())
			{
				if (find_Vertex_data(element, scenario)) f_vertex = true;
				else if (find_Platform_data(element, scenario)) f_platform = true;
				else if (find_Emitter_data(element, scenario)) f_emitter = true;
				else if (find_Esm_data(element, scenario)) f_esm = true;
				else if (find_Ecm_data(element, scenario)) f_ecm = true;
				else if (find_EsmStrategy_data(element, scenario)) f_esms = true;
				else if (find_EcmStrategy_data(element, scenario)) f_ecms = true;
				else if (find_OwnPlatform_data(element, scenario)) f_op = true;
				else if (find_Route_data(element, scenario)) f_route = true;
				else if (find_Site_data(element, scenario)) f_site = true;
				else if (find_Weapon_data(element, scenario)) f_weapon = true;
				else if (find_PlatformSiteRelation_data(element, scenario)) f_psr = true;
				else if (find_PlatformEmitterRelation_data(element, scenario)) f_per = true;
				else if (find_PlatformWeaponRelation_data(element, scenario)) f_pwr = true;
				else if (find_OwnPlatformEsmRelation_data(element, scenario)) f_oesm = true;
				else if (find_EsmEsmStrategyRelation_data(element, scenario)) f_esmesms = true;
				else if (find_OwnPlatformEcmRelation_data(element, scenario)) f_oecm = true;
				else if (find_EcmEcmStrategyRelation_data(element, scenario)) f_ecmecms = true;
				else if (find_OwnPlatformRouteRelation_data(element, scenario)) f_orr = true;

				//listDom(element);
			}
			node = node.nextSibling();//将兄弟元素赋给他
		}

		if (!f_vertex)   qDebug() << "No Vertex data was found!";
		if (!f_platform) qDebug() << "No Platform data was found!";
		if (!f_emitter)  qDebug() << "No Emitter data was found!";
		if (!f_esm)      qDebug() << "No Esm data was found!";
		if (!f_ecm)      qDebug() << "No Ecm data was found!";
		if (!f_esms)     qDebug() << "No EsmStrategy data was found!";
		if (!f_ecms)     qDebug() << "No EcmStrategy data was found!";
		if (!f_op)       qDebug() << "No OwnPlatform data was found!";
		if (!f_route)    qDebug() << "No Route data was found!";
		if (!f_site)     qDebug() << "No Site data was found!";
		if (!f_weapon)   qDebug() << "No Weapon data was found!";
		if (!f_psr) 	 qDebug() << "No PlatformSiteRelation data was found!";
		if (!f_per) 	 qDebug() << "No PlatformEmitterRelation data was found!";
		if (!f_pwr) 	 qDebug() << "No PlatformWeaponRelation data was found!";
		if (!f_oesm)     qDebug() << "No OwnPlatformEsmRelation data was found!";
		if (!f_esmesms)  qDebug() << "No EsmEsmStrategyRelation data was found!";
		if (!f_oecm)     qDebug() << "No OwnPlatformEcmRelation data was found!";
		if (!f_ecmecms)  qDebug() << "No EcmEcmStrategyRelation data was found!";
		if (!f_orr)      qDebug() << "No OwnPlatformRouteRelation data was found!";

		qDebug() << "Data import successfully!";

		//根据位置获取每个site上的Emitter,Weapon
		for (size_t i = 0; i < scenario.getAllSite().size(); ++i)
		{
			auto iterS = scenario.getAllSite().at(i);
			for (size_t j = 0; j < scenario.getAllPlatformSiteRelation().size(); ++j)
			{
				//获取 site-weapon键值对，无序且允许重复关键值
				auto iterPSR = scenario.getAllPlatformSiteRelation().at(j);
				if (iterPSR.getSiteName() == iterS->getName())
				{
					for (size_t z = 0; z < scenario.getAllPlatformWeaponRelation().size(); ++z)
					{
						auto iterPWR = scenario.getAllPlatformWeaponRelation().at(z);
						if (iterPSR.getPlatformName() == iterPWR.getPlatformName())
						{
							scenario.insertSiteWeaponRelation(iterS, iterPWR.getWeapon());
						}
					}
					//获取 site-emitter键值对，无序且允许重复关键值
					for (size_t z = 0; z < scenario.getAllPlatformEmitterRelation().size(); ++z)
					{
						auto iterPWR = scenario.getAllPlatformEmitterRelation().at(z);
						if (iterPSR.getPlatformName() == iterPWR.getPlatformName())
						{
							scenario.insertSiteEmitterRelation(iterS, iterPWR.getEmitter());
						}
					}
				}
			}
		}
	}
}

void PathPlanGui::setOpenFileFlag()
{
	open_flag += 1;
}

//void PathPlanGui::initModelFromScenario()
//{
//	size_t rowCnt = scenario.getAllVertex().size();
//	VertexModel->setRowCount(rowCnt);//设置行数
//	VertexModel->setColumnCount(3);//根据Vertex的属性字段设置列数
//	//设置表头
//	QStringList header_vertex{ QStringLiteral("Longitude"), QStringLiteral("Latitude"), QStringLiteral("Altitude") };//第1行是表头
//	VertexModel->setHorizontalHeaderLabels(header_vertex);
//
//	//设置表格数据
//	QString aText;
//	QStringList tmpList;
//	QStandardItem *aItem_long, *aItem_la,*aItem_al;
//
//	for (size_t i = 0; i < rowCnt; i++)
//	{
//		aItem_long = new QStandardItem(QString::number(scenario.getAllVertex()[i]->getLongitude()));//创建item
//		VertexModel->setItem(i, 0, aItem_long); //为模型的某个行列位置设置Item
//
//		aItem_la = new QStandardItem(QString::number(scenario.getAllVertex()[i]->getLatitude()));//创建item
//		VertexModel->setItem(i, 1, aItem_la); //为模型的某个行列位置设置Item
//
//		aItem_al = new QStandardItem(QString::number(scenario.getAllVertex()[i]->getAltitude()));//创建item
//		VertexModel->setItem(i, 2, aItem_al); //为模型的某个行列位置设置Item
//	}	
//
//}

void PathPlanGui::show_xml_data()
{	
	
	show_Vertex_data();
	show_Platform_data();
	show_Emitter_data();
	show_Weapon_data();
	show_Site_data();
	show_OwnPlatform_data();
	show_Ecm_data();
	show_Esm_data();
	show_EsmStratgy_data();
	show_EcmStratgy_data();
	show_Route_data();
	show_PSRs_data();
	show_PERs_data();
	show_PWRs_data();
	show_OPEsmRs_data();
	show_EsmESRs_data();
	show_OPEcmRs_data();
	show_EcmESRs_data();
	show_OPRRs_data();

	//提供平台选择下拉组合框
	if (ui.comboBox_OPS->count()>0)	
		ui.comboBox_OPS->clear();
	if (ui.comboBox_OPSel->count()>0) 
		ui.comboBox_OPSel->clear();

	//QStringList ownPlatformlist;
	for (auto &iter: scenario.getAllOwnPlatform())
	{
		//ownPlatformlist.append(QString::fromStdString(iter->getName()));
		ui.comboBox_OPS->addItem(QString::fromStdString(iter->getName()));
		ui.comboBox_OPSel->addItem(QString::fromStdString(iter->getName()));
	}
	//ui.comboBox_OPS->addItems(ownPlatformlist);//QComboBox默认阻止插入相同的内容
	//ui.comboBox_OPSel->addItems(ownPlatformlist);
}

//void PathPlanGui::on_actAppend_triggered()
//{ //在表格最后添加行
//	QList<QStandardItem*>    aItemList; //容器类 
//	QStandardItem   *aItem;
//	for (int i = 0; i<2 - 1; i++) //不包含最后1列
//	{
//		aItem = new QStandardItem("0"); //创建Item
//		aItemList << aItem;   //添加到容器
//	}
//	//获取最后一列的表头文字
//	//QString str = VertexModel->headerData(VertexModel->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
//	//aItem = new QStandardItem(str); //创建 "按钮"Item
//	//aItem->setCheckable(true);
//	//aItemList << aItem;   //添加到容器
//	VertexModel->insertRow(VertexModel->rowCount(), aItemList); //插入一行，需要每个Cell的Item
//	QModelIndex curIndex = VertexModel->index(VertexModel->rowCount() - 1, 0);//创建最后一行的ModelIndex
//	theSelection->clearSelection();//清空选择项
//	theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);//设置刚插入的行为当前选择行
//}

//void PathPlanGui::on_actInsert_triggered()
//{ //在表格最后添加行
	//QList<QStandardItem*>    aItemList; //容器类
	//QStandardItem   *aItem;
	//for (int i = 0; i<2 - 1; i++) //不包含最后1列
	//{
	//	aItem = new QStandardItem("0"); //创建Item
	//	aItemList << aItem;   //添加到容器
	//}
	//获取最后一列的表头文字
	//QString str = VertexModel->headerData(VertexModel->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
	//aItem = new QStandardItem(str); //创建 "按钮"Item
	//aItem->setCheckable(true);
	//aItemList << aItem;   //添加到容器
	//VertexModel->insertRow(theSelection->currentIndex().row(), aItemList); //插入一行，需要每个Cell的Item
	//QModelIndex curIndex = VertexModel->index(theSelection->currentIndex().row() , 0);//创建最后一行的ModelIndex
	//theSelection->clearSelection();//清空选择项
	//theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);//设置刚插入的行为当前选择行
//}

//void PathPlanGui::on_actDelete_triggered()
//{ //删除行
//	//QModelIndex curIndex = theSelection->currentIndex();//获取模型索引
//	//if (curIndex.row() == VertexModel->rowCount() - 1) //最后一行
//	//	VertexModel->removeRow(curIndex.row()); //删除最后一行
//	//else {
//	//	VertexModel->removeRow(curIndex.row());//删除一行，并重新设置当前选择行
//	//	theSelection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
//	//}
//}

void PathPlanGui::on_actSave_triggered()
{
	//TODO::这里添加场景数据导入xml操作
}

void PathPlanGui::show_rada()
{
	choice_emitter = ui.tableWidget_Emitter->currentRow();
	qDebug() << "choice_emitter->" << choice_emitter << endl;
	emit rada_mode();
}

void PathPlanGui::show_mission()
{
	choice_ownplatform = ui.tableWidget_OPlatform->currentRow();
	qDebug() << "choice_ownplatform:" << choice_ownplatform;
	emit sign_mission();
}

void PathPlanGui::ecm_tech()
{
	choice_ecm = ui.tableWidget_Ecm->currentRow();
	emit sign_ecm_tech();
}

void PathPlanGui::show_esmstrategy_section()
{
	choice_esmstrategy = ui.tableWidget_ESMStra->currentRow();
	qDebug() << "choice_esmstrategy" << choice_esmstrategy;
	emit sign_esmstrategy_section();
}

void PathPlanGui::show_ecmstrategy_section()
{
	choice_ecmstrategy = ui.tableWidget_ECMStra->currentRow();
	qDebug() << "choice_ecmstrategy" << choice_ecmstrategy;
	emit sign_ecmstrategy_section();
}

void PathPlanGui::setLabelText(int index)
{
	if (index>=0 && index<scenario.getAllOwnPlatform().size())
	{
		if (scenario.getAllOwnPlatform()[index]->getMission().getMissionType() == sce::MissionType::STRIKE)
		{
			ui.label_OPMT->setText(tr("Strike"));
		}
		else if (scenario.getAllOwnPlatform()[index]->getMission().getMissionType() == sce::MissionType::SUPPORT)
		{
			ui.label_OPMT->setText(tr("Support"));
		}
	}
	
}

void PathPlanGui::setLabelText1(int index)
{
	if (index >= 0 && index < scenario.getAllOwnPlatform().size())
	{
		if (scenario.getAllOwnPlatform()[index]->getMission().getMissionType() == sce::MissionType::STRIKE)
		{
			ui.label_OMissionT->setText(tr("Strike"));
		}
		else if (scenario.getAllOwnPlatform()[index]->getMission().getMissionType() == sce::MissionType::SUPPORT)
		{
			ui.label_OMissionT->setText(tr("Support"));
		}
	}
}

void PathPlanGui::setSeleteRoute(int index)
{
	if (index>=0 && index<scenario.getAllOwnPlatform().size())
	{
		if (ui.comboBox_RSel->count()>0)
		{
			ui.comboBox_RSel->clear();
		}
		//QStringList opRoutelist;
		for (auto &iter : scenario.getAllOwnPlatformRouteRelation())
		{
			if (scenario.getAllOwnPlatform()[index]->getName() == iter.getOwnPlatformName())
			{
				ui.comboBox_RSel->addItem(QString::fromStdString(iter.getRouteName()));
				//opRoutelist.append(QString::fromStdString(iter.getRouteName()));
			}
		}
	}
		
	//ui.comboBox_RSel->addItems(opRoutelist);
}

void PathPlanGui::run_algorithm()
{
	//获取当前OwnPlatform的索引
	int op_index = ui.comboBox_OPS->currentIndex();
	QString tips("Select the: ");
	tips.append(QString::fromStdString(scenario.getAllOwnPlatform()[op_index]->getName()));
	tips.append("\nNew route name: ");
	tips.append(ui.lineEdit_Rnew->text());
	int op_select = QMessageBox::information(this, "Tip", tips, QStringLiteral("Yes"), QStringLiteral("No"));
	if (op_select != 0)
	{
		return;
	}
	else {
		int tab_index = ui.tabWidget->currentIndex();
		double Lud = ui.lineEdit_Lud->text().toDouble();
		double Ldu = ui.lineEdit_Ldu->text().toDouble();
		double Ldt = ui.lineEdit_Ldt->text().toDouble();
		double Ltd = ui.lineEdit_Ltd->text().toDouble();
		double Lte = ui.lineEdit_Lte->text().toDouble();
		double Let = ui.lineEdit_Let->text().toDouble();
		double Leh = ui.lineEdit_Leh->text().toDouble();

		CofRada.insert(std::make_pair("Sud", Lud));
		CofRada.insert(std::make_pair("Sdu", Ldu));
		CofRada.insert(std::make_pair("Sdt", Ldt));
		CofRada.insert(std::make_pair("Std", Ltd));
		CofRada.insert(std::make_pair("Ste", Lte));
		CofRada.insert(std::make_pair("Set", Let));
		CofRada.insert(std::make_pair("Seh", Leh));

		//根据威胁位置获取每个威胁的最大武器射程
		std::vector<double> wcrange(scenario.getAllSite().size(), 0.0);
		
		assert(scenario.getAllSite().size() > 0);
		assert(scenario.getAllPlatformSiteRelation().size() > 0);
		for (size_t i = 0; i < scenario.getAllSite().size(); ++i)
		{
			auto iterS = scenario.getAllSite().at(i);
			std::vector<unsigned int> siteTmp;
			for (size_t j = 0; j < scenario.getAllPlatformSiteRelation().size(); ++j)
			{
				auto iterPSR = scenario.getAllPlatformSiteRelation().at(j);
				if (iterPSR.getSiteName() == iterS->getName())
				{
					for (size_t z = 0; z < scenario.getAllPlatformWeaponRelation().size(); ++z)
					{
						auto iterPWR = scenario.getAllPlatformWeaponRelation().at(z);
						if (iterPSR.getPlatformName() == iterPWR.getPlatformName())
						{
							siteTmp.push_back(iterPWR.getWeapon()->getWeaponAreaCoverage());
						}
					}
				}
			}
			assert(siteTmp.size() >= 0);
			wcrange[i] = siteTmp.size() > 0 ? *std::max_element(siteTmp.cbegin(), siteTmp.cend()) : 0.0;
			auto ret = swRelation.insert(std::make_pair(iterS, wcrange[i]));
			assert(ret.second);
		}
		//获取路径片段的起始终止点序列
		size_t target_size = scenario.getAllOwnPlatform()[op_index]->getMission().getAllTargetPoints().size();
		assert(target_size);
		std::vector<sce::Point> mission_section{ scenario.getAllOwnPlatform()[op_index]->getMission().getStartPoint() ,scenario.getAllOwnPlatform()[op_index]->getMission().getEndPoint() };
		if (target_size > 0)
		{
			for (size_t i = 0; i < target_size; ++i)
			{
				mission_section.insert(mission_section.end() - 1, scenario.getAllOwnPlatform()[op_index]->getMission().getTargetPoint(i));
			}
		}


		//scenario.getAllOwnPlatform->at(0);

		if (tab_index == 0) //choose a* algorithm
		{


			float survice_w1 = ui.lineEdit_SurW->text().toFloat();
			float end_w1 = ui.lineEdit_TarW->text().toFloat();
			float StepLength1 = ui.lineEdit_StepL->text().toFloat();
			float hmin1 = ui.lineEdit_minLH->text().toFloat();
			float hmax1 = ui.lineEdit_maxLH->text().toFloat();
			float horizontal_corner1 = ui.lineEdit_HorCor->text().toFloat();
			float verticality_corner1 = ui.lineEdit_VerCor->text().toFloat();
			float e_w1 = ui.lineEdit_EW->text().toFloat();
			float start_w1 = ui.lineEdit_StartW->text().toFloat();

			int ree = QMessageBox::information(this, "Tip", "Choose a* algorithm ?", QStringLiteral("Yes"), QStringLiteral("No"));

			QVector<Rada*> radav;
			for (auto x : swRelation)
			{
				auto site = x.first;
				auto weapon_cov = x.second;
				Rada Rada2(2, site->getLongitude(), site->getLatitude(), site->getAltitude(), weapon_cov, 1);
				radav.append(&Rada2);
			}

			std::vector<sce::Point> mission_section1;
			sce::Point p[3];
			for (int i = 0; i < 3; i++)
			{
				p[i].setAltitude(1);
				p[i].setLongitude(10 + i * 100);
				p[i].setLatitude(20 + i * 50);
				mission_section1.push_back(p[i]);
			}

			sce::Route route;

			if (ree != 0)
			{
				return;
			}
			else {
				qDebug() << "choice is  A*";
				for (int i = 0; i < mission_section1.size() - 2; i++)
				{
					APoint sp(mission_section1[i].getLongitude(), mission_section1[i].getLatitude(), mission_section1[i].getAltitude(), 0, 0, 0, 0, 0);
					APoint tp(mission_section1[i].getLongitude(), mission_section1[i + 1].getLatitude(), mission_section1[i].getAltitude(), 0, 0, 0, 0, 0);
					APoint ep(mission_section1[i + 2].getLongitude(), mission_section1[i + 2].getLatitude(), mission_section1[i + 2].getAltitude(), 0, 0, 0, 0, 0);
					Mission_G mg(1, mission_section1[i + 1].getLongitude(), mission_section1[i + 1].getLatitude(), mission_section1[i + 1].getAltitude(), 2, 0.25);
					A_STAR a(sp, tp, ep, radav, mg, e_w1, survice_w1, start_w1, end_w1, horizontal_corner1, verticality_corner1, hmin1, hmax1, StepLength1);

					for (int i = 0; i < a.result_point.size(); i++)
					{
						route.addWayPoint(sce::WayPoint(i, a.result_point[i]->X, a.result_point[i]->Y, a.result_point[i]->Z));
					}

				}
				auto rt = std::make_shared<sce::Route>(route);
				scenario.addRoute(rt);
				qDebug() << " A* complete";

				//RouteProb = markov_init(1, rt, swRelation, CofRada);
				//isfinished = true;
				//cout << RouteProb;

			}
		}
		if (tab_index == 1) //choose DE algorithm
		{
			size_t Population_Number = ui.lineEdit_Popnum->text().toInt();
			size_t Initial_Node_Number = ui.lineEdit_IniVnum->text().toInt();
			size_t Evolution_Number = ui.lineEdit_ENum->text().toInt();
			double Weight = ui.lineEdit_WF->text().toDouble();
			double Cross_Probability = ui.lineEdit_CP->text().toDouble();
			int ree = QMessageBox::information(this, "Tip", "Choose DE algorithm ?", QStringLiteral("Yes"), QStringLiteral("No"));
			if (ree != 0)
			{
				return;
			}
			else {
				qDebug() << "choice is DE";
				sce::Route_ptr route{ std::make_shared<sce::Route>(sce::Route(ui.lineEdit_Rnew->text().toStdString(),sce::WayPoint(mission_section[0].getLongitude(),mission_section[0].getLatitude(),mission_section[0].getAltitude()))) };

				for (size_t i = 0; i < target_size; ++i)
				{
					de::NVectorPtr route_section(de::De_alg(swRelation, scenario.getAllVertex(), mission_section[i], mission_section[i + 1], Population_Number, Initial_Node_Number, Evolution_Number, Weight, Cross_Probability));

					for (size_t iter = 1; iter < route_section->size(); ++iter)
					{
						de::Node node(route_section->at(iter));
						route->addWayPoint(sce::WayPoint(iter, node.longitude(), node.latitude(), node.altitude()));
					}
				}

				scenario.addRoute(route);
				qDebug() << "DE complete!";
				//MatrixXd stateprob = markov_init(1, route, swRelation, CofRada);
			}
		}
		//if (tab_index == 2) //choose PSO algorithm
		//{
		//	double Swarm_Size = ui.lineEdit_50->text().toDouble();
		//	double Loop_Couner = ui.lineEdit_51->text().toDouble();
		//	double Search_Step = ui.lineEdit_57->text().toDouble();
		//	double Eecute_Step = ui.lineEdit_58->text().toDouble();
		//	double Step_Distance = ui.lineEdit_59->text().toDouble();
		//	double Detect_Range = ui.lineEdit_61->text().toDouble();
		//	double Pitch = ui.lineEdit_60->text().toDouble();
		//	double Yaw = ui.lineEdit_62->text().toDouble();
		//	double RefPath = ui.lineEdit_55->text().toDouble();
		//	double OilCost = ui.lineEdit_53->text().toDouble();
		//	double Missions = ui.lineEdit_56->text().toDouble();
		//	double HightConstrain = ui.lineEdit_54->text().toDouble();
		//	double SurvivalCost = ui.lineEdit_52->text().toDouble();
		//	int ree = QMessageBox::information(this, "Tip", "Choose PSO algorithm ?", QStringLiteral("Yes"), QStringLiteral("No"));
		//	if (ree != 0)
		//	{
		//		return;
		//	}
		//	else {
		//		qDebug() << "choice is PSO";
		//	}
		//}
	}
}

void PathPlanGui::route_evaluate()
{
	//size_t route_index = ui.comboBox_RSel->currentIndex();
	for (auto item : scenario.getAllRoute())
	{
		if (QString::fromStdString(item->getName())== ui.comboBox_RSel->currentText())
		{
			//TODO::这里进行路径评估
			MatrixXd RouteProb = markov_init(1, item, swRelation, CofRada);
			isfinished = true;
			//cout << RouteProb;
			draw_survival_rate(RouteProb);
		}
	}

}

void PathPlanGui::draw_survival_rate(MatrixXd stateProbs)
{
	QBarSet *set0 = new QBarSet("U");
	QBarSet *set1 = new QBarSet("D");
	QBarSet *set2 = new QBarSet("T");
	QBarSet *set3 = new QBarSet("E");
	QBarSet *set4 = new QBarSet("H");
	for (int i = 0; i < stateProbs.cols(); i++)
	{
		*set0 << stateProbs(0, i);
		*set1 << stateProbs(1, i);
		*set2 << stateProbs(2, i);
		*set3 << stateProbs(3, i);
		*set4 << stateProbs(4, i);
	}

	QPercentBarSeries *series = new QPercentBarSeries();
	series->append(set0);
	series->append(set1);
	series->append(set2);
	series->append(set3);
	series->append(set4);

	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle("Simple percentbarchart example");
	chart->setAnimationOptions(QChart::SeriesAnimations);

	QValueAxis *axisY = new QValueAxis();
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);

	QChartView *chartView = new QChartView(chart, ui.graphicsView_pre);
	chartView->setRenderHint(QPainter::Antialiasing);
	//chartView->show();
	ui.graphicsView_pre->show();

	//AreaChart
	//![1]
	QLineSeries *qlseries0 = new QLineSeries();
	QLineSeries *qlseries1 = new QLineSeries();
	QLineSeries *qlseries2 = new QLineSeries();
	QLineSeries *qlseries3 = new QLineSeries();
	QLineSeries *qlseries4 = new QLineSeries();
	//![1]

	//![2]
	for (int i = 0; i < stateProbs.cols(); i++)
	{
		*qlseries0 << QPointF(i, stateProbs(0, i));
		*qlseries1 << QPointF(i, stateProbs(0, i) + stateProbs(1, i));
		*qlseries2 << QPointF(i, stateProbs(0, i) + stateProbs(1, i) + stateProbs(2, i));
		*qlseries3 << QPointF(i, stateProbs(0, i) + stateProbs(1, i) + stateProbs(2, i) + stateProbs(3, i));
		*qlseries4 << QPointF(i, stateProbs(0, i) + stateProbs(1, i) + stateProbs(2, i) + stateProbs(3, i) + stateProbs(4, i));
	}
	//![2]

	//![3]
	QAreaSeries *qaseries0 = new QAreaSeries(qlseries0);
	QAreaSeries *qaseries1 = new QAreaSeries(qlseries0, qlseries1);
	QAreaSeries *qaseries2 = new QAreaSeries(qlseries1, qlseries2);
	QAreaSeries *qaseries3 = new QAreaSeries(qlseries2, qlseries3);
	QAreaSeries *qaseries4 = new QAreaSeries(qlseries3, qlseries4);
	qaseries0->setName("Undetected");
	qaseries1->setName("Detected");
	qaseries2->setName("Tracked");
	qaseries3->setName("Engaged");
	qaseries4->setName("Hit");

	QPen pen0(Qt::black), pen1(Qt::black), pen2(Qt::black), pen3(Qt::black), pen4(Qt::black);
	pen0.setWidth(3);
	pen1.setWidth(3);
	pen2.setWidth(3);
	pen3.setWidth(3);
	pen4.setWidth(3);
	qaseries0->setPen(pen0);
	qaseries1->setPen(pen1);
	qaseries2->setPen(pen2);
	qaseries3->setPen(pen3);
	qaseries4->setPen(pen4);

	QLinearGradient gradient0(QPointF(0, 0), QPointF(0, 1));
	gradient0.setColorAt(0.0, Qt::white);
	gradient0.setColorAt(1.0, Qt::white);
	gradient0.setCoordinateMode(QGradient::ObjectBoundingMode);
	qaseries0->setBrush(gradient0);

	QLinearGradient gradient1(QPointF(0, 0), QPointF(0, 1));
	gradient1.setColorAt(0.0, Qt::yellow);
	gradient1.setColorAt(1.0, Qt::yellow);
	gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
	qaseries1->setBrush(gradient1);

	QLinearGradient gradient2(QPointF(0, 0), QPointF(0, 1));
	gradient2.setColorAt(0.0, Qt::red);
	gradient2.setColorAt(1.0, Qt::red);
	gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
	qaseries2->setBrush(gradient2);

	QLinearGradient gradient3(QPointF(0, 0), QPointF(0, 1));
	gradient3.setColorAt(0.0, Qt::gray);
	gradient3.setColorAt(1.0, Qt::gray);
	gradient3.setCoordinateMode(QGradient::ObjectBoundingMode);
	qaseries3->setBrush(gradient3);

	QLinearGradient gradient4(QPointF(0, 0), QPointF(0, 1));
	gradient4.setColorAt(0.0, Qt::darkGray);
	gradient4.setColorAt(1.0, Qt::darkGray);
	gradient4.setCoordinateMode(QGradient::ObjectBoundingMode);
	qaseries4->setBrush(gradient4);
	//![3]

	//![4]
	QChart *achart = new QChart();
	achart->addSeries(qaseries0);
	achart->addSeries(qaseries1);
	achart->addSeries(qaseries2);
	achart->addSeries(qaseries3);
	achart->addSeries(qaseries4);
	achart->setTitle("Survival rate area map");
	//achart->createDefaultAxes();
	//achart->axes(Qt::Horizontal).first()->setRange(0, 20);
	//achart->axes(Qt::Vertical).first()->setRange(0, 10);
	//![4]

	//![5]
	QChartView *achartView = new QChartView(achart, ui.graphicsView_last);
	achartView->setRenderHint(QPainter::Antialiasing);

	ui.graphicsView_last->show();
	//ui.graphicsView = achartView;
	//this->setCentralWidget(ui.graphicsView);
}

void PathPlanGui::backTab()
{
	if (ui.SceTab->count() > 1 && ui.SceTab->currentIndex() > 0)
	{
		ui.SceTab->setCurrentIndex(ui.SceTab->currentIndex() -1);
	}
	else if(ui.SceTab->count() > 1 && ui.SceTab->currentIndex() == 0 )
	{
		ui.SceTab->setCurrentIndex(ui.SceTab->count()-1);
	}
}

void PathPlanGui::nextTab()
{
	if (ui.SceTab->count() > 1 && ui.SceTab->currentIndex() < (ui.SceTab->count() - 1))
	{
		ui.SceTab->setCurrentIndex(ui.SceTab->currentIndex() + 1);
	}
	else if (ui.SceTab->count() > 1 && ui.SceTab->currentIndex() >= (ui.SceTab->count() -1))
	{
		ui.SceTab->setCurrentIndex(0);
	}
}

void PathPlanGui::show_Vertex_data()
{
	//QStringList tW_Vertex_head{ "Land" , "Air" , "Surface" };
	//ui.tableWidget_Vertex->setHorizontalHeaderLabels(tW_Vertex_head);
	ui.tableWidget_Vertex->setRowCount(0);
	for (int i = 0; i < scenario.getAllVertex().size(); i++) {
		ui.tableWidget_Vertex->insertRow(i);
		ui.tableWidget_Vertex->setItem(i, 0, new QTableWidgetItem(QString::number((scenario.getAllVertex()[i]->getLongitude()), 'f', 2)));
		ui.tableWidget_Vertex->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllVertex()[i]->getLatitude()), 'f', 2)));
		ui.tableWidget_Vertex->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllVertex()[i]->getAltitude()), 'f', 2)));
	}
}

void PathPlanGui::show_Platform_data()
{
	//显示platform的数据
	ui.tableWidget_Platform->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatform().size(); i++)
	{
		ui.tableWidget_Platform->insertRow(i);
		ui.tableWidget_Platform->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllPlatform()[i]->getPlatformName())));
		QPointer<QComboBox> cb_Ptype(new QComboBox());
		cb_Ptype->addItems(platformtype_list);
		ui.tableWidget_Platform->setCellWidget(i, 1, cb_Ptype);
		//auto cell = ui.tableWidget_Platform->cellWidget(i, 1);
		//QComboBox *cb_cell = qobject_cast<QComboBox*>(cell);
		//QPointer<QComboBox> cb_Ptype(qobject_cast<QComboBox*>(cell));
		
		if (scenario.getAllPlatform()[i]->getPlatformType() == sce::PlatformType::LAND)
		{
			cb_Ptype->setCurrentIndex(0);
		}
		if (scenario.getAllPlatform()[i]->getPlatformType() == sce::PlatformType::AIR)
		{
			cb_Ptype->setCurrentIndex(1);
		}
		if (scenario.getAllPlatform()[i]->getPlatformType() == sce::PlatformType::SURFACE)
		{
			cb_Ptype->setCurrentIndex(2);
		}
	}	
}

void PathPlanGui::show_Emitter_data()
{
	//显示emitter的数据
	ui.tableWidget_Emitter->setRowCount(0);
	for (int i = 0; i < scenario.getAllEmitter().size(); i++) {
		ui.tableWidget_Emitter->insertRow(i);
		ui.tableWidget_Emitter->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllEmitter()[i]->getName())));
		QPointer<QPushButton> btn(new QPushButton("View"));
		ui.tableWidget_Emitter->setCellWidget(i, 1, btn);
		connect(btn, SIGNAL(clicked()), this, SLOT(show_rada()));
	}
}

void PathPlanGui::show_Weapon_data()
{
	//显示weapon的数据
	ui.tableWidget_Weapon->setRowCount(0);
	for (int i = 0; i < scenario.getAllWeapon().size(); i++)
	{
		ui.tableWidget_Weapon->insertRow(i);
		ui.tableWidget_Weapon->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllWeapon()[i]->getName())));
		ui.tableWidget_Weapon->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllWeapon()[i]->getCEPR())));
		ui.tableWidget_Weapon->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllWeapon()[i]->getWeaponAreaCoverage())));
	}
}

void PathPlanGui::show_Site_data()
{
	//显示site的数据
	ui.tableWidget_Site->setRowCount(0);
	for (int i = 0; i < scenario.getAllSite().size(); i++)
	{
		ui.tableWidget_Site->insertRow(i);
		ui.tableWidget_Site->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllSite()[i]->getName())));
		ui.tableWidget_Site->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllSite()[i]->getAltitude()), 'f', 2)));
		ui.tableWidget_Site->setItem(i, 3, new QTableWidgetItem(QString::number((scenario.getAllSite()[i]->getLongitude()), 'f', 2)));
		ui.tableWidget_Site->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllSite()[i]->getLatitude()), 'f', 2)));
	}
}

void PathPlanGui::show_OwnPlatform_data()
{
	//显示OwnPlatform
	ui.tableWidget_OPlatform->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatform().size(); i++) {
		ui.tableWidget_OPlatform->insertRow(i);
		ui.tableWidget_OPlatform->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllOwnPlatform()[i]->getName())));
		QPointer<QComboBox> cbOptype(new QComboBox());
		ui.tableWidget_OPlatform->setCellWidget(i, 1, cbOptype);
		ui.tableWidget_OPlatform->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxAcceleration())));
		ui.tableWidget_OPlatform->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxDeceleration())));
		ui.tableWidget_OPlatform->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxClimbRate())));
		ui.tableWidget_OPlatform->setItem(i, 5, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxDiveRate())));
		ui.tableWidget_OPlatform->setItem(i, 6, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxSpeed())));
		ui.tableWidget_OPlatform->setItem(i, 7, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[i]->getMaxTurnRadius())));
		QPointer<QPushButton> cb_mission(new QPushButton("View"));
		ui.tableWidget_OPlatform->setCellWidget(i, 8, cb_mission);
		auto cellWidget = ui.tableWidget_OPlatform->cellWidget(i, 1);
		auto cellWidget_2 = ui.tableWidget_OPlatform->cellWidget(i, 8);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QStringList alist{"Air"};
		combox->addItems(alist);
		QPushButton *btn = qobject_cast<QPushButton*>(cellWidget_2);
		btn->setText("View");
		connect(btn, SIGNAL(clicked()), this, SLOT(show_mission()));
	}
}

void PathPlanGui::show_Ecm_data()
{
	//ECM
	ui.tableWidget_Ecm->setRowCount(0);
	for (int i = 0; i < scenario.getAllEcm().size(); i++) {
		ui.tableWidget_Ecm->insertRow(i);
		ui.tableWidget_Ecm->setItem(i, 0, new QTableWidgetItem(QString::fromStdString((scenario.getAllEcm()[i]->getName()))));
		ui.tableWidget_Ecm->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getPt()), 'f', 2)));
		ui.tableWidget_Ecm->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getGain()), 'f', 2)));
		ui.tableWidget_Ecm->setItem(i, 3, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getRfMin()), 'f', 2)));
		ui.tableWidget_Ecm->setItem(i, 4, new QTableWidgetItem(QString::number((scenario.getAllEcm()[i]->getRfMax()), 'f', 2)));
		QPointer<QPushButton> btn(new QPushButton("View"));
		ui.tableWidget_Ecm->setCellWidget(i, 5, btn);
		auto cell = ui.tableWidget_Ecm->cellWidget(i, 5);
		//QPushButton *tech = qobject_cast<QPushButton*>(cell);
		//tech->setText("View");
		connect(btn, SIGNAL(clicked()), this, SLOT(ecm_tech()));
	}
}

void PathPlanGui::show_Esm_data()
{
	//Esm
	ui.tableWidget_Esm->setRowCount(0);
	for (int i = 0; i < scenario.getAllEsm().size(); i++) {
		ui.tableWidget_Esm->insertRow(i);
		ui.tableWidget_Esm->setItem(i, 0, new QTableWidgetItem(QString::fromStdString((scenario.getAllEsm()[i]->getName()))));
		ui.tableWidget_Esm->setItem(i, 1, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getDwellFreqResolution()), 'f', 2)));
		ui.tableWidget_Esm->setItem(i, 2, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getTuningStep()), 'f', 2)));
		ui.tableWidget_Esm->setItem(i, 3, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getRfCovMin()), 'f', 2)));
		ui.tableWidget_Esm->setItem(i, 4, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getRfCovMax()), 'f', 2)));
		ui.tableWidget_Esm->setItem(i, 5, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getNumPulsesAcquisition()), 'f', 2)));
		ui.tableWidget_Esm->setItem(i, 6, new QTableWidgetItem(QString::number((scenario.getAllEsm()[i]->getNumPulsesAlarm()), 'f', 2)));
	}
}

void PathPlanGui::show_EsmStratgy_data()
{
	//显示EsmStrategy的数据
	ui.tableWidget_ESMStra->setRowCount(0);
	for (int i = 0; i < scenario.getAllEsmStrategy().size(); i++)
	{
		ui.tableWidget_ESMStra->insertRow(i);
		ui.tableWidget_ESMStra->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllEsmStrategy()[i]->getName())));
		
		QPointer<QPushButton> ptn(new QPushButton("View"));
		ui.tableWidget_ESMStra->setCellWidget(i, 1, ptn);
		//QPushButton* ptn = qobject_cast<QPushButton*>(ui.tableWidget_ESMStra->cellWidget(i, 1));
		//ptn->setText("View");
		connect(ptn, SIGNAL(clicked()), this, SLOT(show_esmstrategy_section()));
	}
}

void PathPlanGui::show_EcmStratgy_data()
{
	//显示EcmStrategy的数据
	ui.tableWidget_ECMStra->setRowCount(0);
	for (int i = 0; i < scenario.getAllEcmStrategy().size(); i++) {
		ui.tableWidget_ECMStra->insertRow(i);
		ui.tableWidget_ECMStra->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(scenario.getAllEcmStrategy()[i]->getName())));

		QPointer<QPushButton> ptn(new QPushButton("View"));
		//ptn->setText("View");
		ui.tableWidget_ECMStra->setCellWidget(i, 1, ptn);
		//QPushButton* ptn = qobject_cast<QPushButton*>(ui.tableWidget_ECMStra->cellWidget(i, 1));
		//ptn->setText("View");
		connect(ptn, SIGNAL(clicked()), this, SLOT(show_ecmstrategy_section()));
	}
}

void PathPlanGui::show_Route_data()
{
	//Route
	for (int i = 0; i < ui.tabWidget_Route->count(); ) {
		ui.tabWidget_Route->removeTab(i);
	}
	for (int i = 0; i < scenario.getAllRoute().size(); i++) {
		//首先设置一个widget
		QWidget * tab_nn = new QWidget();
		QSpacerItem *horizontalSpacer_0 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);;
		QSpacerItem *horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);;

		QHBoxLayout* hbLayout_n = new QHBoxLayout();
		QVBoxLayout* vbLayout_n = new QVBoxLayout();
		QPointer<QPushButton> add = new QPushButton("Add");
		QPointer<QPushButton> del = new QPushButton("Del");
		QPointer<QPushButton> save = new QPushButton("Save");

		QStringList headers;
		headers << QStringLiteral("Index") << QStringLiteral("Altitude") << QStringLiteral("Index") << QStringLiteral("Latitude") << QStringLiteral("Longitude") << QStringLiteral("Time") << QStringLiteral("Velocity") << QStringLiteral("Acceleration");
		//创建route的表格
		QPointer<QTableWidget> new_table = new QTableWidget();
		new_table->setColumnCount(7);
		new_table->setRowCount(0);
		new_table->setLayoutDirection(Qt::LeftToRight);
		new_table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		new_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		new_table->setDragEnabled(true);
		new_table->setSelectionBehavior(QAbstractItemView::SelectItems);
		new_table->setTextElideMode(Qt::ElideMiddle);
		new_table->horizontalHeader()->setCascadingSectionResizes(false);
		new_table->verticalHeader()->setVisible(true);
		new_table->setHorizontalHeaderLabels(headers);
		for (int j = 0; j < scenario.getAllRoute()[i]->getAllWayPoints().size(); j++)
		{
			new_table->insertRow(j);
			new_table->setItem(j, 0, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getIndex()), 10));
			new_table->setItem(j, 1, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getAltitude(), 'f', 2)));
			new_table->setItem(j, 2, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getLatitude(), 'f', 2)));
			new_table->setItem(j, 3, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getLongitude(), 'f', 2)));
			new_table->setItem(j, 4, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getTime(), 'f', 2)));
			new_table->setItem(j, 5, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getVelocity(), 'f', 2)));
			new_table->setItem(j, 6, new QTableWidgetItem(QString::number(scenario.getAllRoute()[i]->getAllWayPoints()[j].getAcceleration(), 'f', 2)));
		}
		hbLayout_n->addItem(horizontalSpacer_0);
		hbLayout_n->addWidget(add);
		hbLayout_n->addWidget(del);
		hbLayout_n->addWidget(save);
		hbLayout_n->addItem(horizontalSpacer_1);
		
		vbLayout_n->addItem(hbLayout_n);
		vbLayout_n->addWidget(new_table);

		tab_nn->setLayout(vbLayout_n);
		ui.tabWidget_Route->setCurrentWidget(tab_nn);
		ui.tabWidget_Route->addTab(tab_nn, QString::fromStdString(scenario.getAllRoute()[i]->getName()));

		int cur_index = ui.tabWidget_Route->currentIndex();
		//测试tab类
		MyTab mytab;
		mytab.add = add;
		mytab.del = del;
		mytab.save = save;
		mytab.myindex = cur_index;
		mytab.my_table = new_table;
		connect(mytab.add, &QPushButton::clicked, this, [=]()
		{
			mytab.my_table->insertRow(mytab.my_table->rowCount());

		});
		connect(mytab.del, &QPushButton::clicked, this, [=]
		{
			int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
			if (ree == 0)
			{
				mytab.my_table->removeRow(mytab.my_table->currentRow());
			}
		});
		connect(mytab.save, &QPushButton::clicked, this, [=]
		{
			int cur = mytab.my_table->currentRow();
			QString a = mytab.my_table->item(cur, 0)->text();
			QString b = mytab.my_table->item(cur, 1)->text();
			QString c = mytab.my_table->item(cur, 2)->text();
			QString d = mytab.my_table->item(cur, 3)->text();
			QString e = mytab.my_table->item(cur, 4)->text();
			QString f = mytab.my_table->item(cur, 5)->text();
			QString g = mytab.my_table->item(cur, 6)->text();
			if (cur + 1 > scenario.getAllRoute()[cur_index]->getAllWayPoints().size())
			{
				sce::WayPoint new_wp(a.toInt(), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble(), f.toDouble(), g.toDouble());
				scenario.getAllRoute()[cur_index]->addWayPoint(new_wp);
				QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

				QDomElement third = dom.createElement("WayPoints");

				QDomElement fourth_1 = dom.createElement("Index");
				QDomElement fourth_2 = dom.createElement("Altitude");
				QDomElement fourth_3 = dom.createElement("Latitude");
				QDomElement fourth_4 = dom.createElement("Longitude");
				QDomElement fourth_5 = dom.createElement("Time");
				QDomElement fourth_6 = dom.createElement("Velocity");
				QDomElement fourth_7 = dom.createElement("Acceleration");

				QDomText text1 = dom.createTextNode(a);
				QDomText text2 = dom.createTextNode(b);
				QDomText text3 = dom.createTextNode(c);
				QDomText text4 = dom.createTextNode(d);
				QDomText text5 = dom.createTextNode(e);
				QDomText text6 = dom.createTextNode(f);
				QDomText text7 = dom.createTextNode(g);

				fourth_1.appendChild(text1);
				fourth_2.appendChild(text2);
				fourth_3.appendChild(text3);
				fourth_4.appendChild(text4);
				fourth_5.appendChild(text5);
				fourth_6.appendChild(text6);
				fourth_7.appendChild(text7);

				third.appendChild(fourth_1);
				third.appendChild(fourth_2);
				third.appendChild(fourth_3);
				third.appendChild(fourth_4);
				third.appendChild(fourth_5);
				third.appendChild(fourth_6);
				third.appendChild(fourth_7);

				QDomNodeList list = dom.elementsByTagName("Route");
				int flag = 0;
				for (int i = 0; i < list.count(); i++)
				{
					QDomElement ele = list.at(i).toElement();
					if (ele.parentNode().nodeName() == "Scenario")
					{
						if (flag == cur_index)
						{
							ele.appendChild(third);
							break;
						}
						flag++;
					}
				}
			}
			else
			{
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setIndex(a.toInt());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setAltitude(b.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setLatitude(c.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setLongitude(d.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setTime(e.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setVelocity(f.toDouble());
				scenario.getAllRoute()[cur_index]->getAllWayPoints()[cur].setAcceleration(g.toDouble());
				QMessageBox::about(this, tr("Tip"), tr("Save WayPoint successfully"));

				QDomNodeList list = dom.elementsByTagName("Route");
				int flag = 0;
				for (int i = 0; i < list.count(); i++)
				{
					QDomElement ele = list.at(i).toElement();
					if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的Route
					{
						if (flag == cur_index)
						{
							int flag_2 = 0;
							for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
							{
								if (qd.nodeName() == "WayPoints")//找到具体的WayPoints
								{
									if (flag_2 == cur)
									{
										for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
										{
											if (qdd.nodeName() == "Index")
											{
												qdd.firstChild().setNodeValue(a);
											}
											if (qdd.nodeName() == "Altitude")
											{
												qdd.firstChild().setNodeValue(b);
											}
											if (qdd.nodeName() == "Latitude")
											{
												qdd.firstChild().setNodeValue(c);
											}
											if (qdd.nodeName() == "Longitude")
											{
												qdd.firstChild().setNodeValue(d);
											}
											if (qdd.nodeName() == "Time")
											{
												qdd.firstChild().setNodeValue(e);
											}
											if (qdd.nodeName() == "Velocity")
											{
												qdd.firstChild().setNodeValue(f);
											}
											if (qdd.nodeName() == "Acceleration")
											{
												qdd.firstChild().setNodeValue(g);
											}
										}
										break;
									}
									flag_2++;
								}
							}
							break;
						}
						flag++;
					}
				}
			}
		});

		vTab.push_back(&mytab);
	}
}

void PathPlanGui::show_PSRs_data()
{
	//show PlatformSiteRelation
	QStringList platformname_List;
	QStringList sitename_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	for (int j = 0; j < scenario.getAllSite().size(); j++) {
		sitename_List.append(QString::fromStdString(scenario.getAllSite()[j]->getName()));
	}
	ui.tableWidget_PSR->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatformSiteRelation().size(); i++)
	{
		ui.tableWidget_PSR->insertRow(i);
		ui.tableWidget_PSR->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_PSR->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_PSR->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_PSR->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(platformname_List);
		combox_1->addItems(sitename_List);
		for (int j = 0; j < platformname_List.size(); j++)
		{
			if (platformname_List[j].toStdString() == scenario.getAllPlatformSiteRelation()[i].getPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < sitename_List.size(); j++)
		{
			if (sitename_List[j].toStdString() == scenario.getAllPlatformSiteRelation()[i].getSiteName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_PERs_data()
{
	//显示PlatformEmitterRelation的数据
	QStringList platformname_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}
	QStringList Emittername_List;
	for (int j = 0; j < scenario.getAllEmitter().size(); j++) {
		Emittername_List.append(QString::fromStdString(scenario.getAllEmitter()[j]->getName()));
	}
	ui.tableWidget_PER->setRowCount(0);
	for (int i = 0; i < scenario.getAllPlatformEmitterRelation().size(); i++) {
		ui.tableWidget_PER->insertRow(i);
		ui.tableWidget_PER->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_PER->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_PER->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_PER->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(platformname_List);
		combox_1->addItems(Emittername_List);
		for (int j = 0; j < platformname_List.size(); j++)
		{
			if (platformname_List[j].toStdString() == scenario.getAllPlatformEmitterRelation()[i].getPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Emittername_List.size(); j++)
		{
			if (Emittername_List[j].toStdString() == scenario.getAllPlatformEmitterRelation()[i].getEmitterName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_PWRs_data()
{
	//显示PlatformWeaponRelation的数据
	ui.tableWidget_PWR->setRowCount(0);
	QStringList platformname_List;
	for (int j = 0; j < scenario.getAllPlatform().size(); j++) {
		platformname_List.append(QString::fromStdString(scenario.getAllPlatform()[j]->getPlatformName()));
	}

	QStringList Weaponname_List;
	for (int j = 0; j < scenario.getAllWeapon().size(); j++) {
		Weaponname_List.append(QString::fromStdString(scenario.getAllWeapon()[j]->getName()));
	}

	for (int i = 0; i < scenario.getAllPlatformWeaponRelation().size(); i++) {
		ui.tableWidget_PWR->insertRow(i);
		ui.tableWidget_PWR->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_PWR->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_PWR->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_PWR->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		
		combox->addItems(platformname_List);
		combox_1->addItems(Weaponname_List);
		for (int j = 0; j < platformname_List.size(); j++)
		{
			if (platformname_List[j].toStdString() == scenario.getAllPlatformWeaponRelation()[i].getPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Weaponname_List.size(); j++)
		{
			if (Weaponname_List[j].toStdString() == scenario.getAllPlatformWeaponRelation()[i].getWeaponName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_OPEsmRs_data()
{
	//显示OwnPlatformEsmRelation的数据
	QStringList OwnPlatformname_List;
	QStringList Esmname_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		OwnPlatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	ui.tableWidget_OEs->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatformEsmRelation().size(); i++) {
		ui.tableWidget_OEs->insertRow(i);
		ui.tableWidget_OEs->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_OEs->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_OEs->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_OEs->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(OwnPlatformname_List);
		combox_1->addItems(Esmname_List);
		for (int j = 0; j < OwnPlatformname_List.size(); j++)
		{
			if (OwnPlatformname_List[j].toStdString() == scenario.getAllOwnPlatformEsmRelation()[i].getOwnPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Esmname_List.size(); j++)
		{
			if (Esmname_List[j].toStdString() == scenario.getAllOwnPlatformEsmRelation()[i].getEsmName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_EsmESRs_data()
{
	//显示EsmEsmStrategyRelation的数据
	QStringList Esmname_List;
	QStringList EsmStrategyname_List;
	for (int j = 0; j < scenario.getAllEsm().size(); j++) {
		Esmname_List.append(QString::fromStdString(scenario.getAllEsm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEsmStrategy().size(); j++) {
		EsmStrategyname_List.append(QString::fromStdString(scenario.getAllEsmStrategy()[j]->getName()));
	}
	ui.tableWidget_EsmES->setRowCount(0);
	for (int i = 0; i < scenario.getAllEsmEsmStrategyRelation().size(); i++) {
		ui.tableWidget_EsmES->insertRow(i);
		ui.tableWidget_EsmES->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_EsmES->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_EsmES->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_EsmES->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(Esmname_List);
		combox_1->addItems(EsmStrategyname_List);
		for (int j = 0; j < Esmname_List.size(); j++)
		{
			if (Esmname_List[j].toStdString() == scenario.getAllEsmEsmStrategyRelation()[i].getEsmName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < EsmStrategyname_List.size(); j++)
		{
			if (EsmStrategyname_List[j].toStdString() == scenario.getAllEsmEsmStrategyRelation()[i].getEsmStrategyName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_OPEcmRs_data()
{
	//显示OwnPlatformEcmRelation的数据
	ui.tableWidget_OPEcmR->setRowCount(0);
	QStringList OwnPlatformname_List;
	QStringList Ecmname_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		OwnPlatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcm().size(); j++) {
		Ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	for (int i = 0; i < scenario.getAllOwnPlatformEcmRelation().size(); i++) {
		ui.tableWidget_OPEcmR->insertRow(i);
		ui.tableWidget_OPEcmR->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_OPEcmR->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_OPEcmR->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_OPEcmR->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(OwnPlatformname_List);
		combox_1->addItems(Ecmname_List);
		for (int j = 0; j < OwnPlatformname_List.size(); j++)
		{
			if (OwnPlatformname_List[j].toStdString() == scenario.getAllOwnPlatformEcmRelation()[i].getOwnPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Ecmname_List.size(); j++)
		{
			if (Ecmname_List[j].toStdString() == scenario.getAllOwnPlatformEcmRelation()[i].getEcmName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_EcmESRs_data()
{
	//show EcmEcmStrategyRelation
	QStringList ecmname_List;
	QStringList EcmStrategyname_List;
	for (int j = 0; j < scenario.getAllEcm().size(); j++)
	{
		ecmname_List.append(QString::fromStdString(scenario.getAllEcm()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllEcmStrategy().size(); j++)
	{
		EcmStrategyname_List.append(QString::fromStdString(scenario.getAllEcmStrategy()[j]->getName()));
	}
	ui.tableWidget_EcmES->setRowCount(0);
	for (int i = 0; i < scenario.getAllEcmEcmStrategyRelation().size(); i++)
	{
		ui.tableWidget_EcmES->insertRow(i);
		ui.tableWidget_EcmES->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_EcmES->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_EcmES->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_EcmES->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(ecmname_List);
		combox_1->addItems(EcmStrategyname_List);
		for (int j = 0; j < ecmname_List.size(); j++)
		{
			if (ecmname_List[j].toStdString() == scenario.getAllEcmEcmStrategyRelation()[i].getEcmName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < EcmStrategyname_List.size(); j++)
		{
			if (EcmStrategyname_List[j].toStdString() == scenario.getAllEcmEcmStrategyRelation()[i].getEcmStrategyName())
			{
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::show_OPRRs_data()
{
	//显示OwnPlatformRouteRelation的数据
	QStringList OwnPlatformname_List;
	QStringList Routename_List;
	for (int j = 0; j < scenario.getAllOwnPlatform().size(); j++) {
		OwnPlatformname_List.append(QString::fromStdString(scenario.getAllOwnPlatform()[j]->getName()));
	}
	for (int j = 0; j < scenario.getAllRoute().size(); j++) {
		Routename_List.append(QString::fromStdString(scenario.getAllRoute()[j]->getName()));
	}
	ui.tableWidget_ORR->setRowCount(0);
	for (int i = 0; i < scenario.getAllOwnPlatformRouteRelation().size(); i++) {
		ui.tableWidget_ORR->insertRow(i);
		ui.tableWidget_ORR->setCellWidget(i, 0, new QComboBox());
		ui.tableWidget_ORR->setCellWidget(i, 1, new QComboBox());
		auto cellWidget = ui.tableWidget_ORR->cellWidget(i, 0);
		auto cellWidget_1 = ui.tableWidget_ORR->cellWidget(i, 1);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		QComboBox *combox_1 = qobject_cast<QComboBox*>(cellWidget_1);
		combox->addItems(OwnPlatformname_List);
		combox_1->addItems(Routename_List);
		for (int j = 0; j < OwnPlatformname_List.size(); j++)
		{
			if (OwnPlatformname_List[j].toStdString() == scenario.getAllOwnPlatformRouteRelation()[i].getOwnPlatformName()) {
				combox->setCurrentIndex(j);
				break;
			}
		}
		for (int j = 0; j < Routename_List.size(); j++)
		{
			if (Routename_List[j].toStdString() == scenario.getAllOwnPlatformRouteRelation()[i].getRouteName()) {
				combox_1->setCurrentIndex(j);
				break;
			}
		}
	}
}

void PathPlanGui::onStatusInfo(QtMsgType type, QString msg)
{
	QString text;
	QTextCharFormat chfmt;
	//设置状态信息标签
	switch (type) {
	case QtDebugMsg: //调试信息提示
		text = "INFO";
		chfmt.setForeground(QBrush(Qt::black));
		break;
	case QtWarningMsg: //一般的warning提示
		text = "WARN";
		chfmt.setForeground(QBrush(Qt::yellow));
		break;
	case QtCriticalMsg: //严重错误提示
		text = "ERROR";
		chfmt.setForeground(QBrush(Qt::red));
		break;
	case QtFatalMsg: //致命错误提示
		text = "ERROR";
		chfmt.setForeground(QBrush(Qt::red));
		abort();
		break;
	default:
		break;
	}
	//在标签之后附加状态信息
	text.append(" " + msg);
	//将插入位置设置到QTextEdit最后一行
	QTextCursor cur = ui.statusTextEditer->textCursor();
	cur.movePosition(QTextCursor::End);
	QTextTableFormat tableformate;
	tableformate.setBorder(0);
	//插入table，用于显示状态信息
	QTextTable *table = cur.insertTable(2, 1, tableformate);
	table->cellAt(0, 0).firstCursorPosition().insertText("[" + QTime::currentTime().toString("hh:mm:ss") + "]");
	table->cellAt(1, 0).firstCursorPosition().insertText(text);
	cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor, 1);
	cur.select(QTextCursor::BlockUnderCursor);
	cur.mergeCharFormat(chfmt);
	cur.clearSelection();
	QScrollBar *bar = ui.statusTextEditer->verticalScrollBar();
	bar->setValue(bar->maximum());
	//状态信息条数+1
	m_status_count++;
	//如果信息数>50，则删除最早的一条记录
	if (m_status_count >50)
	{
		//将选择光标移至第一个字符
		cur.setPosition(1, QTextCursor::MoveAnchor);
		//选择最早含有最早状态信息的table
		cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor, 1);
		cur.select(QTextCursor::BlockUnderCursor);
		//删除该信息
		cur.removeSelectedText();
		//光标移至QTextEdit末行
		cur.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
		m_status_count--;
	}
}
