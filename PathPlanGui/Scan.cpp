#include "Scan_page.h"

Scan_page::Scan_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(500, 100);
	QStringList headers_scan;
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	QStringList headers_pw;
	//QPushButton *add = new QPushButton(this);
	//QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	//add->setText("add");
	//del->setText("del");
	save->setText("save");
	headers_scan << QStringLiteral("ScanType") << QStringLiteral("Spmax") << QStringLiteral("Spmin");
	ui.tableWidget->setColumnCount(3);
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setHorizontalHeaderLabels(headers_scan);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 281));
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
	connect(save, SIGNAL(clicked()), this, SLOT(save()));
	scantype_name << "Sectorial";
	
}

Scan_page::~Scan_page()
{
}
void Scan_page::show_scan() {
	int num = scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes().size();
	if (choice_radar < num) {
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getScan().getScanMax()))));
		ui.tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getScan().getScanMin()))));
		ui.tableWidget->setCellWidget(0, 0, scantype);
		scantype->addItems(scantype_name);
		this->show();
	}
	else {
		ui.tableWidget->clearContents();
		this->show();
	}
}
void Scan_page::save() {
	int num = ui.tableWidget->currentRow();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(num, 0));
	int re = cell->currentIndex();
	QString a = ui.tableWidget->item(num, 1)->text();
	QString b = ui.tableWidget->item(num, 2)->text();
	sce::Scan new_data(a.toDouble(),b.toDouble(),sce::ScanType(re));
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->setScan(new_data);
	QMessageBox::about(this, tr("Tip"), tr("Save Scan successfully"));
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
								if (qd_2.nodeName() == "Scan")
								{
									for (QDomNode qd_3 = qd_2.firstChild(); !qd_3.isNull(); qd_3 = qd_3.nextSibling())
									{
										if (qd_3.nodeName() == "ScanType")
										{
											qd_3.firstChild().setNodeValue(scantype_name[re]);
										}
										if (qd_3.nodeName() == "Spmax")
										{
											qd_3.firstChild().setNodeValue(a);
										}
										if (qd_3.nodeName() == "Spmin")
										{
											qd_3.firstChild().setNodeValue(b);
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
//void Scan_page::add() {
//	int row_count = ui.tableWidget->rowCount(); //获取表单行数
//	ui.tableWidget->insertRow(row_count);//添加新的一行
//	ui.tableWidget->setCellWidget(row_count, 0, new QComboBox());
//	QComboBox* cell = qobject_cast<QComboBox*>(ui.tableWidget->cellWidget(row_count, 0));
//	cell->addItems(scantype_name);
//}
//void Scan_page::del()		//删除列表数据
//{
//	int num = ui.tableWidget->currentRow();
//	int ree = QMessageBox::information(this, "", "Confirm deletion ?", QStringLiteral("Yes"), QStringLiteral("No"));
//	if (ree != 0)
//	{
//		return;
//	}
//	else {
//		ui.tableWidget->removeRow(num);
//	}
//}
