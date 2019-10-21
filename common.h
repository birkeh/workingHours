#ifndef COMMON_H
#define COMMON_H


#include <QString>
#include <QColor>


#define COL_DAY1			0
#define COL_PUBLIC_HOLIDAY	1
#define COL_COME1			2
#define COL_GO1				3
#define COL_COME2			4
#define COL_GO2				5
#define COL_COME3			6
#define COL_GO3				7
#define COL_COME4			8
#define COL_GO4				9
#define COL_COME5			10
#define COL_GO5				11
#define COL_BREAK			12
#define COL_CODE			13
#define COL_IS				14
#define COL_SHOULD			15
#define COL_DIFF			16
#define COL_INFORMATION		17
#define COL_CURRENT			18
#define COL_HOURS_DEC		19
#define COL_DAY2			20

#define DATA_HOLIDAY		(Qt::UserRole+1)
#define DATA_BOOKING		(Qt::UserRole+2)

#define COLOR_GLEITZEIT		QColor(191, 191, 255)
#define COLOR_URLAUB		QColor(127, 127, 255)
#define COLOR_SONDERURLAUB	QColor(127, 127, 255)
#define COLOR_KRANK			QColor(Qt::yellow)
#define COLOR_TRAINING		QColor(Qt::red)
#define COLOR_WEEKEND		QColor(255, 165, 0)


QString		secs2String(const qint32& secs);


#endif // COMMON_H
