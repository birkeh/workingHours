#include "cbooking.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>


cBooking::cBooking(const QDate& date, QObject* parent) :
	QObject(parent),
	m_date(date),
	m_kommt1(QTime()),
	m_geht1(QTime()),
	m_kommt2(QTime()),
	m_geht2(QTime()),
	m_kommt3(QTime()),
	m_geht3(QTime()),
	m_kommt4(QTime()),
	m_geht4(QTime()),
	m_kommt5(QTime()),
	m_geht5(QTime()),
	m_code(""),
	m_information(""),
	m_soll(QTime()),
	m_prevDiff(0)

{
}

QDate cBooking::date()
{
	return(m_date);
}

void cBooking::setKommt1(const QTime& time)
{
	m_kommt1	= time;
}

QTime cBooking::kommt1()
{
	return(m_kommt1);
}

void cBooking::setGeht1(const QTime& time)
{
	m_geht1		= time;
}

QTime cBooking::geht1()
{
	return(m_geht1);
}

void cBooking::setKommt2(const QTime& time)
{
	m_kommt2	= time;
}

QTime cBooking::kommt2()
{
	return(m_kommt2);
}

void cBooking::setGeht2(const QTime& time)
{
	m_geht2		= time;
}

QTime cBooking::geht2()
{
	return(m_geht2);
}

void cBooking::setKommt3(const QTime& time)
{
	m_kommt3	= time;
}

QTime cBooking::kommt3()
{
	return(m_kommt3);
}

void cBooking::setGeht3(const QTime& time)
{
	m_geht3		= time;
}

QTime cBooking::geht3()
{
	return(m_geht3);
}

void cBooking::setKommt4(const QTime& time)
{
	m_kommt4	= time;
}

QTime cBooking::kommt4()
{
	return(m_kommt4);
}

void cBooking::setGeht4(const QTime& time)
{
	m_geht4		= time;
}

QTime cBooking::geht4()
{
	return(m_geht4);
}

void cBooking::setKommt5(const QTime& time)
{
	m_kommt5	= time;
}

QTime cBooking::kommt5()
{
	return(m_kommt5);
}

void cBooking::setGeht5(const QTime& time)
{
	m_geht5		= time;
}

QTime cBooking::geht5()
{
	return(m_geht5);
}

void cBooking::setCode(const QString& code)
{
	m_code		= code;
}

QString cBooking::code()
{
	return(m_code);
}

void cBooking::setInformation(const QString& information)
{
	m_information	= information;
}

QString cBooking::information()
{
	return(m_information);
}

qint32 cBooking::pauseSecs()
{
	int	w	= totalWork();
	int	p	= totalPause();

	if(w < 21600)
		return(0);

	if(p < 1800)
		return(1800-p);

	return(0);
}

QTime cBooking::pause()
{
	return(QTime(0, 0, 0).addSecs(pauseSecs()));
}

qint32 cBooking::istSecs()
{
	int	w	= totalWork();
	int	p	= totalPause();

	if(w < 21600)
		return(w);

	if(p < 1800)
		return(w - (1800-p));

	return(w);
}

QTime cBooking::ist()
{
	return(QTime(0, 0, 0).addSecs(istSecs()));
}

void cBooking::setSoll(const QTime& time)
{
	m_soll	= time;
}

QTime cBooking::soll()
{
	return(m_soll);
}

qint32 cBooking::diff()
{
	return(0);
}

QString cBooking::diffString()
{
	QTime	t;
	qint32	d		= diff();
	bool	bNeg	= false;

	if(d < 0)
	{
		t	= QTime(0, 0, 0, d*1000);
		bNeg	= true;
	}
	else
		t	= QTime(0, 0, 0, d*1000);

	return(bNeg ? "-" : "" + t.toString("hh:mm:ss"));
}

void cBooking::setPrevDiff(qint32 diff)
{
	m_prevDiff	= diff;
}

qint32 cBooking::prevDiff()
{
	return(m_prevDiff);
}

qint32 cBooking::currentDiff()
{
	return(0);
}

QString cBooking::currentDiffString()
{
	QTime	t;
	qint32	d		= currentDiff();
	bool	bNeg	= false;

	if(d < 0)
	{
		t	= QTime(0, 0, 0, d*1000);
		bNeg	= true;
	}
	else
		t	= QTime(0, 0, 0, d*1000);

	return(bNeg ? "-" : "" + t.toString("hh:mm:ss"));
}

qreal cBooking::hoursDecimal()
{
	int	w	= istSecs();

	return(static_cast<qreal>(w)/3600);
}

bool cBooking::save()
{
	QSqlQuery	query;

	query.prepare("SELECT datum FROM booking WHERE datum=:datum;");
	query.bindValue(":datum", m_date);

	if(!query.exec())
	{
		qDebug() << "Save: " << query.lastError().text();
		return(false);
	}

	if(query.next())
		query.prepare("UPDATE booking "
					  "SET    kommt1=:kommt1, "
					  "       geht1=:geht1, "
					  "       kommt2=:kommt2, "
					  "       geht2=:geht2, "
					  "       kommt3=:kommt3, "
					  "       geht3=:geht3, "
					  "       kommt4=:kommt4, "
					  "       geht4=:geht4, "
					  "       kommt5=:kommt5, "
					  "       geht5=:geht5, "
					  "       code=:code, "
					  "       information=:information "
					  "WHERE  datum=:datum;");
	else
		query.prepare("INSERT INTO booking ( "
					  "                     datum, "
					  "                     kommt1, "
					  "                     geht1, "
					  "                     kommt2, "
					  "                     geht2, "
					  "                     kommt3, "
					  "                     geht3, "
					  "                     kommt4, "
					  "                     geht4, "
					  "                     kommt5, "
					  "                     geht5, "
					  "                     code, "
					  "                     information"
					  "                    ) "
					  "VALUES "
					  "                    ( "
					  "                     :datum, "
					  "                     :kommt1, "
					  "                     :geht1, "
					  "                     :kommt2, "
					  "                     :geht2, "
					  "                     :kommt3, "
					  "                     :geht3, "
					  "                     :kommt4, "
					  "                     :geht4, "
					  "                     :kommt5, "
					  "                     :geht5, "
					  "                     :code, "
					  "                     :information"
					  "                    );");

	query.bindValue(":datum", date());
	bind(query, ":kommt1", kommt1());
	bind(query, ":geht1", geht1());
	bind(query, ":kommt2", kommt2());
	bind(query, ":geht2", geht2());
	bind(query, ":kommt3", kommt3());
	bind(query, ":geht3", geht3());
	bind(query, ":kommt4", kommt4());
	bind(query, ":geht4", geht4());
	bind(query, ":kommt5", kommt5());
	bind(query, ":geht5", geht5());
	query.bindValue(":code", code());
	query.bindValue(":information", information());

	if(!query.exec())
	{
		qDebug() << "Save: " << query.lastError().text();
		return(false);
	}

	qDebug() << "saved.";
	return(true);
}

void cBooking::bind(QSqlQuery& query, const QString& variable, const QTime& time)
{
	if(time == QTime(0, 0, 0))
		query.bindValue(variable, QVariant(QVariant::Time));
	else
		query.bindValue(variable, time);
}

int cBooking::totalWork()
{
	return(kommt1().secsTo(geht1()) + kommt2().secsTo(geht2()) + kommt3().secsTo(geht3()) + kommt4().secsTo(geht4()) + kommt5().secsTo(geht5()));
}

int cBooking::totalPause()
{
	return(geht1().secsTo(kommt2()) + geht2().secsTo(kommt3()) + geht3().secsTo(kommt4()) + geht4().secsTo(kommt5()));
}

bool cBookingList::load(cDailyWorkingList* lpDailyWorkingList)
{
	m_lpDailyWorkingList	= lpDailyWorkingList;
	QSqlQuery	query;

	query.prepare("SELECT   datum, "
				  "         kommt1, "
				  "         geht1, "
				  "         kommt2, "
				  "         geht2, "
				  "         kommt3, "
				  "         geht3, "
				  "         kommt4, "
				  "         geht4, "
				  "         kommt5, "
				  "         geht5, "
				  "         code, "
				  "         information "
				  "FROM     booking "
				  "ORDER BY datum;");

	if(!query.exec())
	{
		qDebug() << "SELECT FROM booking: " << query.lastError().text();
		return(false);
	}

	while(query.next())
	{
		cBooking*	lpBooking	= add(query.value("datum").toDate());

		if(lpBooking)
		{
			lpBooking->setKommt1(query.value("kommt1").toTime());
			lpBooking->setGeht1(query.value("geht1").toTime());
			lpBooking->setKommt2(query.value("kommt2").toTime());
			lpBooking->setGeht2(query.value("geht2").toTime());
			lpBooking->setKommt3(query.value("kommt3").toTime());
			lpBooking->setGeht3(query.value("geht3").toTime());
			lpBooking->setKommt4(query.value("kommt4").toTime());
			lpBooking->setGeht4(query.value("geht4").toTime());
			lpBooking->setKommt5(query.value("kommt5").toTime());
			lpBooking->setGeht5(query.value("geht5").toTime());
			lpBooking->setCode(query.value("code").toString());
			lpBooking->setInformation(query.value("information").toString());

			cDailyWorking*	lpDailyWorking	= m_lpDailyWorkingList->get(lpBooking->date());
			if(lpDailyWorking)
				lpBooking->setSoll(lpDailyWorking->soll(lpBooking->date().dayOfWeek()));
		}
	}

	return(true);
}

cBooking* cBookingList::add(const QDate& date)
{
	if(find(date))
		return(nullptr);

	cBooking*	lpBooking	= new cBooking(date);
	append(lpBooking);
	return(lpBooking);
}

cBooking* cBookingList::find(const QDate& date)
{
	for(cBookingList::iterator i = begin();i != end(); i++)
	{
		if((*i)->date() == date)
			return(*i);
	}
	return(nullptr);
}
