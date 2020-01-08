#include "point_page.h"

point_page::point_page(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Point"));
	resize(600, 400);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 300));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
}

point_page::~point_page()
{
}

void point_page::show_point() {
	if (scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints().size() > 0) {
		QVBoxLayout *layout = new QVBoxLayout();
		QWidget *widget = new QWidget(this);
		ui.tableWidget->setColumnCount(5);
		QStringList headers_pw;
		headers_pw << QStringLiteral("Altitude") << QStringLiteral("Latitude") << QStringLiteral("Longitude") << "Tmin" << "Tmax";
		ui.tableWidget->setHorizontalHeaderLabels(headers_pw);
		QPushButton *add = new QPushButton();
		QPushButton *del = new QPushButton();
		QPushButton *save = new QPushButton();
		add->setText("add");
		del->setText("del");
		save->setText("save");
		layout->addWidget(add);
		layout->addWidget(del);
		layout->addWidget(save);
		layout->addWidget(ui.tableWidget);
		widget->setLayout(layout);
		this->setCentralWidget(widget);
		ui.tableWidget->setRowCount(0);
		for (int i = 0; i < scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints().size(); i++) 
		{
			ui.tableWidget->insertRow(i);
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[i].getAltitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[i].getLatitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[i].getLongitude(), 'f', 2)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[i].getTmin(), 'f', 2)));
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[i].getTmax(), 'f', 2)));
		}
		connect(add, SIGNAL(clicked()), this, SLOT(add()));
		connect(del, SIGNAL(clicked()), this, SLOT(del()));
		connect(save, SIGNAL(clicked()), this, SLOT(save()));
		this->show();
	}
	else 
	{
		ui.tableWidget->clearContents();
		this->show();
	}
}
void point_page::add() 
{
	int num = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(num);

}
void point_page::del() 
{
	unsigned int num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints().size()){
			scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().deleteTargetPoint(num);
			QMessageBox::about(this, "Tip", "Delete Pw_Values successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("OwnPlatform");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == choice_ownplatform)
					{
						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
						{
							if (qd.nodeName() == "Mission")
							{							
								int flag_point = 0;
								for (QDomNode qd_2 = qd.firstChild(); !qd_2.isNull(); qd_2 = qd_2.nextSibling())
								{
									if (qd_2.nodeName() == "TargetPoint")
									{
										if (flag_point == num) 
										{
											qd.removeChild(qd_2);
											QMessageBox::about(this, "Tip", "Delete Pw_Values in file successfully");
											break;
										}
										flag_point++;
									}
								
								}
								break;
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
void point_page::save() 
{
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QString c = ui.tableWidget->item(num, 2)->text();
	QString d = ui.tableWidget->item(num, 3)->text();
	QString e = ui.tableWidget->item(num, 4)->text();
	if (num + 1 > scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints().size()) 
	{
		sce::Point new_data(a.toDouble(), b.toDouble(), c.toDouble(), d.toDouble(), e.toDouble());
		scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints().push_back(new_data);
		QMessageBox::about(this, tr("Tip"), tr("Save Mission successfully"));
		QDomElement ver = dom.createElement("TargetPoint");
		QDomElement third_1 = dom.createElement("Altitude");
		QDomElement third_2 = dom.createElement("Latitude");
		QDomElement third_3 = dom.createElement("Longitude");
		QDomElement third_4 = dom.createElement("Tmin");
		QDomElement third_5 = dom.createElement("Tmax");
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
		ver.appendChild(third_1);
		ver.appendChild(third_2);
		ver.appendChild(third_3);
		ver.appendChild(third_4);
		ver.appendChild(third_5);
		QDomNodeList list = dom.elementsByTagName("Mission");
		QDomElement ele = list.at(choice_ownplatform).toElement();
		ele.appendChild(ver);
	}
	else 
	{
		scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[num].setAltitude(a.toDouble());
		scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[num].setLatitude(b.toDouble());
		scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[num].setLongitude(c.toDouble());
		scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[num].setTmin(d.toDouble());
		scenario.getAllOwnPlatform()[choice_ownplatform]->getMission().getAllTargetPoints()[num].setTmax(e.toDouble());
		QMessageBox::about(this, tr("Tip"), tr("Save TargetPoint successfully"));
		QDomNodeList list = dom.elementsByTagName("Mission");
		int z = 0;
		for (QDomNode qd = list.at(choice_ownplatform).firstChild(); !qd.isNull(); qd = qd.nextSibling())
		{
			if (qd.nodeName() == "TargetPoint")
			{
				if (z == num)
				{
					qd.firstChild().firstChild().setNodeValue(a);
					qd.firstChild().nextSibling().firstChild().setNodeValue(b);
					qd.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
					qd.firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
					qd.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(e);
				}
				z++;
			}
		}
	}
}