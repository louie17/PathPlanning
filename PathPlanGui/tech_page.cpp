#include "tech_page.h"

tech_page::tech_page(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("tech"));
	resize(600, 200);
	QStringList headers_pw;
	headers_pw << QStringLiteral("Name") ;
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setVerticalHeaderLabels(headers_pw);
	ui.tableWidget->setGeometry(QRect(200, 70, 700, 300));
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	tech_list << "Noise" << "Vgpo" << "Ft" << "Rgpo";
}

tech_page::~tech_page()
{
}
void tech_page::save_ecmstrategy_tech() 
{
	QComboBox *cb = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(0, 0));
	int num = cb->currentIndex();
	scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[choice_section]->setTechName(sce::Tech(num));
	QMessageBox::about(this, tr("Tip"), tr("Save Tech successfully"));
	QDomNodeList list = dom.elementsByTagName("EcmStrategy");
	int flag = 0;
	for (int i = 0; i < list.count(); i++)
	{
		QDomElement ele = list.at(i).toElement();
		if (ele.parentNode().nodeName() == "Scenario")//找到为第二级节点的EcmStrategy
		{
			if (flag == choice_ecmstrategy)
			{
				int flag_section = 0;
				for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
				{
					if (qd.nodeName() == "Section")
					{
						if (flag_section == choice_section)
						{
							for (QDomNode qdd = qd.firstChild(); !qdd.isNull(); qdd = qdd.nextSibling())
							{
								if (qdd.nodeName() == "tech")
								{
									qdd.firstChild().firstChild().setNodeValue(tech_list[num]);
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
void tech_page::save_ecm_tech()
{
	int cur = ui.tableWidget->currentColumn();
	QComboBox *cb1 = qobject_cast<QComboBox *>(ui.tableWidget->cellWidget(0, cur));
	int num = cb1->currentIndex();
	if (cur + 1 > scenario.getAllEcm()[choice_ecm]->getAllTechs().size()) {
		scenario.getAllEcm()[choice_ecm]->addTech(sce::Tech(num));
		QMessageBox::about(this, tr("Tip"), tr("Save Tech successfully"));
		QDomNodeList list = dom.elementsByTagName("Ecm");
		QDomElement second = dom.createElement("tech");
		QDomElement third_1 = dom.createElement("Name");
		QDomText text1 = dom.createTextNode(tech_list[num]);
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == choice_ecm)
				{
					third_1.appendChild(text1);
					second.appendChild(third_1); 
					ele.appendChild(second);				
					break;
				}
				flag++;
			}
		}
	}
	else {
		scenario.getAllEcm()[choice_ecm]->setTech(cur, sce::Tech(num));
		QMessageBox::about(this, tr("Tip"), tr("Save Tech successfully"));
		QDomNodeList list = dom.elementsByTagName("Ecm");
		int flag = 0;
		for (int i = 0; i < list.count(); i++)
		{
			QDomElement ele = list.at(i).toElement();
			if (ele.parentNode().nodeName() == "Scenario")
			{
				if (flag == choice_ecm)
				{
					for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
					{
						if (qd.nodeName() == "tech")
						{
							qd.firstChild().firstChild().setNodeValue(tech_list[num]);
						}
					}					
					break;
				}
				flag++;
			}
		}
	}
}
void tech_page::added_ecmstrategy_tech() 
{
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *save_ecmstrategy_tech = new QPushButton();
	save_ecmstrategy_tech->setText("save");
	connect(save_ecmstrategy_tech, SIGNAL(clicked()), this, SLOT(save_ecmstrategy_tech()));
	layout->addWidget(save_ecmstrategy_tech);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	this->setCentralWidget(widget);
	ui.tableWidget->setColumnCount(0);
	ui.tableWidget->insertColumn(0);
	ui.tableWidget->setCellWidget(0, 0, new QComboBox());
	auto cellWidget = ui.tableWidget->cellWidget(0, 0);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	combox->addItems(tech_list);
	this->show();
}
void tech_page::added_ecm_tech()
{
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *save_ecm_tech = new QPushButton();
	save_ecm_tech->setText("save");
	connect(save_ecm_tech, SIGNAL(clicked()), this, SLOT(save_ecm_tech()));
	layout->addWidget(save_ecm_tech);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	this->setCentralWidget(widget);
	ui.tableWidget->setColumnCount(0);
	/*ui.tableWidget->insertColumn(0);
	ui.tableWidget->setCellWidget(0, 0, new QComboBox());
	auto cellWidget = ui.tableWidget->cellWidget(0, 0);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	combox->addItems(tech_list);*/
	this->show();
}
void tech_page::add_tech() 
{
	int num = ui.tableWidget->columnCount();
	ui.tableWidget->insertColumn(num);
	ui.tableWidget->setCellWidget(0, num,new QComboBox());
	auto cellWidget = ui.tableWidget->cellWidget(0, num);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	combox->addItems(tech_list);
	this->show();
}
void tech_page::del_tech()
{
	int num = ui.tableWidget->currentColumn();
	int ree = QMessageBox::information(this, "", "Confirm deletion ?", QStringLiteral("Yes"), QStringLiteral("No"));
	if (ree != 0)
	{
		return;
	}
	else {
		ui.tableWidget->removeColumn(num);
		if (num < scenario.getAllEcm()[choice_ecm]->getAllTechs().size()) 
		{
			scenario.getAllEcm()[choice_ecm]->deleteTech(num);
			QMessageBox::about(this, "Tip", "Delete tech successfully");
			QDomElement root = dom.documentElement();
			QDomNodeList list_node = dom.elementsByTagName("Ecm");
			int flag = 0;
			for (int i = 0; i < list_node.count(); i++)
			{
				QDomElement ele = list_node.at(i).toElement();
				if (ele.parentNode().nodeName() == "Scenario")
				{
					if (flag == choice_ecm)
					{
						int flag_tech = 0;
						for (QDomNode qd = ele.firstChild(); !qd.isNull(); qd = qd.nextSibling())
						{
							if (qd.nodeName() == "tech")
							{
								if (flag_tech == num)
								{
									ele.removeChild(qd);
									QMessageBox::about(this, "Tip", "Delete tech in file successfully");
									break;
								}
								flag_tech++;
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
void tech_page::show_ecmstragegy_tech()
{	
	QWidget *widget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout();
	QPushButton *save = new QPushButton();
	save->setText("save");
	connect(save, SIGNAL(clicked()), this, SLOT(save_ecmstrategy_tech()));
	layout->addWidget(save);
	layout->addWidget(ui.tableWidget);
	widget->setLayout(layout);
	this->setCentralWidget(widget);
	ui.tableWidget->setColumnCount(0);
	ui.tableWidget->insertColumn(0);
	ui.tableWidget->setCellWidget(0, 0, new QComboBox());
	auto cellWidget = ui.tableWidget->cellWidget(0, 0);
	QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
	combox->addItems(tech_list);
	if (scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[choice_section]->getTechName() == sce::Tech::NOISE ) 
	{
		combox->setCurrentIndex(0);
	}
	if (scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[choice_section]->getTechName() == sce::Tech::VGPO) 
	{
		combox->setCurrentIndex(1);
	}
	if (scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[choice_section]->getTechName() == sce::Tech::FT) 
	{
		combox->setCurrentIndex(2);
	}
	if (scenario.getAllEcmStrategy()[choice_ecmstrategy]->getAllPtr2Sections()[choice_section]->getTechName() == sce::Tech::RGPO) 
	{
		combox->setCurrentIndex(3);
	}
	this->show();
}
void tech_page::show_ecm_tech() 
{
	//int num = ui.tableWidget->currentRow();
	if (choice_ecm < scenario.getAllEcm().size()) {
		QPushButton *add = new QPushButton();
		QPushButton *del = new QPushButton();
		QWidget *widget = new QWidget(this);
		QVBoxLayout *layout = new QVBoxLayout();
		QPushButton *save = new QPushButton();
		add->setText("Add");
		del->setText("Del");
		save->setText("save");
		connect(add, SIGNAL(clicked()), this, SLOT(add_tech()));
		connect(del, SIGNAL(clicked()), this, SLOT(del_tech()));
		connect(save, SIGNAL(clicked()), this, SLOT(save_ecm_tech()));
		layout->addWidget(add);
		layout->addWidget(del);
		layout->addWidget(save);
		layout->addWidget(ui.tableWidget);
		widget->setLayout(layout);
		this->setCentralWidget(widget);
		ui.tableWidget->setColumnCount(0);
		for (int i = 0; i < scenario.getAllEcm()[choice_ecm]->getAllTechs().size(); i++)
		{
			ui.tableWidget->insertColumn(i);
			ui.tableWidget->setCellWidget(0, i, new QComboBox());
			auto cellWidget = ui.tableWidget->cellWidget(0, i);
			QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
			combox->addItems(tech_list);
			if (scenario.getAllEcm()[choice_ecm]->getAllTechs()[i] == sce::Tech::NOISE)
			{
				combox->setCurrentIndex(0);
			}
			if (scenario.getAllEcm()[choice_ecm]->getAllTechs()[i] == sce::Tech::VGPO)
			{
				combox->setCurrentIndex(1);
			}
			if (scenario.getAllEcm()[choice_ecm]->getAllTechs()[i] == sce::Tech::FT)
			{
				combox->setCurrentIndex(2);
			}
			if (scenario.getAllEcm()[choice_ecm]->getAllTechs()[i] == sce::Tech::RGPO)
			{
				combox->setCurrentIndex(3);
			}
		}
		this->show();
	}
	else {
		QPushButton *add = new QPushButton();
		QPushButton *del = new QPushButton();
		QWidget *widget = new QWidget(this);
		QVBoxLayout *layout = new QVBoxLayout();
		QPushButton *save = new QPushButton();
		add->setText("Add");
		del->setText("Del");
		save->setText("save");
		connect(add, SIGNAL(clicked()), this, SLOT(add_tech()));
		connect(del, SIGNAL(clicked()), this, SLOT(del_tech()));
		connect(save, SIGNAL(clicked()), this, SLOT(save_ecm_tech()));
		layout->addWidget(add);
		layout->addWidget(del);
		layout->addWidget(save);
		layout->addWidget(ui.tableWidget);
		widget->setLayout(layout);
		this->setCentralWidget(widget);
		ui.tableWidget->setColumnCount(0);
		ui.tableWidget->insertColumn(0);
		ui.tableWidget->setCellWidget(0, 0, new QComboBox());
		auto cellWidget = ui.tableWidget->cellWidget(0, 0);
		QComboBox *combox = qobject_cast<QComboBox*>(cellWidget);
		combox->addItems(tech_list);
		this->show();
	}
}
