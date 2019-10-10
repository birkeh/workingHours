#include "cpublicholiday.h"

#include <QObject>


cPublicHoliday::cPublicHoliday(qint16 year) :
	m_year(year)
{
	calculate();
}

void cPublicHoliday::setYear(qint16 year)
{
	m_year	= year;
	calculate();
}

qint16 cPublicHoliday::year()
{
	return(m_year);
}

void cPublicHoliday::calculate()
{
	m_holidays.clear();

	m_holidays.insert(neujahr(),				QObject::tr("Neujahr"));
	m_holidays.insert(heilige3koenige(),		QObject::tr("Heilige 3 Könige"));
	m_holidays.insert(karfreitag(),				QObject::tr("Karfreitag"));
	m_holidays.insert(ostersonntag(),			QObject::tr("Ostersonntag"));
	m_holidays.insert(ostermontag(),			QObject::tr("OsterMontag"));
	m_holidays.insert(staatsfeiertag1(),		QObject::tr("Staatsfeiertag"));
	m_holidays.insert(christihimmelfahrt(),		QObject::tr("Christi Himmelfahrt"));
	m_holidays.insert(pfingstsonntag(),			QObject::tr("Pfingstsonntag"));
	m_holidays.insert(pfingstmontag(),			QObject::tr("Pfingstmontag"));
	m_holidays.insert(fronleichnam(),			QObject::tr("Fronleichnam"));
	m_holidays.insert(mariahimmelfahrt(),		QObject::tr("Maria Himmelfahrt"));
	m_holidays.insert(staatsfeiertag(),			QObject::tr("Staatsfeiertag"));
	m_holidays.insert(allerheiligen(),			QObject::tr("Allerheiligen"));
	m_holidays.insert(ersteradvent(),			QObject::tr("1. Advent"));
	m_holidays.insert(mariaempfaengnis(),		QObject::tr("Maria Emphängnis"));
	m_holidays.insert(ersterweihnachtstag(),	QObject::tr("1. Weihnachtstag"));
	m_holidays.insert(zweiterweihnachtstag(),	QObject::tr("2. Weihnachtstag"));
	m_holidays.insert(silvester(),				QObject::tr("Silvester"));
}

/*
1,1,jahr,   "Neujahr (18 Uhr)"
6,1,jahr,   "Hl. Drei Könige"
14,7,jahr,  "Sel. Bernhard"
15,8,jahr,  "Mariä Himmelfahrt"
1,11,jahr,  "Allerheiligen"
8,12,jahr,  "Maria Empfängnis"
24,12,jahr, "Heilig Abend"
25,12,jahr, "1.Weihnachtsfeiertag"
26,12,jahr, "2.Weihnachtsfeiertag"
// von Ostern abhängige Feiertage
Ostern-46,  "Aschermittwoch"
Ostern-7,   "Palmsonntag"
Ostern-3,   "Gründonnerstag"
Ostern-2,   "Karfreitag"
Ostern-1,   "Karsamstag"
Ostern,     "Ostersonntag"
Ostern+1,   "Ostermontag"
Ostern+39,  "Christi Himmelfahrt"
Ostern+49,  "Pfingstsonntag"
Ostern+50,  "Pfingstmontag"
Ostern+60,  "Fronleichnam"
*/

bool cPublicHoliday::isPublicHoliday(const QDate& date)
{
	return(m_holidays.contains(date));
}

QString cPublicHoliday::name(const QDate& date)
{
	return(m_holidays.value(date, ""));
}

QDate cPublicHoliday::neujahr()
{
	return(QDate(m_year, 1, 1));
}

QDate cPublicHoliday::heilige3koenige()
{
	return(QDate(m_year, 1, 6));
}

QDate cPublicHoliday::karfreitag()
{
	return(ostersonntag().addDays(-2));
}

QDate cPublicHoliday::ostersonntag()
{
	int a = m_year % 19;
	int b = m_year / 100;
	int c = m_year % 100;
	int e = b % 4;
	int f = (b + 8) / 25;
	int g = (b - f + 1) / 3;
	int h = (19 * a + b - (b/4) - g + 15) % 30;
	int k = c % 4;
	int l = (32 + 2 * e + 2 * (c / 4) - h - k) % 7;
	int m = (a + 11 * h + 22 * l) / 451;
	int nMonth = (h + l - 7 * m + 114) / 31;
	int nDay = (h + l - 7 * m + 114) % 31 + 1;

	return(QDate(m_year, nMonth, nDay));
}

QDate cPublicHoliday::ostermontag()
{
	return(ostersonntag().addDays(1));
}

QDate cPublicHoliday::staatsfeiertag1()
{
	return(QDate(m_year, 5, 1));
}

QDate cPublicHoliday::christihimmelfahrt()
{
	return(ostersonntag().addDays(39));
}

QDate cPublicHoliday::pfingstsonntag()
{
	return(ostersonntag().addDays(49));
}

QDate cPublicHoliday::pfingstmontag()
{
	return(ostersonntag().addDays(50));
}

QDate cPublicHoliday::fronleichnam()
{
	return(ostersonntag().addDays(60));
}

QDate cPublicHoliday::mariahimmelfahrt()
{
	return(QDate(m_year, 8, 15));
}

QDate cPublicHoliday::staatsfeiertag()
{
	return(QDate(m_year, 10, 26));
}

QDate cPublicHoliday::allerheiligen()
{
	return(QDate(m_year, 11, 1));
}

QDate cPublicHoliday::ersteradvent()
{
	QDate	date(m_year, 11, 27);
	return(date.addDays(7-date.dayOfWeek()));
}

QDate cPublicHoliday::mariaempfaengnis()
{
	return(QDate(m_year, 12, 8));
}

QDate cPublicHoliday::ersterweihnachtstag()
{
	return(QDate(m_year, 12, 25));
}

QDate cPublicHoliday::zweiterweihnachtstag()
{
	return(QDate(m_year, 12, 26));
}

QDate cPublicHoliday::silvester()
{
	return(QDate(m_year, 12, 31));
}

QMap<QDate, QString> cPublicHoliday::list()
{
	return(m_holidays);
}
