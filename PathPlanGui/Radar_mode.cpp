#include "Radar_mode_page.h"

Radar_mode_page::Radar_mode_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(800, 200);
	QWidget *widget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	
	QPushButton *add = new QPushButton(this);
	QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	QStringList headers_rada;
	headers_rada << QStringLiteral("ModeCode") << QStringLiteral("ModeType") << QStringLiteral("Rf") << QStringLiteral("Pw") << QStringLiteral("Pri") << QStringLiteral("Scan") << QStringLiteral("Erp");
	
	add->setText("add");
	del->setText("del");
	save->setText("save");
	
	ui.tableWidget->setColumnCount(7);

	ui.tableWidget->setHorizontalHeaderLabels(headers_rada);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 281));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	layout->addWidget(add);
	layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	setCentralWidget(widget);
	connect(save, SIGNAL(clicked()), this, SLOT(save_radar()));
	connect(add, SIGNAL(clicked()), this, SLOT(add()));
	connect(del, SIGNAL(clicked()), this, SLOT(del()));
	modetype_list << "Pulse";
}

Radar_mode_page::~Radar_mode_page()
{
	
}
void Radar_mode_page::rada_show() {
	int num = scenario.getAllEmitter().size();
	if (choice_emitter < num) 
	{
		ui.tableWidget->setRowCount(0);
		std::vector<std::shared_ptr<sce::Radar_Mode>> ptr_radarmodes{ scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes() };
		for (int i = 0; i < ptr_radarmodes.size(); i++) 
		{
			ui.tableWidget->insertRow(i);
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(ptr_radarmodes[i]->getModeCode())));

			ui.tableWidget->setCellWidget(i, 1, new QComboBox());
			auto cellWidget = ui.tableWidget->cellWidget(i, 1);
			QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);

			QStringList cb_modetypeList;
			if (ptr_radarmodes[i]->getModeType() == sce::ModeType::PULSE)
			{
				cb_modetypeList.append(tr("Pulse"));

			}
			combox->addItems(cb_modetypeList);
			ui.tableWidget->setCellWidget(i, 2, new QPushButton(tr("View")));
			auto rf_cellwidget = ui.tableWidget->cellWidget(i, 2);
			QPushButton *pb_rf = qobject_cast<QPushButton*>(rf_cellwidget);
			connect(pb_rf, &QPushButton::clicked, this, &Radar_mode_page::show_rf);

			ui.tableWidget->setCellWidget(i, 3, new QPushButton(tr("View")));
			auto pw_cellwidget = ui.tableWidget->cellWidget(i, 3);
			QPushButton *pb_pw = qobject_cast<QPushButton*>(pw_cellwidget);
			connect(pb_pw, &QPushButton::clicked, this, &Radar_mode_page::show_pw);

			ui.tableWidget->setCellWidget(i, 4, new QPushButton(tr("View")));
			auto pri_cellwidget = ui.tableWidget->cellWidget(i, 4);
			QPushButton *pb_pri = qobject_cast<QPushButton*>(pri_cellwidget);
			connect(pb_pri, &QPushButton::clicked, this, &Radar_mode_page::show_pri);

			ui.tableWidget->setCellWidget(i, 5, new QPushButton(tr("View")));
			auto scan_cellwidget = ui.tableWidget->cellWidget(i, 5);
			QPushButton *pb_scan = qobject_cast<QPushButton*>(scan_cellwidget);
			connect(pb_scan, &QPushButton::clicked, this, &Radar_mode_page::show_scan);

			ui.tableWidget->setCellWidget(i, 6, new QPushButton(tr("View")));
			auto erp_cellwidget = ui.tableWidget->cellWidget(i, 6);
			QPushButton *pb_erp = qobject_cast<QPushButton*>(erp_cellwidget);
			connect(pb_erp, &QPushButton::clicked, this, &Radar_mode_page::show_erp);
		}
		this->show();	
	}
	else 
	{
		ui.tableWidget->setRowCount(0);
		this->show();
	}
}
void Radar_mode_page::save_radar() {
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(num, 1));
	QString re = cell->currentText();
	if (num + 1 > scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes().size()) {
		sce::Radar_Mode new_data;
		new_data.setModeCode(a.toStdString());
		new_data.setModeType(sce::ModeType::PULSE);
		scenario.getAllEmitter()[choice_emitter]->addRadarMode(std::make_shared<sce::Radar_Mode>(new_data));
		QMessageBox::about(this, tr("Tip"), tr("Save Radar_Mode successfully"));
		QDomElement third = dom.createElement("Radar_Mode");
		QDomElement fourth_1 = dom.createElement("ModeCode");
		QDomElement fourth_2 = dom.createElement("ModeType");
		QDomText text1 = dom.createTextNode(a);
		QDomText text2 = dom.createTextNode(re);
		fourth_1.appendChild(text1);
		fourth_2.appendChild(text2);

		QDomElement fourth_3 = dom.createElement("Rf");
		QDomElement fifth_1 = dom.createElement("Rfmin");
		QDomElement fifth_2 = dom.createElement("Rfmax");
		QDomElement fifth_3 = dom.createElement("RfType");
		QDomText text_c = dom.createTextNode("0");
		QDomText text_c1 = dom.createTextNode("0");
		QDomText text_c2 = dom.createTextNode("0");
		fifth_1.appendChild(text_c);
		fifth_2.appendChild(text_c1);
		fifth_3.appendChild(text_c2);
		fourth_3.appendChild(fifth_1);
		fourth_3.appendChild(fifth_2);
		fourth_3.appendChild(fifth_3);

		QDomElement fourth_4 = dom.createElement("Pw");
		QDomElement fifth_4 = dom.createElement("Pwmin");
		QDomElement fifth_5 = dom.createElement("Pwmax");
		QDomElement fifth_6 = dom.createElement("PwType");
		QDomText text_c3 = dom.createTextNode("0");
		QDomText text_c4 = dom.createTextNode("0");
		QDomText text_c5 = dom.createTextNode("0");
		fifth_4.appendChild(text_c3);
		fifth_5.appendChild(text_c4);
		fifth_6.appendChild(text_c5);
		fourth_4.appendChild(fifth_4);
		fourth_4.appendChild(fifth_5);
		fourth_4.appendChild(fifth_6);

		QDomElement fourth_5 = dom.createElement("Pri");
		QDomElement fifth_7 = dom.createElement("Primin");
		QDomElement fifth_8 = dom.createElement("Primax");
		QDomElement fifth_9 = dom.createElement("PriType");
		QDomText text_c6 = dom.createTextNode("0");
		QDomText text_c7 = dom.createTextNode("0");
		QDomText text_c8 = dom.createTextNode("0");
		fifth_7.appendChild(text_c6);
		fifth_8.appendChild(text_c7);
		fifth_9.appendChild(text_c8);
		fourth_5.appendChild(fifth_7);
		fourth_5.appendChild(fifth_8);
		fourth_5.appendChild(fifth_9);

		QDomElement fourth_6 = dom.createElement("Scan");
		QDomElement fifth_10 = dom.createElement("ScanType");
		QDomElement fifth_11 = dom.createElement("Spmax");
		QDomElement fifth_12 = dom.createElement("Spmin");
		QDomText text_c9 = dom.createTextNode("0");
		QDomText text_c10 = dom.createTextNode("0");
		QDomText text_c11 = dom.createTextNode("0");
		fifth_10.appendChild(text_c9);
		fifth_11.appendChild(text_c10);
		fifth_12.appendChild(text_c11);
		fourth_6.appendChild(fifth_10);
		fourth_6.appendChild(fifth_11);
		fourth_6.appendChild(fifth_12);

		QDomElement fourth_7 = dom.createElement("Erp");
		QDomElement fifth_13 = dom.createElement("Erpmin");
		QDomElement fifth_14 = dom.createElement("Erpmax");
		QDomText text_c12 = dom.createTextNode("0");
		QDomText text_c13 = dom.createTextNode("0");
		fifth_13.appendChild(text_c12);
		fifth_14.appendChild(text_c13);
		fourth_7.appendChild(fifth_13);
		fourth_7.appendChild(fifth_14);

		third.appendChild(fourth_1);
		third.appendChild(fourth_2);
		third.appendChild(fourth_3);
		third.appendChild(fourth_4);
		third.appendChild(fourth_5);
		third.appendChild(fourth_6);
		third.appendChild(fourth_7);

		QDomNodeList list = dom.elementsByTagName("Emitter");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == choice_emitter)
				{
					ele.appendChild(third);
					break;
				}
				flag++;
			}
		}
	}
	else {
		scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[num]->setModeCode(a.toStdString());
		//scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[num]->setModeType(ui.tableWidget->item(num, 0)->text().toStdString());
		QMessageBox::about(this, tr("Tip"), tr("Save Radar_Mode successfully"));
		QDomNodeList list = dom.elementsByTagName("Emitter");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == choice_emitter)
				{
					ele.firstChild().firstChild().setNodeValue(a);
					ele.firstChild().nextSibling().firstChild().setNodeValue(re);
					break;
				}
				flag++;
			}
		}
	}
}
void Radar_mode_page::show_rf() {
	choice_radar = ui.tableWidget->currentRow();
	qDebug() << "choice_radar" << choice_radar;
	emit sign_rf();
}
void Radar_mode_page::show_pw() {
	choice_radar = ui.tableWidget->currentRow();
	qDebug() << "choice_radar" << choice_radar;
	emit sign_pw();
}
void Radar_mode_page::show_pri() {
	choice_radar = ui.tableWidget->currentRow();
	qDebug() << "choice_radar" << choice_radar;
	emit sign_pri();
}
void Radar_mode_page::show_scan() {
	choice_radar = ui.tableWidget->currentRow();
	qDebug() << "choice_radar" << choice_radar;
	emit sign_scan();
}
void Radar_mode_page::show_erp() {
	choice_radar = ui.tableWidget->currentRow();
	qDebug() << "choice_radar" << choice_radar;
	emit sign_erp();
}
void Radar_mode_page::add() {
	int row_count = ui.tableWidget->rowCount(); //获取表单行数
	ui.tableWidget->insertRow(row_count);//添加新的一行
	QPushButton *but = new QPushButton();
	QPushButton *but1 = new QPushButton();
	QPushButton *but2 = new QPushButton();
	QPushButton *but3 = new QPushButton();
	QPushButton *but4= new QPushButton();
	but->setText("Rf");
	but1->setText("Pw");
	but2->setText("Pri");
	but3->setText("Scan");
	but4->setText("Erp");
	ui.tableWidget->setCellWidget(row_count, 1, new QComboBox());
	ui.tableWidget->setCellWidget(row_count, 2, but);
	ui.tableWidget->setCellWidget(row_count, 3, but1);
	ui.tableWidget->setCellWidget(row_count, 4, but2);
	ui.tableWidget->setCellWidget(row_count, 5, but3);
	ui.tableWidget->setCellWidget(row_count, 6, but4);
	auto cellWidget = ui.tableWidget->cellWidget(row_count, 1);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	QStringList list;
	list << "Pulse";
	combox->addItems(list);
	connect(but, SIGNAL(clicked()), this, SLOT(show_rf()));
	connect(but1, SIGNAL(clicked()), this, SLOT(show_pw()));
	connect(but2, SIGNAL(clicked()), this, SLOT(show_pri()));
	connect(but3, SIGNAL(clicked()), this, SLOT(show_scan()));
	connect(but4, SIGNAL(clicked()), this, SLOT(show_erp()));

}
void Radar_mode_page::del()		//删除列表数据
{
	size_t num = ui.tableWidget->currentRow();
	int ree = QMessageBox::information(this, "Tip", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeRow(num);
		if (num < scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes().size()) {
			scenario.getAllEmitter()[choice_emitter]->deleteRadarMode(num);
			QMessageBox::about(this, "Tip", "Delete RadarMode successfully");
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
								if (flag_radar == num) 
								{
									ele.removeChild(qd);
									QMessageBox::about(this, "Tip", "Delete Radar_mode in file successfully");
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