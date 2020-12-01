#include "cmonthlybooking.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


cMonthlyBooking::cMonthlyBooking(const QDate& date, QObject *parent) :
	QObject(parent),
	m_date(date),
	m_correction(0),
	m_ueberstunden(0)
{
}

QDate cMonthlyBooking::date()
{
	return(m_date);
}

void cMonthlyBooking::setCorrection(qint32 correction)
{
	m_correction	= correction;
}

qint32 cMonthlyBooking::correction()
{
	return(m_correction);
}

void cMonthlyBooking::setUeberstunden(qint32 ueberstunden)
{
	m_ueberstunden	= ueberstunden;
}

qint32 cMonthlyBooking::ueberstunden()
{
	return(m_ueberstunden);
}

void cMonthlyBooking::setSalery(const QByteArray& salery)
{
	m_salery	= salery;
}

QByteArray cMonthlyBooking::salery()
{
	return(m_salery);
}

void cMonthlyBooking::setTimesheet(const QByteArray& timesheet)
{
	m_timesheet	= timesheet;
}

QByteArray cMonthlyBooking::timesheet()
{
	return(m_timesheet);
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
					  "SET    correction=:correction,"
					  "       ueberstunden=:ueberstunden, "
					  "       salery=:salery, "
					  "       timesheet=:timesheet "
					  "WHERE  datum=:datum;");
	else
		query.prepare("INSERT INTO monthlyBooking ( "
					  "                     datum, "
					  "                     correction, "
					  "                     ueberstunden, "
					  "                     salery, "
					  "                     timesheet "
					  "                    ) "
					  "VALUES "
					  "                    ( "
					  "                     :datum, "
					  "                     :correction, "
					  "                     :ueberstunden, "
					  "                     :salery, "
					  "                     :timesheet "
					  "                    );");

	query.bindValue(":datum", date());
	query.bindValue(":correction", correction());
	query.bindValue(":ueberstunden", ueberstunden());
	query.bindValue(":salery", salery());
	query.bindValue(":timesheet", timesheet());

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
				  "         correction, "
				  "         ueberstunden, "
				  "         salery, "
				  "         timesheet "
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
			lpBooking->setCorrection(query.value("correction").toInt());
			lpBooking->setUeberstunden(query.value("ueberstunden").toInt());
			lpBooking->setSalery(query.value("salery").toByteArray());
			lpBooking->setTimesheet(query.value("timesheet").toByteArray());
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
