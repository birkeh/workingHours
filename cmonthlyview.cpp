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

	ui->m_lpGleitzeitLabel->setAutoFillBackground(true);
	ui->m_lpGleitzeitLabel->setStyleSheet(QString("QLabel { background-color: rgba(%1, %2, %3, 255); }").arg(COLOR_GLEITZEIT.red()).arg(COLOR_GLEITZEIT.green()).arg(COLOR_GLEITZEIT.blue()));
	ui->m_lpKrankLabel->setAutoFillBackground(true);
	ui->m_lpKrankLabel->setStyleSheet(QString("QLabel { background-color: rgba(%1, %2, %3, 255); }").arg(COLOR_KRANK.red()).arg(COLOR_KRANK.green()).arg(COLOR_KRANK.blue()));
	ui->m_lpUrlaubLabel->setAutoFillBackground(true);
	ui->m_lpUrlaubLabel->setStyleSheet(QString("QLabel { background-color: rgba(%1, %2, %3, 255); }").arg(COLOR_URLAUB.red()).arg(COLOR_URLAUB.green()).arg(COLOR_URLAUB.blue()));
	ui->m_lpSonderurlaubLabel->setAutoFillBackground(true);
	ui->m_lpSonderurlaubLabel->setStyleSheet(QString("QLabel { background-color: rgba(%1, %2, %3, 255); }").arg(COLOR_SONDERURLAUB.red()).arg(COLOR_SONDERURLAUB.green()).arg(COLOR_SONDERURLAUB.blue()));

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
	items[COL_DIFF]->setText("+00:00:00");
	items[COL_INFORMATION]->setText("");
	items[COL_CURRENT]->setText("+00:00:00");
	items[COL_HOURS_DEC]->setText("0.0");
	items[COL_DAY2]->setText("99");

	m_lpMonthlyListModel->appendRow(items);

	for(int y = 0;y < header.count();y++)
		ui->m_lpMonthlyList->resizeColumnToContents(y);

	m_lpMonthlyListModel->removeRows(0, 1);

	QBrush	weekend	= QBrush(COLOR_WEEKEND);
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

		for(int z = 0;z < items.count();z++)
		{
			if(date1.dayOfWeek() >= 6)
			{
				items[z]->setData(QVariant::fromValue(true), Qt::UserRole+1);
				items[z]->setBackground(weekend);
			}
			else
				items[z]->setData(QVariant::fromValue(false), Qt::UserRole+1);
		}

		m_lpMonthlyListModel->appendRow(items);
	}
	ui->m_lpMonthlyList->resizeColumnToContents(COL_DAY1);
	ui->m_lpMonthlyList->resizeColumnToContents(COL_DAY2);

	ui->m_lpUebertragLabel->setText(QString(tr("Übertrag %1:")).arg(date.addMonths(-1).toString("MMMM yyyy")));
	ui->m_lpShouldLabel->setText(QString(tr("SOLL Arbeitszeit (%1):")).arg(date.toString("MMMM yyyy")));
	ui->m_lpIsLabel->setText(QString(tr("IST Arbeitszeit (%1):")).arg(date.toString("MMMM yyyy")));
	ui->m_lpResturlaubLabel->setText(QString(tr("Resturlaub %1:")).arg(date.addMonths(-1).toString("MMMM yyyy")));
	ui->m_lpRestSonderurlaubLabel->setText(QString(tr("Sonderurlaub %1:")).arg(date.addMonths(-1).toString("MMMM yyyy")));
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

	if(field == COL_CODE)
	{
		QBrush	brush;
		QString	code	= m_code.key(text);

		if(code == "G")
			brush	= QBrush(COLOR_GLEITZEIT);
		else if(code == "U")
			brush	= QBrush(COLOR_URLAUB);
		else if(code == "K")
			brush	= QBrush(COLOR_KRANK);
		else if(code == "SU")
			brush	= QBrush(COLOR_SONDERURLAUB);

		for(int x = 0;x < m_lpMonthlyListModel->columnCount();x++)
		{
			QStandardItem* lpItem	= m_lpMonthlyListModel->item(day-1, x);
			lpItem->setBackground(brush);
		}
	}
}
