#include "Section_page.h"
#include "MainWindow.h"
Section_page::Section_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Section"));
	resize(950, 200);
	ui.tableWidget->setColumnCount(9);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	headers_esmstrategy_section << QStringLiteral("StartTime") << QStringLiteral("EndTime") << QStringLiteral("Start_Altitude") << "Start_Latitude" << "Start_Longtitude" << QStringLiteral("End_Altitude") << "End_Latitude" << "End_Longtitude" << QStringLiteral("DwellSquence");
	headers_ecmstrategy_section << QStringLiteral("StartTime") << QStringLiteral("EndTime") << QStringLiteral("Start_Altitude") << "Start_Latitude" << "Start_Longtitude" << QStringLiteral("End_Altitude") << "End_Latitude" << "End_Longtitude" << QStringLiteral("Tech");
}

Section_page::~Section_page()
{
}

void Section_page::show_esmstrategy_section() 
{
	QPushButton *add = new QPushButton(this);
	QPushButton *del = new QPushButton(this);
	add->setText("add");
	del->setText("del");
	QPushButton *save = new QPushButton();
	save->setText("save");
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	ui.tableWidget->setHorizontalHeaderLabels(headers_esmstrategy_section);
	this->setCentralWidget(widget);
	if (choice_esmstrategy < scenario.getAllEsmStrategy().size()) {
		ui.tableWidget->setRowCount(0);
		for (int i = 0; i < scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections().size(); i++)
		{
			ui.tableWidget->insertRow(i);
			ui.tableWidget->setCellWidget(i, 8, new QPushButton());
			QPushButton* btn_3 = qobject_cast<QPushButton*>(ui.tableWidget->cellWidget(i, 8));
			btn_3->setText("DwellSquence");
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getStartTime(), 'f', 2)));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getEndTime(), 'f', 2)));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getAltitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getLatitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getLongitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getEndLocation().getAltitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getLatitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getLongitude(), 'f', 2)));
			connect(btn_3, SIGNAL(clicked()), this, SLOT(show_dwellsquence()));
		}
		connect(save, SIGNAL(clicked()), this, SLOT(save_esmstrategy_section()));
		connect(add, SIGNAL(clicked()), this, SLOT(append_esmstrategy_section()));
		connect(del, SIGNAL(clicked()), this, SLOT(del_esmstrategy_section()));
		this->show();
	}
	else {
		ui.tableWidget->setRowCount(0);
		this->show();
	}
}
void Section_page::show_ecmstrategy_section() 
{	
	if (choice_ecmstrategy < scenario.getAllEcmStrategy().size()) {
		QPushButton *add = new QPushButton(this);
		QPushButton *del = new QPushButton(this);
		add->setText("add");
		del->setText("del");
		QPushButton *save = new QPushButton();
		save->setText("save");
		QWidget *widget = new QWidget(this);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(add);
		layout->addWidget(del);
		layout->addWidget(save);
		layout->addWidget(ui.tableWidget);
		widget->setLayout(layout);
		this->setCentralWidget(widget);
		ui.tableWidget->setHorizontalHeaderLabels(headers_ecmstrategy_section);
		ui.tableWidget->setRowCount(0);
		for (int i = 0; i < scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections().size(); i++)
		{
			ui.tableWidget->insertRow(i);
			ui.tableWidget->setCellWidget(i, 8, new QPushButton());
			QPushButton* btn_3 = qobject_cast<QPushButton*>(ui.tableWidget->cellWidget(i, 8));
			btn_3->setText("Tech");
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getStartTime(), 'f', 2)));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getEndTime(), 'f', 2)));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getAltitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getLatitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getStartLocation().getLongitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getEndLocation().getAltitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getEndLocation().getLatitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[i]->getEndLocation().getLongitude(), 'f', 2)));
			connect(btn_3, SIGNAL(clicked()), this, SLOT(show_ecmstragegy_tech()));
		}
		connect(save, SIGNAL(clicked()), this, SLOT(save_ecmstrategy_section()));
		connect(add, SIGNAL(clicked()), this, SLOT(append_ecmstrategy_section()));
		connect(del, SIGNAL(clicked()), this, SLOT(del_ecmstrategy_section()));
		this->show();
	}
	else {
		QPushButton *add = new QPushButton(this);
		QPushButton *del = new QPushButton(this);
		add->setText("add");
		del->setText("del");
		QPushButton *save = new QPushButton();
		save->setText("save");
		QWidget *widget = new QWidget(this);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->addWidget(add);
		layout->addWidget(del);
		layout->addWidget(save);
		layout->addWidget(ui.tableWidget);
		widget->setLayout(layout);
		this->setCentralWidget(widget);
		ui.tableWidget->setRowCount(0);
		ui.tableWidget->insertRow(0);
		ui.tableWidget->insertRow(1);
		ui.tableWidget->setHorizontalHeaderLabels(headers_ecmstrategy_section);
		QPushButton *btn_3 = new QPushButton();
		QPushButton *btn_6 = new QPushButton();
		btn_3->setText("View");
		btn_6->setText("View");
		ui.tableWidget->setCellWidget(0, 8, btn_3);
		ui.tableWidget->setCellWidget(1, 8, btn_6);
		connect(btn_3, SIGNAL(clicked()), this, SLOT(added_ecmstrategy_tech()));
		connect(btn_6, SIGNAL(clicked()), this, SLOT(added_ecmstrategy_tech()));
		connect(save, SIGNAL(clicked()), this, SLOT(save_ecmstrategy_section()));
		connect(add, SIGNAL(clicked()), this, SLOT(append_ecmstrategy_section()));
		connect(del, SIGNAL(clicked()), this, SLOT(del_ecmstrategy_section()));
		this->show();
	}
}
//void Section_page::show_startlocation() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_esm_startlocation();
//}
//void Section_page::show_endlocation() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_esm_endlocation();
//}
void Section_page::show_dwellsquence() {
	choice_section = ui.tableWidget->currentRow();
	emit sign_dwellsquence();
}
//void Section_page::show_startlocation2() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_esm_startlocation();
//}
//void Section_page::show_endlocation2() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_esm_endlocation();
//}
void Section_page::show_dwellsquence2() {
	choice_section = ui.tableWidget->currentRow();
	emit sign_dwellsquence();
}
//void Section_page::show_ecm_startlocation() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_ecm_startlocation();
//}
//void Section_page::show_ecm_endlocation() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_ecm_endlocation();
//}
//void Section_page::show_ecm_startlocation2() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_ecm_startlocation();
//}
//void Section_page::show_ecm_endlocation2() {
//	choice_section = ui.tableWidget->currentRow();
//	emit sign_ecm_endlocation();
//}
void Section_page::show_ecmstragegy_tech() {
	choice_section = ui.tableWidget->currentRow();
	emit sign_tech();
}
void Section_page::show_ecmstragegy_tech2() {
	choice_section = ui.tableWidget->currentRow();
	emit sign_tech();
}
void Section_page::append_ecmstrategy_section()
{
	int row_count = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row_count);
	QPushButton *but = new QPushButton();
	but->setText("Tech");
	ui.tableWidget->setCellWidget(row_count, 8, but);
	connect(but, SIGNAL(clicked()), this, SLOT(added_ecmstrategy_tech()));
}
void Section_page::del_ecmstrategy_section()
{
	size_t num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion ? ", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections().size()) {
			scenario.getAllEcmStrategy()[choice_ecmstrategy]->deleteSection(num);
			QMessageBox::about(this, "Tip", "Delete Section successfully");
			QDomNodeList list_node = dom.elementsByTagName("EcmStrategy");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == choice_ecmstrategy)
					{
						int flag_radar = 0;
						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
						{
							if (qd.nodeName() == "Section")
							{
								if (flag_radar == num)
								{
									ele.removeChild(qd);
									QMessageBox::about(this, "Tip", "Delete Section in file successfully");
									break;
								}
								flag_radar++;
							}
						}
						break;
					}
					flag++;
				}
			}
		}
	}

}
void Section_page::append_esmstrategy_section()
{
	int row_count = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row_count);
	QPushButton *but = new QPushButton();
	but->setText("DwellSquence");
	ui.tableWidget->setCellWidget(row_count, 8, but);
	connect(but, SIGNAL(clicked()), this, SLOT(added_dwellsquence()));
}
void Section_page::del_esmstrategy_section()
{
	size_t num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion ? ", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections().size()) {
			scenario.getAllEsmStrategy()[choice_esmstrategy]->deleteSection(num);
			QMessageBox::about(this, "Tip", "Delete Section successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("EsmStrategy");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == choice_esmstrategy)
					{
						int flag_radar = 0;
						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
						{
							if (qd.nodeName() == "Section")
							{
								if (flag_radar == num)
								{
									ele.removeChild(qd);
									QMessageBox::about(this, "Tip", "Delete Section in file successfully");
									break;
								}
								flag_radar++;
							}
						}
						break;
					}
					flag++;
				}
			}
		}
	}

}
void Section_page::added_esmstrategy_section() 
{
	QPushButton *add = new QPushButton(this);
	QPushButton *del = new QPushButton(this);
	add->setText("add");
	del->setText("del");
	QPushButton *save = new QPushButton();
	save->setText("save");
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	this->setCentralWidget(widget);
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->insertRow(0);
	ui.tableWidget->setHorizontalHeaderLabels(headers_esmstrategy_section);
	QPushButton *btn_3 = new QPushButton();
	btn_3->setText("View");
	ui.tableWidget->setCellWidget(0, 8, btn_3);
	connect(btn_3, SIGNAL(clicked()), this, SLOT(added_dwellsquence()));
	connect(save, SIGNAL(clicked()), this, SLOT(save_esmstrategy_section()));
	connect(add, SIGNAL(clicked()), this, SLOT(append_esmstrategy_section()));
	connect(del, SIGNAL(clicked()), this, SLOT(del_esmstrategy_section()));
	this->show();
}
void Section_page::save_esmstrategy_section() 
{
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QString c = ui.tableWidget->item(num, 2)->text();
	QString d = ui.tableWidget->item(num, 3)->text();
	QString e = ui.tableWidget->item(num, 4)->text();
	QString f = ui.tableWidget->item(num, 5)->text();
	QString g = ui.tableWidget->item(num, 6)->text();
	QString h = ui.tableWidget->item(num, 7)->text();
	if (num + 1 > scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections().size())
	{
		sce::Location new_start(c.toDouble(), d.toDouble(), e.toDouble());
		sce::Location new_end(f.toDouble(), g.toDouble(), h.toDouble());
		sce::EsmStrategySection new_section(a.toDouble(), b.toDouble(), new_start, new_end);
		scenario.getAllEsmStrategy()[choice_esmstrategy]->addSection(std::make_shared<sce::EsmStrategySection>(new_section));
		QMessageBox::about(this, tr("Tip"), tr("Save Esmstrategy Section successfully"));
		QDomNodeList list = dom.elementsByTagName("EsmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的EsmStrategy
			{
				if (flag == choice_esmstrategy)
				{
					QDomElement second = dom.createElement("Section");
					QDomElement st_node = dom.createElement("StartTime");
					QDomElement et_node = dom.createElement("EndTime");
					QDomElement sl_node = dom.createElement("StartLocation");
					QDomElement el_node = dom.createElement("EndLocation");
					QDomElement start_altitu = dom.createElement("Altitude");
					QDomElement start_latitu = dom.createElement("Latitude");
					QDomElement start_longit = dom.createElement("Longitude");
					QDomElement end_altitu = dom.createElement("Altitude");
					QDomElement end_latitu = dom.createElement("Latitude");
					QDomElement end_longit = dom.createElement("Longitude");
					QDomText text1 = dom.createTextNode(a);
					QDomText text2 = dom.createTextNode(b);
					QDomText text3 = dom.createTextNode(c);
					QDomText text4 = dom.createTextNode(d);
					QDomText text5 = dom.createTextNode(e);
					QDomText text6 = dom.createTextNode(f);
					QDomText text7 = dom.createTextNode(g);
					QDomText text8 = dom.createTextNode(h);
					st_node.appendChild(text1);
					et_node.appendChild(text2);
					start_altitu.appendChild(text3);
					start_latitu.appendChild(text4);
					start_longit.appendChild(text5);
					end_altitu.appendChild(text6);
					end_latitu.appendChild(text7);
					end_longit.appendChild(text8);
					sl_node.appendChild(start_altitu);
					sl_node.appendChild(start_latitu);
					sl_node.appendChild(start_longit);
					el_node.appendChild(end_altitu);
					el_node.appendChild(end_latitu);
					el_node.appendChild(end_longit);
					second.appendChild(st_node);
					second.appendChild(et_node);
					second.appendChild(sl_node);
					second.appendChild(el_node);
					ele.appendChild(second);
					break;
				}
				flag++;
			}
		}
	}
	else 
	{
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->setStartTime(a.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->setEndTime(b.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->getStartLocation().setAltitude(c.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->getStartLocation().setLatitude(d.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->getStartLocation().setLongitude(e.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->getEndLocation().setAltitude(f.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->getEndLocation().setLatitude(g.toDouble());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[num]->getEndLocation().setLongitude(h.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save Esmstrategy Section successfully"));
		QDomNodeList list = dom.elementsByTagName("EsmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的EsmStrategy
			{
				if (flag == choice_esmstrategy)
				{
					int flag_section = 0;
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "Section")
						{
							if (flag_section == num)
							{
								for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
								{
									if (qdd.nodeName() == "StartTime")
									{
										qdd.firstChild().setNodeValue(a);
									}
									if (qdd.nodeName() == "EndTime")
									{
										qdd.firstChild().setNodeValue(b);
									}
									if (qdd.nodeName() == "StartLocation")
									{
										qdd.firstChild().firstChild().setNodeValue(c);
										qdd.firstChild().nextSibling().firstChild().setNodeValue(d);
										qdd.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(e);
									}
									if (qdd.nodeName() == "EndLocation")
									{
										qdd.firstChild().firstChild().setNodeValue(f);
										qdd.firstChild().nextSibling().firstChild().setNodeValue(g);
										qdd.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(h);
									}
								}
								break;
							}
							flag_section++;
						}
					}
					break;
				}
				flag++;
			}
		}
	}
}
void Section_page::added_dwellsquence() {
	choice_section = ui.tableWidget->currentRow();
	emit sign_added_dwellsquence();
}
void Section_page::added_ecmstrategy_section() 
{
	QPushButton *add = new QPushButton(this);
	QPushButton *del = new QPushButton(this);
	add->setText("add");
	del->setText("del");
	QPushButton *save = new QPushButton();
	save->setText("save");
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	this->setCentralWidget(widget);
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->setHorizontalHeaderLabels(headers_ecmstrategy_section);
	connect(save, SIGNAL(clicked()), this, SLOT(save_ecmstrategy_section()));
	connect(add, SIGNAL(clicked()), this, SLOT(append_ecmstrategy_section()));
	connect(del, SIGNAL(clicked()), this, SLOT(del_ecmstrategy_section()));
	this->show();
}
void Section_page::added_ecmstrategy_tech() {
	choice_section = ui.tableWidget->currentRow();
	emit sign_added_ecmstrategy_tech();
}
void Section_page::save_ecmstrategy_section() 
{
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QString c = ui.tableWidget->item(num, 2)->text();
	QString d = ui.tableWidget->item(num, 3)->text();
	QString e = ui.tableWidget->item(num, 4)->text();
	QString f = ui.tableWidget->item(num, 5)->text();
	QString g = ui.tableWidget->item(num, 6)->text();
	QString h = ui.tableWidget->item(num, 7)->text();
	if (num + 1 > scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections().size())
	{
		sce::Location new_start(c.toDouble(), d.toDouble(), e.toDouble());
		sce::Location new_end(f.toDouble(), g.toDouble(), h.toDouble());
		sce::EcmStrategySection new_section(a.toDouble(), b.toDouble(), new_start, new_end);
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->addSection(std::make_shared<sce::EcmStrategySection>(new_section));
		QMessageBox::about(this, tr("Tip"), tr("Save Ecmstrategy Section successfully"));
		QDomNodeList list = dom.elementsByTagName("EcmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的EcmStrategy
			{
				if (flag == choice_ecmstrategy)
				{
					QDomElement second = dom.createElement("Section");

					QDomElement st_node = dom.createElement("StartTime");
					QDomElement et_node = dom.createElement("EndTime");
					QDomElement sl_node = dom.createElement("StartLocation");
					QDomElement el_node = dom.createElement("EndLocation");
					QDomElement tech_node = dom.createElement("tech");

					QDomElement start_altitu = dom.createElement("Altitude");
					QDomElement start_latitu = dom.createElement("Latitude");
					QDomElement start_longit = dom.createElement("Longitude");
					QDomElement end_altitu = dom.createElement("Altitude");
					QDomElement end_latitu = dom.createElement("Latitude");
					QDomElement end_longit = dom.createElement("Longitude");
					QDomElement tech_name = dom.createElement("Name");

					QDomText text1 = dom.createTextNode(a);
					QDomText text2 = dom.createTextNode(b);
					QDomText text3 = dom.createTextNode(c);
					QDomText text4 = dom.createTextNode(d);
					QDomText text5 = dom.createTextNode(e);
					QDomText text6 = dom.createTextNode(f);
					QDomText text7 = dom.createTextNode(g);
					QDomText text8 = dom.createTextNode(h);
					QDomText text9 = dom.createTextNode("");

					st_node.appendChild(text1);
					et_node.appendChild(text2);
					tech_name.appendChild(text9);
					start_altitu.appendChild(text3);
					start_latitu.appendChild(text4);
					start_longit.appendChild(text5);
					end_altitu.appendChild(text6);
					end_latitu.appendChild(text7);
					end_longit.appendChild(text8);
					tech_node.appendChild(tech_name);
					sl_node.appendChild(start_altitu);
					sl_node.appendChild(start_latitu);
					sl_node.appendChild(start_longit);
					el_node.appendChild(end_altitu);
					el_node.appendChild(end_latitu);
					el_node.appendChild(end_longit);
					second.appendChild(st_node);
					second.appendChild(et_node);
					second.appendChild(sl_node);
					second.appendChild(el_node);
					second.appendChild(tech_node);
					ele.appendChild(second);
					break;
				}
				flag++;
			}
		}
	}
	else
	{
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->setStartTime(a.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->setEndTime(b.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->getStartLocation().setAltitude(c.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->getStartLocation().setLatitude(d.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->getStartLocation().setLongitude(e.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->getEndLocation().setAltitude(f.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->getEndLocation().setLatitude(g.toDouble());
		scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[num]->getEndLocation().setLongitude(h.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save Ecmstrategy Section successfully"));
		QDomNodeList list = dom.elementsByTagName("EcmStrategy");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的EcmStrategy
			{
				if (flag == choice_ecmstrategy)
				{
					int flag_section = 0;
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "Section")
						{
							if (flag_section == num)
							{
								for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
								{
									if (qdd.nodeName() == "StartTime")
									{
										qdd.firstChild().setNodeValue(a);
									}
									if (qdd.nodeName() == "EndTime")
									{
										qdd.firstChild().setNodeValue(b);
									}
									if (qdd.nodeName() == "StartLocation")
									{
										qdd.firstChild().firstChild().setNodeValue(c);
										qdd.firstChild().nextSibling().firstChild().setNodeValue(d);
										qdd.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(e);
									}
									if (qdd.nodeName() == "EndLocation")
									{
										qdd.firstChild().firstChild().setNodeValue(f);
										qdd.firstChild().nextSibling().firstChild().setNodeValue(g);
										qdd.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(h);
									}
								}
								break;
							}
							flag_section++;
						}
					}
					break;
				}
				flag++;
			}
		}
	}
}
