#include "Pw_page.h"

Pw_page::Pw_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(500, 100);
	QStringList headers_pw;
	QWidget *widget = new QWidget();
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *btn = new QPushButton();
	//QPushButton *add = new QPushButton(this);
	//QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	//add->setText("add");
	//del->setText("del");
	save->setText("save");
	btn->setText("Pw_Values");
	headers_pw << QStringLiteral("Pwmin") << QStringLiteral("Pwmax") << QStringLiteral("PwType") << QStringLiteral("Pw_Values");
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setCellWidget(0, 3, btn);
	ui.tableWidget->setHorizontalHeaderLabels(headers_pw);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 300));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	//layout->addWidget(add);
	//layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	setCentralWidget(widget);
	connect(btn, SIGNAL(clicked()), this, SLOT(show_pw_v()));
	//connect(add, SIGNAL(clicked()), this, SLOT(add()));
	//connect(del, SIGNAL(clicked()), this, SLOT(del()));
	connect(save, SIGNAL(clicked()), this, SLOT(save()));
	pwtype_list << "Fix";
	pwtype->addItems(pwtype_list);
}

Pw_page::~Pw_page()
{
}
void Pw_page::show_pw() {
	int num = scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes().size();
	if (choice_radar < num) {
		ui.tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPw().getPwMin()))));
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPw().getPwMax()))));
		ui.tableWidget->setCellWidget(0, 2, pwtype);
		this->show();
	}
	else {
		QPushButton *btn_new = new QPushButton();
		ui.tableWidget->setRowCount(1);
		ui.tableWidget->setItem(0, 0, new QTableWidgetItem(""));
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(""));
		ui.tableWidget->setCellWidget(0, 2, new QComboBox());
		auto cell = ui.tableWidget->cellWidget(0, 2);
		QComboBox *cb = qobject_cast<QComboBox*>(cell);
		cb->addItems(pwtype_list);
		btn_new->setText("View");
		ui.tableWidget->setCellWidget(0, 3, btn_new);
		connect(btn_new, SIGNAL(clicked()), this, SLOT(add_button()));
		this->show();
	}
}

void Pw_page::show_pw_v() {
	emit sign_pw_v();
}
void Pw_page::add_button() {
	emit sign_add();
}
void Pw_page::save() {
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QComboBox* cell = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(num, 2));
	int re = cell->currentIndex();
	sce::Pw new_data;
	new_data.setPwMin(a.toDouble());
	new_data.setPwMax(b.toDouble());
	new_data.setPwType(sce::PwType(re));
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->setPw(new_data);
	QMessageBox::about(this, tr("Tip"), tr("Save Pri successfully"));
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
								if (qdd.nodeName() == "Pw")
								{
									for (QDomNode qddd = qdd.firstChild(); !qddd.isNull(); qddd = qddd.nextSibling())
									{
										if (qddd.nodeName() == "Pwmin")
										{
											qddd.firstChild().setNodeValue(a);
										}
										if (qddd.nodeName() == "Pwmax")
										{
											qddd.firstChild().setNodeValue(b);
										}
										if (qddd.nodeName() == "PwType")
										{
											qddd.firstChild().setNodeValue(pwtype_list[re]);
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
//void Pw_page::add() {
//	int row_count = ui.tableWidget->rowCount();
//	ui.tableWidget->insertRow(row_count);
//	QPushButton *but = new QPushButton();
//	but->setText("Pw_values");
//	ui.tableWidget->setCellWidget(row_count, 3, but);
//	connect(but, SIGNAL(clicked()), this, SLOT(add_button()));
//}
//void Pw_page::del()		
//{
//	QItemSelectionModel *so = ui.tableWidget->selectionModel();
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
void Pw_page::add_new_pw() {
	QPushButton *btn_new = new QPushButton();
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(""));
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(""));
	ui.tableWidget->setCellWidget(0, 2, new QComboBox());
	auto cell = ui.tableWidget->cellWidget(0, 2);
	QComboBox *cb = qobject_cast<QComboBox*>(cell);
	cb->addItems(pwtype_list);
	btn_new->setText("View");
	ui.tableWidget->setCellWidget(0, 3, btn_new);
	connect(btn_new, SIGNAL(clicked()), this, SLOT(add_button()));
	this->show();
}