#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "czeitnachweis.h"

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
	m_lpYearyView(nullptr),
	m_lpMonthlyView(nullptr),
	m_lpBookingList(nullptr)
{
	m_code.insert(" ", tr(""));
	m_code.insert("G", tr("Gleitzeit"));
	m_code.insert("U", tr("Urlaub"));
	m_code.insert("SU", tr("Sonderurlaub"));
	m_code.insert("K", tr("Krank"));
	m_code.insert("T", tr("Training"));

	openDB();
	m_dailyWorkingList.load();
	m_vacationList.load();
	m_monthlyBookingList.load();

	m_lpBookingList	= new cBookingList(&m_monthlyBookingList, &m_publicHoliday, &m_dailyWorkingList, &m_vacationList);
	m_lpBookingList->load();

	initUI();

//	cZeitnachweis	zeitnachweis1("C:\\Users\\VET0572\\Downloads\\Zeitnachweis\\2019-09.pdf");
//	cZeitnachweis	zeitnachweis2("C:\\Users\\VET0572\\Downloads\\Zeitnachweis\\2019-08.pdf");
//	cZeitnachweis	zeitnachweis3("C:\\Users\\VET0572\\Downloads\\Zeitnachweis\\2018-07.pdf");
//	cZeitnachweis	zeitnachweis4("C:\\Users\\VET0572\\Downloads\\Zeitnachweis\\2017-06.pdf");
//	cZeitnachweis	zeitnachweis5("C:\\Users\\VET0572\\Downloads\\Zeitnachweis\\2016-05.pdf");

	qDebug() << "count: " << m_lpBookingList->count();

	connect(m_lpMonthlyView,	&cMonthlyView::dateChanged,	m_lpYearyView,	&cYearlyView::onDateChanged);
}

cMainWindow::~cMainWindow()
{
	if(m_lpBookingList)
		delete m_lpBookingList;

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

	m_lpYearyView	= new cYearlyView(QDate::currentDate(), m_code, &m_monthlyBookingList, &m_publicHoliday, m_lpBookingList, this);
	ui->m_lpMainTab->addTab(m_lpYearyView, "Yearly");

	m_lpMonthlyView	= new cMonthlyView(QDate::currentDate(), m_code, &m_monthlyBookingList, &m_publicHoliday, m_lpBookingList, this);
	ui->m_lpMainTab->addTab(m_lpMonthlyView, "Monthly");

	ui->m_lpMainTab->setCurrentWidget(m_lpMonthlyView);
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
	QSqlQuery	query;

	if(!m_db.tables().contains("dailyWorking"))
	{
		query.prepare("CREATE TABLE dailyWorking "
					  "( "
					  "     ab          DATE PRIMARY KEY UNIQUE, "
					  "     montag      TIME, "
					  "     dienstag    TIME, "
					  "     mittwoch    TIME, "
					  "     donnerstag  TIME, "
					  "     freitag     TIME, "
					  "     samstag     TIME, "
					  "     sonntag     TIME "
					  ");");
		if(!query.exec())
		{
			qDebug() << "CREATE TABLE dailyWorking: " << query.lastError().text();
			return;
		}
		qDebug() << "CREATE TABLE dailyWorking";
	}

	if(!m_db.tables().contains("booking"))
	{
		query.prepare("CREATE TABLE booking "
					  "( "
					  "     datum       DATE PRIMARY KEY UNIQUE, "
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

	if(!m_db.tables().contains("monthlyBooking"))
	{
		query.prepare("CREATE TABLE monthlyBooking "
					  "( "
					  "     datum        DATE PRIMARY KEY UNIQUE, "
					  "     correction   INTEGER, "
					  "     ueberstunden INTEGER, "
					  "     salery       BLOB, "
					  "     timesheet    BLOB "
					  ");");

		if(!query.exec())
		{
			qDebug() << "CREATE TABLE monthlyBooking: " << query.lastError().text();
			return;
		}
		qDebug() << "CREATE TABLE monthlyBooking";
	}

	if(!m_db.tables().contains("vacation"))
	{
		query.prepare("CREATE TABLE vacation "
					  "( "
					  "     ab          DATE PRIMARY KEY UNIQUE, "
					  "     days        INTEGER "
					  ");");

		if(!query.exec())
		{
			qDebug() << "CREATE TABLE vacation: " << query.lastError().text();
			return;
		}
		qDebug() << "CREATE TABLE vacation";
	}
}
