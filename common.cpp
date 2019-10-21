#ifndef COMMON_H
#define COMMON_H


#include "common.h"

#include <QTime>


QString secs2String(const qint32& secs)
{
	QTime	t;
	qint32	h	= secs/3600;

	if(secs < 0)
		t	= QTime(0, 0, 0).addSecs(-secs);
	else
		t	= QTime(0, 0, 0).addSecs(secs);

	return(QString::number(h) + t.toString(":mm:ss"));
}

#endif //COMMON_H
