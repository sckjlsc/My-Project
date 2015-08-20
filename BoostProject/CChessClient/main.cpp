#include "MainWindow.h"
#include <QApplication>
#include <boost/asio.hpp>
#include "ControlerManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	ControlerManager controlManager(argc, argv);
	controlManager.init();
	controlManager.connectSigSlot();
	
	MainWindow w;
    w.show();

	controlManager.runProgram();
    return a.exec();
}
