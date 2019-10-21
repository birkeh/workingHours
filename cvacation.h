#ifndef CVACATION_H
#define CVACATION_H


#include <QMetaType>
#include <QList>
#include <QObject>
#include <QDate>


class cVacation : public QObject
{
	Q_OBJECT
public:
	explicit		cVacation(const QDate& date, QObject *parent = nullptr);

	QDate			date();

	void			setDays(qint16 days);
	qint16			days();

private:
	QDate			m_date;
	qint16			m_days;

signals:

public slots:
};

Q_DECLARE_METATYPE(cVacation*)

class cVacationList : public QList<cVacation*>
{
public:
	bool			load();
	cVacation*		add(const QDate& date);
	cVacation*		find(const QDate& date);
	cVacation*		get(const QDate& date);
};

#endif // CVACATION_H
