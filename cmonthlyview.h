#ifndef CMONTHLYVIEW_H
#define CMONTHLYVIEW_H


#include "cmonthlyitemdelegate.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QMap>
#include <QTime>


namespace Ui {
class cMonthlyView;
}

class cMonthlyView : public QWidget
{
	Q_OBJECT

public:
	explicit cMonthlyView(const QDate& date, QWidget *parent = nullptr);
	~cMonthlyView();

	void					setDate(const QDate& date);
private:
	Ui::cMonthlyView*		ui;
	QStandardItemModel*		m_lpMonthlyListModel;
	QMap<QString, QString>	m_code;

private slots:
	void					onTimeChanged(const int day, const int field, const QTime& time);
	void					onTextChanged(const int day, const int field, const QString& text);
};

#endif // CMONTHLYVIEW_H
