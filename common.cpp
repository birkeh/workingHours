#ifndef COMMON_H
#define COMMON_H


#include "common.h"

#include <QTime>
#include <QStringList>


QString secs2String(const qint32& secs, qint8 leading)
{
	QTime	t;
	qint32	h	= secs/3600;
	bool	neg	= false;

	if(secs < 0)
	{
		t	= QTime(0, 0, 0).addSecs(-secs);
		h	= -h;
		neg	= true;
	}
	else
		t	= QTime(0, 0, 0).addSecs(secs);

	return((neg ? "-" : "") + QString::number(h).rightJustified(leading, '0') + t.toString(":mm:ss"));
}

qint32 string2Secs(const QString& string)
{
	qint32		secs;
	QStringList	list	= string.split(":");
	qint32		h		= list[0].toInt();
	qint32		m		= list[1].toInt();
	qint32		s		= list[2].toInt();

	secs	= h*3600+m*60+s;

	if(string[0] == "-")
		secs	= -secs;

	return(secs);
}

qint32 time2Secs(const QTime& time)
{
	return(time.hour()*3600+time.minute()*60+time.second());
}

#endif //COMMON_H
