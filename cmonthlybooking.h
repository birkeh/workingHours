#ifndef CMONTHLYBOOKING_H
#define CMONTHLYBOOKING_H


#include <QMetaType>
#include <QList>
#include <QObject>
#include <QDate>
#include <QTime>
#include <QByteArray>


class cMonthlyBooking : public QObject
{
	Q_OBJECT

public:
	explicit			cMonthlyBooking(const QDate& date, QObject *parent = nullptr);

	QDate				date();

	void				setUeberstunden(qint32 ueberstunden);
	qint32				ueberstunden();

	void				setSalery(const QByteArray& salery);
	QByteArray			salery();

	void				setTimesheet(const QByteArray& timesheet);
	QByteArray			timesheet();

	bool				save();

private:
	QDate				m_date;
	qint32				m_ueberstunden;
	QByteArray			m_salery;
	QByteArray			m_timesheet;

signals:

public slots:
};

Q_DECLARE_METATYPE(cMonthlyBooking*)

class cMonthlyBookingList : public QList<cMonthlyBooking*>
{
public:
	bool				load();
	cMonthlyBooking*	add(const QDate& date);
	cMonthlyBooking*	find(const QDate& date);
	cMonthlyBooking*	get(const QDate& date);
};

#endif // CMONTHLYBOOKING_H
