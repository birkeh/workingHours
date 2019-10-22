#ifndef COMMON_H
#define COMMON_H


#include "common.h"

#include <QTime>


QString secs2String(const qint32& secs)
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

	return((neg ? "-" : "") + QString::number(h) + t.toString(":mm:ss"));
}

qint32 time2Secs(const QTime& time)
{
	return(time.hour()*3600+time.minute()*60+time.second());
}

#endif //COMMON_H
