#include "Rf_page.h"

Rf_page::Rf_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(500, 100);
	QStringList headers_rf;
	QWidget *widget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *btn = new QPushButton();
	QPushButton *btn_c = new QPushButton();
	//QPushButton *add = new QPushButton(this);
	//QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	//add->setText("add");
	//del->setText("del");
	save->setText("save");
	btn->setText("Rf_Values");
	btn_c->setText("Rf_Values");
	headers_rf << QStringLiteral("Rfmin") << QStringLiteral("Rfmax") << QStringLiteral("RfType") << QStringLiteral("Rf_Values") ;
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setCellWidget(0, 3, btn);
	//ui.tableWidget->setCellWidget(1, 3, btn_c);
	ui.tableWidget->setHorizontalHeaderLabels(headers_rf);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	//layout->addWidget(add);
	//layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	setCentralWidget(widget);
	//connect(add, SIGNAL(clicked()), this, SLOT(add()));
	//connect(del, SIGNAL(clicked()), this, SLOT(del()));
	connect(btn, SIGNAL(clicked()), this, SLOT(show_rf_value()));
	connect(save, SIGNAL(clicked()), this, SLOT(save()));
	rftype_name << "Burst" << "Fix";
	rftype->addItems(rftype_name);
}

Rf_page::~Rf_page()
{
}

void Rf_page::rf_show() 
{
	int num = scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes().size();
	if (choice_radar < num) 
	{
		ui.tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getRfMin()))));
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getRfMax()))));
		if (scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getRfType() == sce::RfType::BURST) {
			rftype->setCurrentIndex(0);
		}
		if (scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getRfType() == sce::RfType::FIX) {
			rftype->setCurrentIndex(1);
		}
		ui.tableWidget->setCellWidget(0, 2, rftype);
		this->show();
	}
	else 
	{
		ui.tableWidget->setRowCount(1);
		ui.tableWidget->setItem(0, 0, new QTableWidgetItem(""));
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(""));
		ui.tableWidget->setCellWidget(0, 2, new QComboBox());
		auto temp = ui.tableWidget->cellWidget(0, 2);
		QComboBox *cell = qobject_cast<QComboBox*>(temp);
		cell->addItems(rftype_name);
		QPushButton *but = new QPushButton();
		but->setText("View");
		ui.tableWidget->setCellWidget(0, 3, but);
		connect(but, SIGNAL(clicked()), this, SLOT(add_new()));
		this->show();
	}
}

void Rf_page::show_rf_value() 
{
	emit sign_rf_value();
}
void Rf_page::save() 
{
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	//sce::Rf new_data(a.toInt(),b.toInt());
	auto temp = ui.tableWidget->cellWidget(num, 2);
	QComboBox* cell = qobject_cast<QComboBox*>(temp);
	int index = cell->currentIndex();
	//new_data.setRfType(sce::RfType(index));
	sce::Rf new_data(a.toInt(), b.toInt(), sce::RfType(index));
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->setRf(new_data);
	QMessageBox::about(this, tr("Tip"), tr("Save Rf successfully"));
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
									for (QDomNode qddd = qdd.firstChild(); !qddd.isNull(); qddd = qddd.nextSibling()) 
									{
										if (qddd.nodeName() == "Rfmin") 
										{
											qddd.firstChild().setNodeValue(a);
										}
										if (qddd.nodeName() == "Rfmax")
										{
											qddd.firstChild().setNodeValue(b);
										}
										if (qddd.nodeName() == "RfType")
										{
											qddd.firstChild().setNodeValue(rftype_name[index]);
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
//void Rf_page::add() 
//{
//	int row_count = ui.tableWidget->rowCount(); //获取表单行数
//	ui.tableWidget->insertRow(row_count);//添加新的一行
//	QPushButton *but = new QPushButton();
//	but->setText("Rf_values");
//	ui.tableWidget->setCellWidget(row_count,3, but);
//	connect(but, SIGNAL(clicked()), this, SLOT(add_new()));
//}
//void Rf_page::del()		//删除列表数据
//{
//	int num = ui.tableWidget->currentRow();
//	int ree = QMessageBox::information(this, "", "Confirm deletion?", QStringLiteral("Yes"), QStringLiteral("No"));
//	if (ree != 0)
//	{
//		return;
//	}
//	else {
//		ui.tableWidget->removeRow(num);
//		/*if (num < scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().getAllRfValues().size()) {
//			scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getRf().deleteRfValues(num);
//			QMessageBox::about(this, "Tip", "Delete OwnPlatformRouteRelation successfully");
//		}*/
//	}
//}
void Rf_page::add_new() {
	emit sign_add_new();
}
void Rf_page::add_new_Rf() {
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(""));
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(""));
	QPushButton *but = new QPushButton();
	but->setText("Rf_values");
	ui.tableWidget->setCellWidget(0, 3, but);
	connect(but, SIGNAL(clicked()), this, SLOT(add_new()));
	this->show();
}