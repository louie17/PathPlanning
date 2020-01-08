#include "DwellSquence_page.h"

DwellSquence_page::DwellSquence_page(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("DwellSquence"));
	resize(600, 400);
	QStringList headers_pw;
	QWidget *widget = new QWidget(this);
	QPushButton *add = new QPushButton();
	QPushButton *del = new QPushButton();
	QPushButton *save_btn = new QPushButton();
	QVBoxLayout *layout = new QVBoxLayout();
	add->setText("add");
	del->setText("del");
	save_btn->setText("save");
	headers_pw << QStringLiteral("Index") << QStringLiteral("MinFreq") << QStringLiteral("MaxFreq")<<QStringLiteral("StartTime") << QStringLiteral("EndTime");
	ui.tableWidget->setColumnCount(5);
	ui.tableWidget->setHorizontalHeaderLabels(headers_pw);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 300));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save_btn);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	connect(add, SIGNAL(clicked()), this, SLOT(add()));
	connect(del, SIGNAL(clicked()), this, SLOT(del()));
	connect(save_btn, SIGNAL(clicked()), this, SLOT(save()));
	this->setCentralWidget(widget);
}

DwellSquence_page::~DwellSquence_page()
{
}
void DwellSquence_page::add() {
	int num = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(num);
}
void DwellSquence_page::del() {
	int num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences().size()) {
			scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->deleteDwellSquence(num);
			QMessageBox::about(this, "Tip", "Delete DwellSquence successfully");
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
						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
						{
							if (qd.nodeName() == "Section")
							{
								int flag_point = 0;
								for (QDomNode qd_2 = qd.firstChild(); !qd_2.isNull(); qd_2 = qd_2.nextSibling())
								{
									if (qd_2.nodeName() == "DwellSquence")
									{
										if (flag_point == num)
										{
											qd.removeChild(qd_2);
											QMessageBox::about(this, "Tip", "Delete DwellSquence in file successfully");
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
void DwellSquence_page::save() {
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QString c = ui.tableWidget->item(num, 2)->text();
	QString d = ui.tableWidget->item(num, 3)->text();
	QString e = ui.tableWidget->item(num, 4)->text();
	if (num + 1 > scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences().size()) {
		sce::DwellSquence new_data(a.toInt(),b.toInt(),c.toInt(),d.toInt(),e.toInt());
		scenario.getAllEsmStrategy()[choice_esmstrategy ]->getAllPtr2Sections()[choice_section]->addDwellSquence(new_data);
		QMessageBox::about(this, tr("Tip"), tr("Add DwellSquence successfully"));
		QDomElement DS_node = dom.createElement("DwellSquence");
		QDomElement INDEX_node = dom.createElement("Index");
		QDomElement MIN_node = dom.createElement("MinFreq");
		QDomElement MAX_node = dom.createElement("MaxFreq");
		QDomElement ST_node = dom.createElement("StartTime");
		QDomElement END_node = dom.createElement("EndTime");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(b);
		QDomText text3 = dom.createTextNode(c);
		QDomText text4 = dom.createTextNode(d);
		QDomText text5 = dom.createTextNode(e);
		INDEX_node.appendChild(text1);
		MIN_node.appendChild(text2);
		MAX_node.appendChild(text3);
		ST_node.appendChild(text4);
		END_node.appendChild(text5);
		DS_node.appendChild(INDEX_node);
		DS_node.appendChild(MIN_node);
		DS_node.appendChild(MAX_node);
		DS_node.appendChild(ST_node);
		DS_node.appendChild(END_node);
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
							if (flag_section == choice_section)
							{
								qd.appendChild(DS_node);
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
	else {
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[num].setIndex(a.toInt());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[num].setMinFreq(b.toInt());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[num].setMaxFreq(c.toInt());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[num].setStartTime(d.toInt());
		scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[num].setEndTime(e.toInt());
		QMessageBox::about(this, tr("Tip"), tr("Save DwellSquence successfully"));
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
						if (qd.nodeName() == "Section")//找到对应位置的Section
						{
							if (flag_section == choice_section)
							{
								int flag_ds = 0;
								for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling()) 
								{
									if (qdd.nodeName() == "DwellSquence") 
									{
										if (flag_ds == num) 
										{
											qdd.firstChild().firstChild().setNodeValue(a);
											qdd.firstChild().nextSibling().firstChild().setNodeValue(b);
											qdd.firstChild().nextSibling().nextSibling().firstChild().setNodeValue(c);
											qdd.firstChild().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(d);
											qdd.firstChild().nextSibling().nextSibling().nextSibling().nextSibling().firstChild().setNodeValue(e);
											break;
										}
										flag_ds++;
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
void DwellSquence_page::show_dwellsquence()
{
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);
	if (scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences().size() > 0) {
		for (int i = 0; i < scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences().size(); i++)
		{
			ui.tableWidget->insertRow(i);
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[i].getIndex())));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[i].getMinFreq(), 'f', 2)));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[i].getMaxFreq(), 'f', 2)));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[i].getStartTime(), 'f', 2)));
			ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(scenario.getAllEsmStrategy()[choice_esmstrategy]->getAllPtr2Sections()[choice_section]->getAllDwellSquences()[i].getEndTime(), 'f', 2)));
		}
		this->show();
	}
	else 
	{
		ui.tableWidget->clearContents();
		ui.tableWidget->setRowCount(0);
		this->show();
	}
}
void DwellSquence_page::added_dwellsquence() {
	ui.tableWidget->clearContents();
	this->show();
}