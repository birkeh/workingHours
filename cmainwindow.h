#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "cyearlyview.h"
#include "cmonthlyview.h"
#include "cdailyworking.h"
#include "cvacation.h"
#include "cpublicholiday.h"
#include "cmonthlybooking.h"
#include "cbooking.h"

#include <QMainWindow>
#include <QCloseEvent>

#include <QSqlDatabase>

#include <QMap>


namespace Ui {
class cMainWindow;
}

class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit cMainWindow(QWidget *parent = nullptr);
	~cMainWindow();

private:
	Ui::cMainWindow*		ui;
	cYearlyView*			m_lpYearyView;
	cMonthlyView*			m_lpMonthlyView;
	QSqlDatabase			m_db;
	cPublicHoliday			m_publicHoliday;
	cDailyWorkingList		m_dailyWorkingList;
	cVacationList			m_vacationList;
	cMonthlyBookingList		m_monthlyBookingList;
	cBookingList*			m_lpBookingList;
	QMap<QString, QString>	m_code;


	void					initUI();
	void					openDB();

protected:
	void					closeEvent(QCloseEvent* event);
};

#endif // CMAINWINDOW_H
