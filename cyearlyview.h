#ifndef CYEARLYVIEW_H
#define CYEARLYVIEW_H


#include "cmonthlybooking.h"
#include "cmonthlyitemdelegate.h"
#include "cpublicholiday.h"

#include "cbooking.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QMap>
#include <QTime>

#include <QRegExpValidator>


namespace Ui {
class cYearlyView;
}

class cYearlyView : public QWidget
{
	Q_OBJECT

public:
	explicit cYearlyView(const QDate& date, const QMap<QString, QString> code, cMonthlyBookingList* lpMonthlyBookingList, cPublicHoliday* lpPublicHoliday, cBookingList* lpBookingList, QWidget *parent = nullptr);
	~cYearlyView();

	void					setDate(const QDate& date);

private:
	Ui::cYearlyView*		ui;
	bool					m_loading;
	QStandardItemModel*		m_lpYearlyListModel;
	QMap<QString, QString>	m_code;
	cMonthlyBookingList*	m_lpMonthlyBookingList;
	cPublicHoliday*			m_lpPublicHoliday;
	cBookingList*			m_lpBookingList;
	QDate					m_date;
	cMonthlyBooking*		m_lpMonthlyBooking;
//	QRegExpValidator*		m_lpValidator;
//	QStringList				m_temporaryFileList;

public slots:
	void					onPrevYear();
	void					onNextYear();
	void					onDateChanged(const QDate& date);

};

#endif // CYEARLYVIEW_H
