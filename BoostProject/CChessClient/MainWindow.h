#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	bool init();
	bool connectSigSlot();

signals:
	void startConnect();

public slots:
	void on_startButton_clicked();
	void on_sendButton_clicked();
	void onMessageReveived(const QString&);

private:
    Ui::MainWindow *ui;
	int count;
};

#endif // MAINWINDOW_H
