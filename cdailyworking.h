#ifndef CDAILYWORKING_H
#define CDAILYWORKING_H


#include <QMetaType>
#include <QList>
#include <QObject>
#include <QDate>
#include <QTime>


class cDailyWorking : public QObject
{
	Q_OBJECT
public:
	explicit		cDailyWorking(const QDate& date, QObject *parent = nullptr);

	QDate			date();

	void			setMontag(const QTime& time);
	QTime			montag();

	void			setDienstag(const QTime& time);
	QTime			dienstag();

	void			setMittwoch(const QTime& time);
	QTime			mittwoch();

	void			setDonnerstag(const QTime& time);
	QTime			donnerstag();

	void			setFreitag(const QTime& time);
	QTime			freitag();

	void			setSamstag(const QTime& time);
	QTime			samstag();

	void			setSonntag(const QTime& time);
	QTime			sonntag();

	QTime			soll(qint16 day);

private:
	QDate			m_date;
	QTime			m_montag;
	QTime			m_dienstag;
	QTime			m_mittwoch;
	QTime			m_donnerstag;
	QTime			m_freitag;
	QTime			m_samstag;
	QTime			m_sonntag;
};

Q_DECLARE_METATYPE(cDailyWorking*)

class cDailyWorkingList : public QList<cDailyWorking*>
{
public:
	bool			load();
	cDailyWorking*	add(const QDate& date);
	cDailyWorking*	find(const QDate& date);
	cDailyWorking*	get(const QDate& date);
};

#endif // CDAILYWORKING_H
