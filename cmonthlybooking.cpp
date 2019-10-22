#include "cmonthlybooking.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


cMonthlyBooking::cMonthlyBooking(const QDate& date, QObject *parent) :
	QObject(parent),
	m_date(date),
	m_ueberstunden(QTime(0, 0, 0))
{
}

QDate cMonthlyBooking::date()
{
	return(m_date);
}

void cMonthlyBooking::setUeberstunden(const QTime &time)
{
	m_ueberstunden	= time;
}

QTime cMonthlyBooking::ueberstunden()
{
	return(m_ueberstunden);
}

bool cMonthlyBooking::save()
{
	QSqlQuery	query;

	query.prepare("SELECT datum FROM monthlyBooking WHERE datum=:datum;");
	query.bindValue(":datum", m_date);

	if(!query.exec())
	{
		qDebug() << "Save: " << query.lastError().text();
		return(false);
	}

	if(query.next())
		query.prepare("UPDATE monthlyBooking "
					  "SET    ueberstunden=:ueberstunden "
					  "WHERE  datum=:datum;");
	else
		query.prepare("INSERT INTO monthlyBooking ( "
					  "                     datum, "
					  "                     ueberstunden "
					  "                    ) "
					  "VALUES "
					  "                    ( "
					  "                     :datum, "
					  "                     :ueberstunden "
					  "                    );");

	query.bindValue(":datum", date());
	query.bindValue(":ueberstunden", ueberstunden());

	if(!query.exec())
	{
		qDebug() << "Save: " << query.lastError().text();
		return(false);
	}

	qDebug() << "saved.";
	return(true);
}

bool cMonthlyBookingList::load()
{
	QSqlQuery	query;

	query.prepare("SELECT	datum, "
				  "         ueberstunden "
				  "FROM     monthlyBooking "
				  "ORDER BY datum;");

	if(!query.exec())
	{
		qDebug() << "SELECT FROM monthlyBooking: " << query.lastError().text();
		return(false);
	}

	while(query.next())
	{
		cMonthlyBooking*	lpBooking	= add(query.value("datum").toDate());

		if(lpBooking)
		{
			lpBooking->setUeberstunden(query.value("ueberstunden").toTime());
		}
	}

	return(true);
}

cMonthlyBooking* cMonthlyBookingList::add(const QDate& date)
{
	if(find(date))
		return(nullptr);

	cMonthlyBooking*	lpVacation	= new cMonthlyBooking(date);
	append(lpVacation);
	return(lpVacation);
}

cMonthlyBooking* cMonthlyBookingList::find(const QDate& date)
{
	for(cMonthlyBookingList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() == date)
			return(*i);
	}
	return(nullptr);
}

cMonthlyBooking* cMonthlyBookingList::get(const QDate& date)
{
	for(cMonthlyBookingList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() <= date)
			return(*i);
	}
	return(nullptr);
}
