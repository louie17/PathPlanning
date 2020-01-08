#include "stdafx.h"
#include "MainWindow.h"
#include <QtGlobal>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qInstallMessageHandler(outputMessage); //×¢²áMsgHandler»Øµ÷º¯Êý

	PathPlanGui w;
	w.show();
	return a.exec();
}
