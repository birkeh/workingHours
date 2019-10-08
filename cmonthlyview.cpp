#include "cmonthlyview.h"
#include "ui_cmonthlyview.h"

#include "common.h"

#include <QDebug>


cMonthlyView::cMonthlyView(const QDate& date, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cMonthlyView)
{
	m_code.insert(" ", tr(""));
	m_code.insert("G", tr("Gleitzeit"));
	m_code.insert("U", tr("Urlaub"));
	m_code.insert("K", tr("Krank"));
	m_code.insert("SU", tr("Sonderurlaub"));

	ui->setupUi(this);

	m_lpMonthlyListModel	= new QStandardItemModel(0, 1);
	ui->m_lpMonthlyList->setModel(m_lpMonthlyListModel);

	ui->m_lpMonthlyList->setItemDelegate(new cMonthlyItemDelegate(m_code));

	setDate(date);

	connect(static_cast<cMonthlyItemDelegate*>(ui->m_lpMonthlyList->itemDelegate()),	&cMonthlyItemDelegate::timeChanged,	this,	&cMonthlyView::onTimeChanged);
	connect(static_cast<cMonthlyItemDelegate*>(ui->m_lpMonthlyList->itemDelegate()),	&cMonthlyItemDelegate::textChanged,	this,	&cMonthlyView::onTextChanged);
}

void cMonthlyView::setDate(const QDate& date)
{
	m_lpMonthlyListModel->clear();

	QStringList	header;
	header << tr("day") << tr("public holiday") << tr("come1") << tr("go1") << tr("come2") << tr("go2") << tr("come3") << tr("go3") << tr("break") << tr("code") << tr("is") << tr("should") << tr("+/-") << tr("information") << tr("current +/-") << tr("hours dec") << tr("day");
	m_lpMonthlyListModel->setHorizontalHeaderLabels(header);

	QList<QStandardItem*>	items;
	for(int y = 0;y < header.count();y++)
		items.append(new QStandardItem);

	items[COL_DAY1]->setText("99");
	items[COL_PUBLIC_HOLIDAY]->setText("");
	items[COL_COME1]->setText("00:00:00");
	items[COL_GO1]->setText("00:00:00");
	items[COL_COME2]->setText("00:00:00");
	items[COL_GO2]->setText("00:00:00");
	items[COL_COME3]->setText("00:00:00");
	items[COL_GO3]->setText("00:00:00");
	items[COL_BREAK]->setText("00:00:00");
	items[COL_CODE]->setText("");
	items[COL_IS]->setText("00:00:00");
	items[COL_SHOULD]->setText("00:00:00");
	items[COL_DIFF]->setText("00:00:00");
	items[COL_INFORMATION]->setText("");
	items[COL_CURRENT]->setText("00:00:00");
	items[COL_HOURS_DEC]->setText("0.0");
	items[COL_DAY2]->setText("99");

	m_lpMonthlyListModel->appendRow(items);

	for(int y = 0;y < header.count();y++)
		ui->m_lpMonthlyList->resizeColumnToContents(y);

	m_lpMonthlyListModel->removeRows(0, 1);

	QDate	date1	= date;
	date1.setDate(date.year(), date.month(), 1);

	for(;date1.month() == date.month();date1 = date1.addDays(1))
	{
		items.clear();

		for(int y = 0;y < header.count();y++)
			items.append(new QStandardItem);

		items[COL_DAY1]->setText(date1.toString("dddd dd"));
		items[COL_PUBLIC_HOLIDAY]->setText("");
		items[COL_COME1]->setText("");
		items[COL_GO1]->setText("");
		items[COL_COME2]->setText("");
		items[COL_GO2]->setText("");
		items[COL_COME3]->setText("");
		items[COL_GO3]->setText("");
		items[COL_BREAK]->setText("00:00:00");
		items[COL_CODE]->setText("");
		items[COL_IS]->setText("00:00:00");
		items[COL_SHOULD]->setText("00:00:00");
		items[COL_DIFF]->setText("00:00:00");
		items[COL_INFORMATION]->setText("");
		items[COL_CURRENT]->setText("00:00:00");
		items[COL_HOURS_DEC]->setText("0.0");
		items[COL_DAY2]->setText(date1.toString("dd dddd"));

		items[COL_DAY1]->setTextAlignment(Qt::AlignRight);
		items[COL_PUBLIC_HOLIDAY]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME1]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO1]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME2]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO2]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME3]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO3]->setTextAlignment(Qt::AlignHCenter);
		items[COL_BREAK]->setTextAlignment(Qt::AlignHCenter);
		items[COL_CODE]->setTextAlignment(Qt::AlignHCenter);
		items[COL_IS]->setTextAlignment(Qt::AlignHCenter);
		items[COL_SHOULD]->setTextAlignment(Qt::AlignHCenter);
		items[COL_DIFF]->setTextAlignment(Qt::AlignHCenter);
		items[COL_INFORMATION]->setTextAlignment(Qt::AlignLeft);
		items[COL_CURRENT]->setTextAlignment(Qt::AlignHCenter);
		items[COL_HOURS_DEC]->setTextAlignment(Qt::AlignRight);
		items[COL_DAY2]->setTextAlignment(Qt::AlignLeft);

		m_lpMonthlyListModel->appendRow(items);
	}
	ui->m_lpMonthlyList->resizeColumnToContents(COL_DAY1);
	ui->m_lpMonthlyList->resizeColumnToContents(COL_DAY2);
}

cMonthlyView::~cMonthlyView()
{
	delete ui;
}

void cMonthlyView::onTimeChanged(const int day, const int field, const QTime& time)
{
	qDebug() << "Day: " << day;
	qDebug() << "field: " << field;
	qDebug() << "time: " << time;
}

void cMonthlyView::onTextChanged(const int day, const int field, const QString& text)
{
	qDebug() << "Day: " << day;
	qDebug() << "field: " << field;
	qDebug() << "time: " << text;
}
