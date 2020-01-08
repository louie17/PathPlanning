#include "Rf_values_page.h"
#include <QString>

Rf_values_page::Rf_values_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(600, 200);
	QWidget *widget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *add = new QPushButton(this);
	QPushButton *del = new QPushButton(this);
	QPushButton *save= new QPushButton(this);
	add->setText("add");
	del->setText("del");
	save->setText("save");
	QStringList headers_Erp;
	headers_Erp << QStringLiteral("PhaseOrder") << QStringLiteral("NumberOfPulses") << QStringLiteral("Rfmin") << QStringLiteral("Rfmax") << QStringLiteral("SweepTime");
	ui.tableWidget->setColumnCount(5);
	ui.tableWidget->setRowCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(headers_Erp);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	setCentralWidget(widget);
	connect(add, SIGNAL(clicked()), this, SLOT(add()));
	connect(del, SIGNAL(clicked()), this, SLOT(del()));
	connect(save, SIGNAL(clicked()), this, SLOT(save_rf_v()));
}

Rf_values_page::~Rf_values_page()
{
}
void Rf_values_page::save_rf_v() {
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QString c = ui.tableWidget->item(num, 2)->text();
	QString d = ui.tableWidget->item(num, 3)->text();
	QString e = ui.tableWidget->item(num, 4)->text();
	if (num + 1 > scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues().size()) {
		sce::Rf_values new_data(a.toInt(),b.toInt(),c.toInt(),d.toInt(),e.toInt());
		scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().addRfValues(new_data);
		QMessageBox::about(this, tr("Tip"), tr("Save Rf_values successfully"));
		QDomElement rf_v = dom.createElement("Rf_Values");
		QDomElement third_1 = dom.createElement("PhaseOrder");
		QDomElement third_2 = dom.createElement("NumberOfPulses");
		QDomElement third_3 = dom.createElement("Rfmin");
		QDomElement third_4 = dom.createElement("Rfmax");
		QDomElement third_5 = dom.createElement("SweepTime");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		QDomText text5 = dom.createTextNode(e);
		third_1.appendChild(text1);
		third_2.appendChild(text3);
		third_3.appendChild(text3);
		third_4.appendChild(text4);
		third_5.appendChild(text5);
		rf_v.appendChild(third_1);
		rf_v.appendChild(third_2);
		rf_v.appendChild(third_3);
		rf_v.appendChild(third_4);
		rf_v.appendChild(third_5);
		QDomNodeList list = dom.elementsByTagName("Emitter");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的Emitter
			{
				if (flag == choice_emitter)
				{
					int flag_2 = 0;
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "Radar_Mode")//找到具体的Radar_Mode
						{
							if (flag_2 == choice_radar)
							{
								for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
								{
									if (qdd.nodeName() == "Rf")
									{
										qdd.appendChild(rf_v);
										break;
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
	else {
		sce::Rf_values new_data(a.toInt(),b.toInt(),c.toInt(),d.toInt(),e.toInt());
		scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().setRfValues(num,new_data);
		QMessageBox::about(this, tr("Tip"), tr("Save Rf_values successfully"));
		QDomNodeList list = dom.elementsByTagName("Emitter");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的Emitter
			{
				if (flag == choice_emitter)
				{
					int flag_2 = 0;
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "Radar_Mode")//找到具体的Radar_Mode
						{
							if (flag_2 == choice_radar)
							{
								for (QDomNode qd_2 = qd.firstChild(); !qd_2.isNull(); qd_2 = qd_2.nextSibling())
								{
									if (qd_2.nodeName() == "Rf")
									{
										int flag_3 = 0;
										for (QDomNode qd_3 = qd_2.firstChild(); !qd_3.isNull(); qd_3 = qd_3.nextSibling())
										{
											if (qd_3.nodeName() == "Rf_Values")
											{
												if (flag_3 == num) 
												{
													for (QDomNode qd_4 = qd_3.firstChild(); !qd_4.isNull(); qd_4 = qd_4.nextSibling()) 
													{
														if (qd_4.nodeName() == "PhaseOrder") 
														{
															qd_4.firstChild().setNodeValue(a);
														}
														if (qd_4.nodeName() == "NumberOfPulses")
														{
															qd_4.firstChild().setNodeValue(b);
														}
														if (qd_4.nodeName() == "Rfmin")
														{
															qd_4.firstChild().setNodeValue(c);
														}
														if (qd_4.nodeName() == "Rfmax")
														{
															qd_4.firstChild().setNodeValue(d);
														}
														if (qd_4.nodeName() == "SweepTime")
														{
															qd_4.firstChild().setNodeValue(e);
														}
													}
													break;
												}
												flag_3++;
											}
											
										}
										break;
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
}
void Rf_values_page::show_rf_v()
{
	int num = scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues().size();
	if (ui.tableWidget->currentRow() < num) 
	{
		for (int i = 0; i < scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues().size(); i++) {
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues()[i].getPhaseOrder())));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues()[i].getNumberOfPulses())));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues()[i].getMin(), 'f', 2)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues()[i].getMax())));
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues()[i].getSweepTime(), 'f', 2)));
		}
		this->show();
	}
	else {
		ui.tableWidget->clearContents();
		this->show();
	}
}

void Rf_values_page::add() {
	int row_count = ui.tableWidget->rowCount(); //获取表单行数
	ui.tableWidget->insertRow(row_count);//添加新的一行
}
void Rf_values_page::del()		//删除列表数据
{
	int num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues().size())
		{
			scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().deleteRfValues(num);
			QMessageBox::about(this, "Tip", "Delete Rf_Values successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Emitter");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == choice_emitter)
					{
						int flag_radar = 0;
						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
						{
							if (qd.nodeName() == "Radar_Mode")
							{
								if (flag_radar == choice_radar)
								{
									for (QDomNode qd_2 = qd.firstChild(); !qd_2.isNull(); qd_2 = qd_2.nextSibling())
									{
										if (qd_2.nodeName() == "Rf")
										{
											int flag_rf_v = 0;
											for (QDomNode qd_3 = qd_2.firstChild(); !qd_3.isNull(); qd_3 = qd_3.nextSibling())
											{
												if (qd_3.nodeName() == "Rf_Values")
												{
													if (flag_rf_v == num)
													{
														qd_2.removeChild(qd_3);
														QMessageBox::about(this, "Tip", "Delete Rf_Values in file successfully");
														break;
													}
													flag_rf_v++;
												}
											}
											break;
										}
									}
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
void Rf_values_page::add_new() {
	ui.tableWidget->clearContents();
	this->show();
}