#ifndef CPUBLICHOLIDAY_H
#define CPUBLICHOLIDAY_H


#include <QString>
#include <QDate>

#include <QMap>


class cPublicHoliday
{
public:
	cPublicHoliday(qint16 year = 2000);

	void					setYear(qint16 year);
	qint16					year();

	bool					isPublicHoliday(const QDate& date);
	QString					name(const QDate& date);
	QMap<QDate, QString>	list();

private:
	qint16					m_year;
	QMap<QDate, QString>	m_holidays;

	void					calculate();

	QDate					neujahr();
	QDate					heilige3koenige();
	QDate					karfreitag();
	QDate					ostersonntag();
	QDate					ostermontag();
	QDate					staatsfeiertag1();
	QDate					christihimmelfahrt();
	QDate					pfingstsonntag();
	QDate					pfingstmontag();
	QDate					fronleichnam();
	QDate					mariahimmelfahrt();
	QDate					staatsfeiertag();
	QDate					allerheiligen();
	QDate					ersteradvent();
	QDate					mariaempfaengnis();
	QDate					ersterweihnachtstag();
	QDate					zweiterweihnachtstag();
	QDate					silvester();
};
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

#endif // CPUBLICHOLIDAY_H
