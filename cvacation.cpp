#include "cvacation.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


cVacation::cVacation(const QDate& date, QObject *parent) :
	QObject(parent),
	m_date(date),
	m_days(0)
{
}

QDate cVacation::date()
{
	return(m_date);
}

void cVacation::setDays(qint16 days)
{
	m_days	= days;
}

qint16 cVacation::days()
{
	return(m_days);
}

bool cVacationList::load()
{
	QSqlQuery	query;

	query.prepare("SELECT	ab, "
				  "         days "
				  "FROM     vacation "
				  "ORDER BY ab;");

	if(!query.exec())
	{
		qDebug() << "SELECT FROM vacation: " << query.lastError().text();
		return(false);
	}

	while(query.next())
	{
		cVacation*	lpVacation	= add(query.value("ab").toDate());

		if(lpVacation)
		{
			lpVacation->setDays(static_cast<qint16>(query.value("days").toInt()));
		}
	}

	return(true);
}

cVacation* cVacationList::add(const QDate& date)
{
	if(find(date))
		return(nullptr);

	cVacation*	lpVacation	= new cVacation(date);
	append(lpVacation);
	return(lpVacation);
}

cVacation* cVacationList::find(const QDate& date)
{
	for(cVacationList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() == date)
			return(*i);
	}
	return(nullptr);
}

cVacation* cVacationList::get(const QDate& date)
{
	for(cVacationList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() <= date)
			return(*i);
	}
	return(nullptr);
}
