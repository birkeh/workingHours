#ifndef CMONTHLYBOOKING_H
#define CMONTHLYBOOKING_H


#include <QMetaType>
#include <QList>
#include <QObject>
#include <QDate>
#include <QTime>


class cMonthlyBooking : public QObject
{
	Q_OBJECT

public:
	explicit			cMonthlyBooking(const QDate& date, QObject *parent = nullptr);

	QDate				date();

	void				setUeberstunden(const QTime& time);
	QTime				ueberstunden();

	bool				save();

private:
	QDate				m_date;
	QTime				m_ueberstunden;

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
