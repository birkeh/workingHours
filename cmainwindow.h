#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "cmonthlyview.h"
#include "cdailyworking.h"
#include "cvacation.h"
#include "cpublicholiday.h"
#include "cbooking.h"

#include <QMainWindow>
#include <QCloseEvent>

#include <QSqlDatabase>


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
	Ui::cMainWindow*	ui;
	cMonthlyView*		m_lpMonthlyView;
	QSqlDatabase		m_db;
	cPublicHoliday		m_publicHoliday;
	cDailyWorkingList	m_dailyWorkingList;
	cVacationList		m_vacationList;
	cBookingList*		m_lpBookingList;


	void				initUI();
	void				openDB();

protected:
	void				closeEvent(QCloseEvent* event);
};

#endif // CMAINWINDOW_H
