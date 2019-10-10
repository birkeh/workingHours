#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QDate>
#include <QDir>

#include <QSettings>

#include <QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QDebug>


cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_lpMonthlyView(nullptr)
{
	openDB();
	m_bookingList.load();

	initUI();

	qDebug() << "count: " << m_bookingList.count();
}

cMainWindow::~cMainWindow()
{
	if(m_db.isOpen())
		m_db.close();

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

void cMainWindow::initUI()
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

	m_lpMonthlyView	= new cMonthlyView(QDate::currentDate(), &m_bookingList, this);
	ui->m_lpMainTab->addTab(m_lpMonthlyView, "Monthly");
}

void cMainWindow::openDB()
{
	QSettings	settings;

	if(!settings.contains("db/file"))
		settings.setValue("db/file", QDir::homePath() + "/workingHours.db");

	QString		szDB	= settings.value("db/file", QDir::homePath() + "/workingHours.db").toString();
	qDebug() << "Database: " << szDB;

	m_db	= QSqlDatabase::addDatabase("QSQLITE");
	m_db.setHostName("localhost");
	m_db.setDatabaseName(szDB);

	if(!m_db.open())
	{
		QMessageBox::critical(this, tr("Database Open"), "Database cannot be opened!\n" + m_db.lastError().text());
		qDebug() << "Database Open error: " << m_db.lastError().text();
		return;
	}

	qDebug() << "Database opened";

//	if(!m_db.tables().contains("config"))
//	{
//		QSqlQuery	query;

//		query.prepare("CREATE TABLE config "
//					  "             ")
//	}

	if(!m_db.tables().contains("booking"))
	{
		QSqlQuery	query;

		query.prepare("CREATE TABLE booking "
					  "( "
					  "     datum       DATE PRIMARY KEY UNIQE, "
					  "     kommt1      TIME, "
					  "     geht1       TIME, "
					  "     kommt2      TIME, "
					  "     geht2       TIME, "
					  "     kommt3      TIME, "
					  "     geht3       TIME, "
					  "     kommt4      TIME, "
					  "     geht4       TIME, "
					  "     kommt5      TIME, "
					  "     geht5       TIME, "
					  "     code        INTEGER, "
					  "     information STRING "
					  ");");

		if(!query.exec())
		{
			qDebug() << "CREATE TABLE booking: " << query.lastError().text();
			return;
		}
		qDebug() << "CREATE TABLE booking";
	}
}
