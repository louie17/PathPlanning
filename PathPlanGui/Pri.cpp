#include "Pri_page.h"
#include <string>


Pri_page::Pri_page(QMainWindow *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	resize(500, 100);
	QWidget *widget = new QWidget();
	QStringList headers_Pri;
	QPushButton *btn = new QPushButton();
	QVBoxLayout *layout = new QVBoxLayout();
	//QPushButton *add = new QPushButton(this);
	//QPushButton *del = new QPushButton(this);
	QPushButton *save = new QPushButton(this);
	//add->setText("add");
	//del->setText("del");
	save->setText("save");
	btn->setText("View");
	headers_Pri << QStringLiteral("Primin") << QStringLiteral("Primax") << QStringLiteral("PriType") << QStringLiteral("Pri_Values");
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setCellWidget(0, 3, btn);
	ui.tableWidget->setHorizontalHeaderLabels(headers_Pri);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 300));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	//layout->addWidget(add);
	//layout->addWidget(del);
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	setCentralWidget(widget);
	connect(btn, SIGNAL(clicked()), this, SLOT(show_pri_va()));
	//connect(add, SIGNAL(clicked()), this, SLOT(add()));
	//connect(del, SIGNAL(clicked()), this, SLOT(del()));
	connect(save, SIGNAL(clicked()), this, SLOT(save()));
	pritype_list << "Stagger" << "Switching" << "Fix";
	pritype->addItems(pritype_list);

}

Pri_page::~Pri_page()
{
}
void Pri_page::show_pri() {
	int num = scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes().size();
	if (choice_radar < num) {
		ui.tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getPriMin()))));
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(QString::number((scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getPriMax()))));
		if (scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getPriType() == sce::PriType::STAGGER) {
			pritype->setCurrentIndex(0);
		}
		else if (scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().getPriType() == sce::PriType::SWITCHING) {
			pritype->setCurrentIndex(1);
		}
		else {
			pritype->setCurrentIndex(2);
		}
		ui.tableWidget->setCellWidget(0, 2, pritype);
		this->show();
	}
	else {
		ui.tableWidget->setRowCount(1);
		ui.tableWidget->setItem(0, 0, new QTableWidgetItem(""));
		ui.tableWidget->setItem(0, 1, new QTableWidgetItem(""));
		ui.tableWidget->setCellWidget(0, 2, new QComboBox());
		auto cellWidget = ui.tableWidget->cellWidget(0, 2);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		combox->addItems(pritype_list);
		QPushButton *but = new QPushButton();
		but->setText("View");
		ui.tableWidget->setCellWidget(0, 3, but);
		connect(but, SIGNAL(clicked()), this, SLOT(emit_add()));
		this->show();
	}
}

void Pri_page::show_pri_va() {
	emit sign_pri_values();
}
void Pri_page::save() {
	int num = ui.tableWidget->currentRow();
	QString a = ui.tableWidget->item(num, 0)->text();
	QString b = ui.tableWidget->item(num, 1)->text();
	QComboBox *cell = qobject_cast<QComboBox*> (ui.tableWidget->cellWidget(num, 2));
	int c = cell->currentIndex();
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().setPriMin(a.toDouble());
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().setPriMax(b.toDouble());
	scenario.getAllEmitter()[choice_emitter]->getAllPtr2RadarModes()[choice_radar]->getPri().setPriType(sce::PriType(c));
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
								if (qdd.nodeName() == "Pri")
								{
									for (QDomNode qddd = qdd.firstChild(); !qddd.isNull(); qddd = qddd.nextSibling())
									{
										if (qddd.nodeName() == "Primin")
										{
											qddd.firstChild().setNodeValue(a);
										}
										if (qddd.nodeName() == "Primax")
										{
											qddd.firstChild().setNodeValue(b);
										}
										if (qddd.nodeName() == "PriType")
										{
											qddd.firstChild().setNodeValue(pritype_list[c]);
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
//void Pri_page::add() {
//	int row_count = ui.tableWidget->rowCount(); //获取表单行数
//	ui.tableWidget->insertRow(row_count);//添加新的一行
//	QPushButton *but = new QPushButton();
//	but->setText("Pri_values");
//	ui.tableWidget->setCellWidget(row_count, 3, but);
//	connect(but, SIGNAL(clicked()), this, SLOT(emit_add()));
//}
//void Pri_page::del()		//删除列表数据
//{
//	QItemSelectionModel *so = ui.tableWidget->selectionModel();//选择模式
//	QString msg = ("Confirm deletion ?");
//	QModelIndexList s = so->selectedIndexes();
//	if (s.size() > 0)
//	{
//		int ree = QMessageBox::information(this, "Tip", msg, QStringLiteral("Yes"), QStringLiteral("No"));
//		if (ree != 0)
//		{
//			return;
//		}
//		else {
//			ui.tableWidget->removeRow(s[0].row());
//		}
//	}
//}
void Pri_page::emit_add() {
	emit sign_add_new();
}
void Pri_page::add_new_pri() {
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setItem(0, 0, new QTableWidgetItem(""));
	ui.tableWidget->setItem(0, 1, new QTableWidgetItem(""));
	ui.tableWidget->setCellWidget(0, 2, new QComboBox());
	auto cellWidget = ui.tableWidget->cellWidget(0, 2);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	combox->addItems(pritype_list);
	QPushButton *but = new QPushButton();
	but->setText("Pri_values");
	ui.tableWidget->setCellWidget(0, 3, but);
	connect(but, SIGNAL(clicked()), this, SLOT(emit_add()));
	this->show();
}