#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ControlerManager.h"
#include "NetworkManager.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	init();
	connectSigSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::init()
{
	count = 0;
	return true;
}
bool MainWindow::connectSigSlot()
{
	connect(this, SIGNAL(startConnect()), atpControlers->networkManager(), SLOT(onStartConnect()));
	connect(atpControlers->networkManager(), SIGNAL(messageReceived(const QString&)), this, SLOT(onMessageReveived(const QString&)));
	return true;
}

void MainWindow::on_startButton_clicked()
{
	emit startConnect();
}

void MainWindow::on_sendButton_clicked()
{
	count++;
	QString msg = "abcdef " + QString::number(count);
	atpControlers->networkManager()->sendMsg(msg);
}

void MainWindow::onMessageReveived(const QString& res)
{
	ui->replyLabel->setText(res);
}