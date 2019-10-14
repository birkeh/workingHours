#ifndef CBOOKING_H
#define CBOOKING_H


#include "cdailyworking.h"

#include <QMetaType>
#include <QList>
#include <QObject>
#include <QDate>
#include <QTime>

#include <QSqlQuery>


class cBooking : public QObject
{
	Q_OBJECT

public:
	explicit			cBooking(const QDate& date, QObject* parent = nullptr);

	QDate				date();

	void				setKommt1(const QTime& time);
	QTime				kommt1();

	void				setGeht1(const QTime& time);
	QTime				geht1();

	void				setKommt2(const QTime& time);
	QTime				kommt2();

	void				setGeht2(const QTime& time);
	QTime				geht2();

	void				setKommt3(const QTime& time);
	QTime				kommt3();

	void				setGeht3(const QTime& time);
	QTime				geht3();

	void				setKommt4(const QTime& time);
	QTime				kommt4();

	void				setGeht4(const QTime& time);
	QTime				geht4();

	void				setKommt5(const QTime& time);
	QTime				kommt5();

	void				setGeht5(const QTime& time);
	QTime				geht5();

	void				setCode(const QString& code);
	QString				code();

	void				setInformation(const QString& information);
	QString				information();

	qint32				pauseSecs();
	QTime				pause();

	qint32				istSecs();
	QTime				ist();

	void				setSoll(const QTime& time);
	QTime				soll();

	qint32				diff();
	QString				diffString();

	void				setPrevDiff(qint32 diff);
	qint32				prevDiff();

	qint32				currentDiff();
	QString				currentDiffString();

	qreal				hoursDecimal();

	bool				save();
private:
	QDate				m_date;
	QTime				m_kommt1;
	QTime				m_geht1;
	QTime				m_kommt2;
	QTime				m_geht2;
	QTime				m_kommt3;
	QTime				m_geht3;
	QTime				m_kommt4;
	QTime				m_geht4;
	QTime				m_kommt5;
	QTime				m_geht5;
	QString				m_code;
	QString				m_information;
	QTime				m_soll;
	qint32				m_prevDiff;

	void				bind(QSqlQuery& query, const QString& variable, const QTime& time);

	int					totalWork();
	int					totalPause();
};

Q_DECLARE_METATYPE(cBooking*)

class cBookingList : public QList<cBooking*>
{
public:
	bool				load(cDailyWorkingList* m_dailyWorkingList);
	cBooking*			add(const QDate& date);
	cBooking*			find(const QDate& date);

private:
	cDailyWorkingList*	m_lpDailyWorkingList;
};

#endif // CBOOKING_H
