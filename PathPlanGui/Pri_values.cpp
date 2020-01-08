#include "Pri_values_page.h"
#include <QMainWindow>

Pri_values_page::Pri_values_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(600, 200);
	QWidget *widget = new QWidget();
	setCentralWidget(widget);
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *add = new QPushButton(this);
	QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton();
	add->setText("add");
	del->setText("del");
	save->setText("save");
	QStringList headers_Pri;
	headers_Pri << QStringLiteral("PhaseOrder") << QStringLiteral("NumberOfPulses") << QStringLiteral("Primin") << QStringLiteral("Primax") << QStringLiteral("SweepTime");
	tableWidget->setColumnCount(5);
	tableWidget->setRowCount(3);
	tableWidget->setHorizontalHeaderLabels(headers_Pri);
	tableWidget->setGeometry(QRect(200, 70, 700, 300));
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(tableWidget);
	widget->setLayout(layout);
	connect(add, SIGNAL(clicked()), this, SLOT(add()));
	connect(del, SIGNAL(clicked()), this, SLOT(del()));
	connect(save, SIGNAL(clicked()), this, SLOT(save()));
}

Pri_values_page::~Pri_values_page()
{
}
void Pri_values_page::save() {
	int num = tableWidget->currentRow();
	QString a = tableWidget->item(num, 0)->text();
	QString b = tableWidget->item(num, 1)->text();
	QString c = tableWidget->item(num, 2)->text();
	QString d = tableWidget->item(num, 3)->text();
	QString e = tableWidget->item(num, 4)->text();
	sce::Pri_values new_data(a.toInt(), b.toInt(), c.toInt(), d.toInt(), e.toInt());
	if (num+1>scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues().size())
	{
		scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().addPriValues(new_data);
		QMessageBox::about(this, tr("Tip"), tr("Save Pri_values successfully"));
		QDomElement rf_v = dom.createElement("Pri_Values");
		QDomElement third_1 = dom.createElement("PhaseOrder");
		QDomElement third_2 = dom.createElement("NumberOfPulses");
		QDomElement third_3 = dom.createElement("Primin");
		QDomElement third_4 = dom.createElement("Primax");
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
									if (qdd.nodeName() == "Pri")
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
		scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().setPriValues(num,new_data);
		QMessageBox::about(this, tr("Tip"), tr("Save Pri_values successfully"));
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
														if (qd_4.nodeName() == "Primin")
														{
															qd_4.firstChild().setNodeValue(c);
														}
														if (qd_4.nodeName() == "Primax")
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
void Pri_values_page::show_pri_v() {
	int num = scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues().size();
	if (tableWidget->currentRow() < num) {
		for (int i = 0; i < scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues().size(); i++) {
			tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues()[i].getPhaseOrder())));
			tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues()[i].getNumberOfPulses())));
			tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues()[i].getMin(), 'f', 2)));
			tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues()[i].getMax())));
			tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues()[i].getSweepTime(), 'f', 2)));
		}
		this->show();
	}
	else {
		tableWidget->clearContents();
		this->show();
	}
}
void Pri_values_page::add() {
	int row_count = tableWidget->rowCount(); //获取表单行数
	tableWidget->insertRow(row_count);//添加新的一行
}
void Pri_values_page::del()		//删除列表数据
{
	int num = tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		tableWidget->removeRow(num);
		if (num < scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getAllPriValues().size()) 
		{
			scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().deletePriValues(num);
			QMessageBox::about(this, "Tip", "Delete Pri_Values successfully");
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
										if (qd_2.nodeName() == "Pri")
										{
											int flag_rf_v = 0;
											for (QDomNode qd_3 = qd_2.firstChild(); !qd_3.isNull(); qd_3 = qd_3.nextSibling())
											{
												if (qd_3.nodeName() == "Pri_Values")
												{
													if (flag_rf_v == num)
													{
														qd_2.removeChild(qd_3);
														QMessageBox::about(this, "Tip", "Delete Pri_Values in file successfully");
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