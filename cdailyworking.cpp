#include "cdailyworking.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>


cDailyWorking::cDailyWorking(const QDate& date, QObject *parent) :
	QObject(parent),
	m_date(date),
	m_montag(QTime(7, 42, 0)),
	m_dienstag(QTime(7, 42, 0)),
	m_mittwoch(QTime(7, 42, 0)),
	m_donnerstag(QTime(7, 42, 0)),
	m_freitag(QTime(7, 42, 0)),
	m_samstag(QTime(0, 0, 0)),
	m_sonntag(QTime(0, 0, 0))
{
}

QDate cDailyWorking::date()
{
	return(m_date);
}

void cDailyWorking::setMontag(const QTime& time)
{
	m_montag	= time;
}

QTime cDailyWorking::montag()
{
	return(m_montag);
}

void cDailyWorking::setDienstag(const QTime& time)
{
	m_dienstag	= time;
}

QTime cDailyWorking::dienstag()
{
	return(m_dienstag);
}

void cDailyWorking::setMittwoch(const QTime& time)
{
	m_mittwoch	= time;
}

QTime cDailyWorking::mittwoch()
{
	return(m_mittwoch);
}

void cDailyWorking::setDonnerstag(const QTime& time)
{
	m_donnerstag	= time;
}

QTime cDailyWorking::donnerstag()
{
	return(m_donnerstag);
}

void cDailyWorking::setFreitag(const QTime& time)
{
	m_freitag	= time;
}

QTime cDailyWorking::freitag()
{
	return(m_freitag);
}

void cDailyWorking::setSamstag(const QTime& time)
{
	m_samstag	= time;
}

QTime cDailyWorking::samstag()
{
	return(m_samstag);
}

void cDailyWorking::setSonntag(const QTime& time)
{
	m_sonntag	= time;
}

QTime cDailyWorking::sonntag()
{
	return(m_sonntag);
}

QTime cDailyWorking::soll(qint16 day)
{
	switch(day)
	{
	case 1:
		return(montag());
	case 2:
		return(dienstag());
	case 3:
		return(mittwoch());
	case 4:
		return(donnerstag());
	case 5:
		return(freitag());
	case 6:
		return(samstag());
	case 7:
		return(sonntag());
	default:
		return(QTime(0, 0, 0));
	}
}

bool cDailyWorkingList::load()
{
	QSqlQuery	query;

	query.prepare("SELECT	datum, "
				  "         montag,"
				  "         dienstag, "
				  "         mittwoch, "
				  "         donnerstag, "
				  "         freitag, "
				  "         samstag, "
				  "         sonntag "
				  "FROM     dailyWorking "
				  "ORDER BY datum;");

	if(!query.exec())
	{
		qDebug() << "SELECT FROM dailyWorking: " << query.lastError().text();
		return(false);
	}

	while(query.next())
	{
		cDailyWorking*	lpWorking	= add(query.value("datum").toDate());

		if(lpWorking)
		{
			lpWorking->setMontag(query.value("montag").toTime());
			lpWorking->setDienstag(query.value("dienstag").toTime());
			lpWorking->setMittwoch(query.value("mittwoch").toTime());
			lpWorking->setDonnerstag(query.value("donnerstag").toTime());
			lpWorking->setFreitag(query.value("freitag").toTime());
			lpWorking->setSamstag(query.value("samstag").toTime());
			lpWorking->setSonntag(query.value("sonntag").toTime());
		}
	}

	return(true);
}

cDailyWorking* cDailyWorkingList::add(const QDate& date)
{
	if(find(date))
		return(nullptr);

	cDailyWorking*	lpWorking	= new cDailyWorking(date);
	append(lpWorking);
	return(lpWorking);
}

cDailyWorking* cDailyWorkingList::find(const QDate& date)
{
	for(cDailyWorkingList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() == date)
			return(*i);
	}
	return(nullptr);
}

cDailyWorking* cDailyWorkingList::get(const QDate& date)
{
	for(cDailyWorkingList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() <= date)
			return(*i);
	}
	return(nullptr);
}
