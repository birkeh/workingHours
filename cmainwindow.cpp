#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QDate>

#include <QSettings>


cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_lpMonthlyView(nullptr)
{
	ui->setupUi(this);

	QSettings	settings;
	qint32		iX			= settings.value("main/x", QVariant::fromValue(-1)).toInt();
	qint32		iY			= settings.value("main/y", QVariant::fromValue(-1)).toInt();
	qint32		iWidth		= settings.value("main/width", QVariant::fromValue(-1)).toInt();
	qint32		iHeight		= settings.value("main/height", QVariant::fromValue(-1)).toInt();

	if(iWidth != -1 && iHeight != -1)
		resize(iWidth, iHeight);
	if(iX != -1 && iY != -1)
		move(iX, iY);

	m_lpMonthlyView	= new cMonthlyView(QDate(2019, 10, 8), this);
	ui->m_lpMainTab->addTab(m_lpMonthlyView, "Monthly");
}

cMainWindow::~cMainWindow()
{
	if(m_lpMonthlyView)
		delete m_lpMonthlyView;

	delete ui;
}

void cMainWindow::closeEvent(QCloseEvent *event)
{
	QSettings	settings;
	settings.setValue("main/width", QVariant::fromValue(size().width()));
	settings.setValue("main/height", QVariant::fromValue(size().height()));
	settings.setValue("main/x", QVariant::fromValue(x()));
	settings.setValue("main/y", QVariant::fromValue(y()));
	if(this->isMaximized())
		settings.setValue("main/maximized", QVariant::fromValue(true));
	else
		settings.setValue("main/maximized", QVariant::fromValue(false));

	event->accept();
}
