#include "Erp_page.h"

Erp_page::Erp_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(300, 100);
	QWidget *widget = new QWidget();
	QStringList headers_Erp;
	QVBoxLayout *layout = new QVBoxLayout();
	//QPushButton *add = new QPushButton(this);
	//QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	//add->setText("add");
	//del->setText("del");
	save->setText("save");
	headers_Erp << QStringLiteral("Erpmin") << QStringLiteral("Erpmax") << QStringLiteral("ErpType") << QStringLiteral("Erp_Values");
	ui.tableWidget->setColumnCount(2);	
	ui.tableWidget->setHorizontalHeaderLabels(headers_Erp);
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
}

Erp_page::~Erp_page()
{
}
void Erp_page::show_erp(){
	ui.tableWidget->setRowCount(2);
	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getErp().getErpMin())));
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number(scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getErp().getErpMax())));
	this->show();
}
void Erp_page::save() {
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	sce::Erp new_data(a.toDouble(), b.toDouble());
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->setErp(new_data);
	QMessageBox::about(this, tr("Tip"), tr("Save Erp successfully"));
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
								if (qd_2.nodeName() == "Erp")
								{
									for (QDomNode qd_3 = qd_2.firstChild(); !qd_3.isNull(); qd_3 = qd_3.nextSibling())
									{
										if (qd_3.nodeName() == "Erpmin")
										{
											qd_3.firstChild().setNodeValue(a);
										}
										if (qd_3.nodeName() == "Erpmax")
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
//void Erp_page::add() {
//	int row_count = ui.tableWidget->rowCount(); //获取表单行数
//	ui.tableWidget->insertRow(row_count);//添加新的一行
//}
//void Erp_page::del()		//删除列表数据
//{
//	QItemSelectionModel *so = ui.tableWidget->selectionModel();//选择模式
//	QString msg = ("Confirm deletion ?");
//	QModelIndexList s = so->selectedIndexes();
//	if (s.size() > 0)
//	{
//		int ree = QMessageBox::information(this, "", msg, QStringLiteral("Yes"), QStringLiteral("No"));
//		if (ree != 0)
//		{
//			return;
//		}
//		else {
//			ui.tableWidget->removeRow(s[0].row());
//		}
//	}
//}
void Erp_page::add_new_erp() {
	ui.tableWidget->clearContents();
	this->show();
}