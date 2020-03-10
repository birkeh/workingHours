#ifndef CMONTHLYVIEW_H
#define CMONTHLYVIEW_H


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
class cMonthlyView;
}

class cMonthlyView : public QWidget
{
	Q_OBJECT

public:
	explicit cMonthlyView(const QDate& date, const QMap<QString, QString> code, cMonthlyBookingList* lpMonthlyBookingList, cPublicHoliday* lpPublicHoliday, cBookingList* lpBookingList, QWidget *parent = nullptr);
	~cMonthlyView();

	void					setDate(const QDate& date);
private:
	Ui::cMonthlyView*		ui;
	bool					m_loading;
	QStandardItemModel*		m_lpMonthlyListModel;
	QMap<QString, QString>	m_code;
	cMonthlyBookingList*	m_lpMonthlyBookingList;
	cPublicHoliday*			m_lpPublicHoliday;
	cBookingList*			m_lpBookingList;
	QDate					m_date;
	cMonthlyBooking*		m_lpMonthlyBooking;
	QRegExpValidator*		m_lpValidator;
	QStringList				m_temporaryFileList;

	void					setBackground(const int day, const QString& code);
	void					setText(QStandardItem* lpItem, const QTime& time);

	void					recalculate(int day, int field);
	void					displaySummary();
	void					updatePDF();

private slots:
	void					onPrevMonth();
	void					onNextMonth();
	void					onTimeChanged(const int day, const int field, const QTime& time);
	void					onTextChanged(const int day, const int field, const QString& text);
	void					onDateChanged(const QDate& date);
	void					onUeberstundenChanged(const QString& string);
	void					onTimesheetView();
	void					onTimesheetDownload();
	void					onTimesheetReplace();
	void					onTimesheetDelete();
	void					onSaleryView();
	void					onSaleryDownload();
	void					onSaleryReplace();
	void					onSaleryDelete();

signals:
	void					dateChanged(const QDate& date);
};

#endif // CMONTHLYVIEW_H
