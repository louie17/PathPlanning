#include "Mission_page.h"

Mission_page::Mission_page(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(1000,200);
	QStringList headers;
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();	
	//QPushButton *add = new QPushButton(this);
	//QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	
	//add->setText("add");
	//del->setText("del");
	save->setText("save");
	headers << "Type" << "Start_Altitude"<<"Start_Latitude" << "Start_Longtitude" << "Start_Tmin" << "Start_Tmax" << "End_Altitude" << "End_Latitude" << "End_Longtitude" << "End_Tmin" << "End_Tmax" <<QStringLiteral("TargetPoint");
	ui.tableWidget->setColumnCount(12);
	ui.tableWidget->setHorizontalHeaderLabels(headers);
	//ui.tableWidget->setGeometry(QRect(200, 70, 700, 281));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	//layout->addWidget(add);
	//layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	setCentralWidget(widget);
	/*connect(add, SIGNAL(clicked()), this, SLOT(add()));
	connect(del, SIGNAL(clicked()), this, SLOT(del()));*/
	connect(save, SIGNAL(clicked()), this, SLOT(save()));
	mission_list << "Strike" << "Support";
}

Mission_page::~Mission_page()
{
}
void Mission_page::save() 
{
	int num = ui.tableWidget->currentRow();
	QComboBox* cell = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(num, 0));
	int index = cell->currentIndex();
	QString a = ui.tableWidget->item(num, 1)->text();
	QString b = ui.tableWidget->item(num, 2)->text();
	QString c = ui.tableWidget->item(num, 3)->text();
	QString d = ui.tableWidget->item(num, 4)->text();
	QString e = ui.tableWidget->item(num, 5)->text();
	QString f = ui.tableWidget->item(num, 6)->text();
	QString g = ui.tableWidget->item(num, 7)->text();
	QString h = ui.tableWidget->item(num, 8)->text();
	QString i = ui.tableWidget->item(num, 9)->text();
	QString j = ui.tableWidget->item(num, 10)->text();
	sce::Point point1(a.toDouble(), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble());
	sce::Point point2(f.toDouble(), g.toDouble(), h.toDouble(), i.toDouble(), j.toDouble());
	scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().setMissionType(sce::MissionType(index));
	scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().setStartPoint(point1);
	scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().setEndPoint(point2);
	QMessageBox::about(this, tr("Tip"), tr("Save Mission successfully"));
	QDomNodeList list = dom.elementsByTagName("Mission");
	int flag = 0;
	for (int z = 0; z < list.count(); z++)
	{
		QDomElement ele = list.at(z).toElement();
		if (flag == choice_ownplatform)
		{
			//ele.firstChild().firstChild().setNodeValue(mission_list[index]);
			//ele.firstChild().nextSibling().firstChild().firstChild().setNodeValue(a);
			//ele.firstChild().nextSibling().firstChild().nextSibling().firstChild().setNodeValue(b);
			//ele.firstChild().nextSibling().firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
			//ele.firstChild().nextSibling().firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
			//ele.firstChild().nextSibling().firstChild().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(e);
			//ele.firstChild().nextSibling().nextSibling().firstChild().firstChild().setNodeValue(f);
			//ele.firstChild().nextSibling().nextSibling().firstChild().nextSibling().firstChild().setNodeValue(g);
			//ele.firstChild().nextSibling().nextSibling().firstChild().nextSibling().nextSibling().firstChild().setNodeValue(h);
			//ele.firstChild().nextSibling().nextSibling().firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(i);
			//ele.firstChild().nextSibling().nextSibling().firstChild().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(j);

			for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
			{
				if (qd.nodeName() == "Type")
				{
					qd.firstChild().setNodeValue(mission_list[index]);
				}
				if (qd.nodeName() == "StartPoint")
				{
					for (QDomNode cqd = qd.firstChild(); !cqd.isNull(); cqd = cqd.nextSibling())
					{
						if (cqd.nodeName()=="Altitude")
						{
							cqd.firstChild().setNodeValue(a);
						}
						if (cqd.nodeName() == "Latitude")
						{
							cqd.firstChild().setNodeValue(b);
						}
						if (cqd.nodeName() == "Longitude")
						{
							cqd.firstChild().setNodeValue(c);
						}
						if (cqd.nodeName() == "Tmin")
						{
							cqd.firstChild().setNodeValue(d);
						}
						if (cqd.nodeName() == "Tmax")
						{
							cqd.firstChild().setNodeValue(e);
						}
						
					}
					
				}
				if (qd.nodeName() == "EndPoint")
				{
					for (QDomNode cqd = qd.firstChild(); !cqd.isNull(); cqd = cqd.nextSibling())
					{
						if (cqd.nodeName() == "Altitude")
						{
							cqd.firstChild().setNodeValue(f);
						}
						if (cqd.nodeName() == "Latitude")
						{
							cqd.firstChild().setNodeValue(g);
						}
						if (cqd.nodeName() == "Longitude")
						{
							cqd.firstChild().setNodeValue(h);
						}
						if (cqd.nodeName() == "Tmin")
						{
							cqd.firstChild().setNodeValue(i);
						}
						if (cqd.nodeName() == "Tmax")
						{
							cqd.firstChild().setNodeValue(j);
						}

					}

				}
			}
			break;
		}
		flag++;
	}
}
void Mission_page::show_mission() 
{
	if (choice_ownplatform < scenario.getAllOwnPlatform().size()) {
		//ui.tableWidget->setRowCount(0);
		ui.tableWidget->setRowCount(1);
		QComboBox *mission_type = new QComboBox();
		mission_type->addItems(mission_list);
		QPushButton *btn = new QPushButton();
		btn->setText("View");
		if (scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getMissionType() == sce::MissionType::STRIKE) {
			mission_type->setCurrentIndex(0);
		}
		if (scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getMissionType() == sce::MissionType::SUPPORT) {
			mission_type->setCurrentIndex(1);
		}
		ui.tableWidget->setCellWidget(0, 0, mission_type);
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getStartPoint().getAltitude(), 'f', 2)));
		ui.tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getStartPoint().getLatitude(), 'f', 2)));
		ui.tableWidget->setItem(0, 3, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getStartPoint().getLongitude(), 'f', 2)));
		ui.tableWidget->setItem(0, 4, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getStartPoint().getTmin(), 'f', 2)));
		ui.tableWidget->setItem(0, 5, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getStartPoint().getTmax(), 'f', 2)));
		ui.tableWidget->setItem(0, 6, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getEndPoint().getAltitude(), 'f', 2)));
		ui.tableWidget->setItem(0, 7, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getEndPoint().getLatitude(), 'f', 2)));
		ui.tableWidget->setItem(0, 8, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getEndPoint().getLongitude(), 'f', 2)));
		ui.tableWidget->setItem(0, 9, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getEndPoint().getTmin(), 'f', 2)));
		ui.tableWidget->setItem(0, 10, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getEndPoint().getTmax(), 'f', 2)));
		ui.tableWidget->setCellWidget(0, 11, btn);
		connect(btn, SIGNAL(clicked()), this, SLOT(show_point()));
		this->show();
	}
	else {
		ui.tableWidget->setRowCount(0);
		ui.tableWidget->insertRow(0);
		ui.tableWidget->setCellWidget(0, 0, new QComboBox());
		QComboBox *cell = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(0, 0));
		cell->addItems(mission_list);
		QPushButton *bt = new QPushButton();
		ui.tableWidget->setCellWidget(0, 11, bt);
		connect(bt, SIGNAL(clicked()), this, SLOT(show_point()));
		this->show();
	}
}
void Mission_page::add() {
	int num = ui.tableWidget->currentRow();
	ui.tableWidget->insertRow(num);
	ui.tableWidget->setCellWidget(num, 0, new QComboBox());
	ui.tableWidget->setCellWidget(num, 11, new QPushButton());
	auto cell_1 = ui.tableWidget->cellWidget(num, 0);
	auto cell_4 = ui.tableWidget->cellWidget(num, 11);
	QComboBox *cb = qobject_cast<QComboBox*>(cell_1);
	QPushButton *btn_3= qobject_cast<QPushButton*>(cell_4);
	btn_3->setText("View");
	cb->addItems(mission_list); 
	connect(btn_3, SIGNAL(clicked()), this, SLOT(show_point()));
}
void Mission_page::del()		
{
	int num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion ?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
	}

}
void Mission_page::show_point() 
{
	emit sign_point();
}
void Mission_page::added_mission() 
{
	ui.tableWidget->setRowCount(0);
	ui.tableWidget->insertRow(0);
	ui.tableWidget->setCellWidget(0, 0,new QComboBox());
	QComboBox *cell = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(0, 0));
	cell->addItems(mission_list);
	QPushButton *btn = new QPushButton();
	ui.tableWidget->setCellWidget(0, 11, btn);
	connect(btn, SIGNAL(clicked()), this, SLOT(show_point()));
	this->show();
}