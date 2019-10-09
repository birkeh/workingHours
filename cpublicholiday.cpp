#include "cpublicholiday.h"

#include <QObject>


cPublicHoliday::cPublicHoliday(qint16 year) :
	m_year(year)
{
	m_holidays.insert(ostersonntag(), QObject::tr("Ostersonntag"));
}

bool cPublicHoliday::isPublicHoliday(const QDate& date)
{
	return(m_holidays.contains(date));
}

QString cPublicHoliday::name(const QDate& date)
{
	return(m_holidays.value(date, ""));
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
