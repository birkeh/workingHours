#include "cmonthlyview.h"
#include "ui_cmonthlyview.h"

#include "common.h"

#include <QDebug>


cMonthlyView::cMonthlyView(const QDate& date, cPublicHoliday *lpPublicHoliday, cBookingList* lpBookingList, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cMonthlyView),
	m_loading(true),
	m_lpPublicHoliday(lpPublicHoliday),
	m_lpBookingList(lpBookingList),
	m_date(date)
{
	m_code.insert(" ", tr(""));
	m_code.insert("G", tr("Gleitzeit"));
	m_code.insert("U", tr("Urlaub"));
	m_code.insert("SU", tr("Sonderurlaub"));
	m_code.insert("K", tr("Krank"));
	m_code.insert("T", tr("Training"));

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
	ui->m_lpTrainingLabel->setAutoFillBackground(true);
	ui->m_lpTrainingLabel->setStyleSheet(QString("QLabel { background-color: rgba(%1, %2, %3, 255); }").arg(COLOR_TRAINING.red()).arg(COLOR_TRAINING.green()).arg(COLOR_TRAINING.blue()));

	ui->m_lpMonth->setDate(date);
	setDate(date);

	connect(static_cast<cMonthlyItemDelegate*>(ui->m_lpMonthlyList->itemDelegate()),	&cMonthlyItemDelegate::timeChanged,	this,	&cMonthlyView::onTimeChanged);
	connect(static_cast<cMonthlyItemDelegate*>(ui->m_lpMonthlyList->itemDelegate()),	&cMonthlyItemDelegate::textChanged,	this,	&cMonthlyView::onTextChanged);

	connect(ui->m_lpMonth,																&QDateEdit::dateChanged,			this,	&cMonthlyView::onDateChanged);
}

void cMonthlyView::setDate(const QDate& date)
{
	m_loading	= true;

	m_date		= date;

	m_lpPublicHoliday->setYear(static_cast<qint16>(date.year()));

	m_lpMonthlyListModel->clear();

	QStringList	header;
	header << tr("Tag") << tr("Feiertag") << tr("kommt 1") << tr("geht 1") << tr("kommt 2") << tr("geht 2") << tr("kommt 3") << tr("geht 3") << tr("kommt 4") << tr("geht 4") << tr("kommt 5") << tr("geht 5") << tr("Pause") << tr("Kode") << tr("ist") << tr("soll") << tr("+/-") << tr("information") << tr("aktuell +/-") << tr("dezimal") << tr("Tag");
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
	items[COL_COME4]->setText("00:00:00");
	items[COL_GO4]->setText("00:00:00");
	items[COL_COME5]->setText("00:00:00");
	items[COL_GO5]->setText("00:00:00");
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
		cBooking*	lpBooking	= m_lpBookingList->find(date1);

		if(!lpBooking)
		{
			lpBooking	= m_lpBookingList->add(date1);
			lpBooking->save();
		}

		items.clear();

		for(int y = 0;y < header.count();y++)
			items.append(new QStandardItem);

		items[COL_DAY1]->setText(date1.toString("dddd dd"));
		items[COL_PUBLIC_HOLIDAY]->setText(m_lpPublicHoliday->name(date1));
		items[COL_DAY2]->setText(date1.toString("dd dddd"));

		setText(items[COL_COME1], lpBooking->kommt1());
		setText(items[COL_COME1], lpBooking->kommt1());
		setText(items[COL_GO1], lpBooking->geht1());
		setText(items[COL_COME2], lpBooking->kommt2());
		setText(items[COL_GO2], lpBooking->geht2());
		setText(items[COL_COME3], lpBooking->kommt3());
		setText(items[COL_GO3], lpBooking->geht3());
		setText(items[COL_COME4], lpBooking->kommt4());
		setText(items[COL_GO4], lpBooking->geht4());
		setText(items[COL_COME5], lpBooking->kommt5());
		setText(items[COL_GO5], lpBooking->geht5());
		items[COL_BREAK]->setText(lpBooking->pause().toString("hh:mm:ss"));
		items[COL_CODE]->setText(lpBooking->code());
		items[COL_IS]->setText(lpBooking->ist().toString("hh:mm:ss"));
		items[COL_SHOULD]->setText(lpBooking->soll().toString("hh:mm:ss"));
		items[COL_DIFF]->setText(lpBooking->diffString());
		items[COL_INFORMATION]->setText(lpBooking->information());
		items[COL_CURRENT]->setText(lpBooking->currentDiffString());
		items[COL_HOURS_DEC]->setText(QString::number(lpBooking->hoursDecimal(), 'f', 2));

		items[COL_DAY1]->setTextAlignment(Qt::AlignRight);
		items[COL_PUBLIC_HOLIDAY]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME1]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO1]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME2]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO2]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME3]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO3]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME4]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO4]->setTextAlignment(Qt::AlignHCenter);
		items[COL_COME5]->setTextAlignment(Qt::AlignHCenter);
		items[COL_GO5]->setTextAlignment(Qt::AlignHCenter);
		items[COL_BREAK]->setTextAlignment(Qt::AlignHCenter);
		items[COL_CODE]->setTextAlignment(Qt::AlignHCenter);
		items[COL_IS]->setTextAlignment(Qt::AlignHCenter);
		items[COL_SHOULD]->setTextAlignment(Qt::AlignHCenter);
		items[COL_DIFF]->setTextAlignment(Qt::AlignRight);
		items[COL_INFORMATION]->setTextAlignment(Qt::AlignLeft);
		items[COL_CURRENT]->setTextAlignment(Qt::AlignRight);
		items[COL_HOURS_DEC]->setTextAlignment(Qt::AlignRight);
		items[COL_DAY2]->setTextAlignment(Qt::AlignLeft);

		for(int z = 0;z < items.count();z++)
		{
			if(date1.dayOfWeek() >= 6 || m_lpPublicHoliday->isPublicHoliday(date1))
			{
				items[z]->setData(QVariant::fromValue(true), DATA_HOLIDAY);
				items[z]->setBackground(weekend);
			}
			else
				items[z]->setData(QVariant::fromValue(false), DATA_HOLIDAY);

			items[z]->setData(QVariant::fromValue(lpBooking), DATA_BOOKING);
		}

		m_lpMonthlyListModel->appendRow(items);

		if(date1.dayOfWeek() < 6 && !m_lpPublicHoliday->isPublicHoliday(date1))
			setBackground(date1.day(), lpBooking->code());
	}
	ui->m_lpMonthlyList->resizeColumnToContents(COL_DAY1);
	ui->m_lpMonthlyList->resizeColumnToContents(COL_DAY2);
	ui->m_lpMonthlyList->resizeColumnToContents(COL_PUBLIC_HOLIDAY);

	ui->m_lpUebertragLabel->setText(QString(tr("Übertrag %1:")).arg(date.addMonths(-1).toString("MMMM yyyy")));
	ui->m_lpShouldLabel->setText(QString(tr("SOLL Arbeitszeit (%1):")).arg(date.toString("MMMM yyyy")));
	ui->m_lpIsLabel->setText(QString(tr("IST Arbeitszeit (%1):")).arg(date.toString("MMMM yyyy")));
	ui->m_lpResturlaubLabel->setText(QString(tr("Resturlaub %1:")).arg(date.addMonths(-1).toString("MMMM yyyy")));
	ui->m_lpRestSonderurlaubLabel->setText(QString(tr("Sonderurlaub %1:")).arg(date.addMonths(-1).toString("MMMM yyyy")));

	m_loading	= false;
}

cMonthlyView::~cMonthlyView()
{
	delete ui;
}

void cMonthlyView::onTimeChanged(const int day, const int field, const QTime& time)
{
	QStandardItem*	lpItem		= m_lpMonthlyListModel->item(day-1, field);
	if(!lpItem)
		return;

	cBooking*		lpBooking	= lpItem->data(DATA_BOOKING).value<cBooking*>();
	if(!lpBooking)
		return;

	switch(field)
	{
	case COL_COME1:
		lpBooking->setKommt1(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_GO1:
		lpBooking->setGeht1(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_COME2:
		lpBooking->setKommt2(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_GO2:
		lpBooking->setGeht2(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_COME3:
		lpBooking->setKommt3(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_GO3:
		lpBooking->setGeht3(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_COME4:
		lpBooking->setKommt4(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_GO4:
		lpBooking->setGeht4(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_COME5:
		lpBooking->setKommt5(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	case COL_GO5:
		lpBooking->setGeht5(time);
		lpBooking->save();
		recalculate(day, field);
		break;
	}
}

void cMonthlyView::onTextChanged(const int day, const int field, const QString& text)
{
	QStandardItem*	lpItem		= m_lpMonthlyListModel->item(day-1, field);
	if(!lpItem)
		return;

	cBooking*		lpBooking	= lpItem->data(DATA_BOOKING).value<cBooking*>();
	if(!lpBooking)
		return;

	switch(field)
	{
	case COL_CODE:
	{
		QString	code	= m_code.key(text);

		setBackground(day, code);
		lpBooking->setCode(code);
		lpBooking->save();
		recalculate(day, field);
		break;
	}
	case COL_INFORMATION:
		lpBooking->setInformation(text);
		lpBooking->save();
		break;
	}
}

void cMonthlyView::onDateChanged(const QDate& date)
{
	setDate(date);
}

void cMonthlyView::setBackground(const int day, const QString& code)
{
	QBrush	brush;

	if(code == "G")
		brush	= QBrush(COLOR_GLEITZEIT);
	else if(code == "U")
		brush	= QBrush(COLOR_URLAUB);
	else if(code == "K")
		brush	= QBrush(COLOR_KRANK);
	else if(code == "SU")
		brush	= QBrush(COLOR_SONDERURLAUB);
	else if(code == "T")
		brush	= QBrush(COLOR_TRAINING);

	for(int x = 0;x < m_lpMonthlyListModel->columnCount();x++)
	{
		QStandardItem* lpItem	= m_lpMonthlyListModel->item(day-1, x);
		lpItem->setBackground(brush);
	}
}

void cMonthlyView::setText(QStandardItem* lpItem, const QTime& time)
{
	if(time == QTime(0, 0, 0))
		lpItem->setText("");
	else
		lpItem->setText(time.toString("hh:mm:ss"));
}

void cMonthlyView::recalculate(int day, int /*field*/)
{
	m_lpBookingList->recalculate(QDate(m_date.year(), m_date.month(), day));

	QStandardItem*	lpPauseItem	= m_lpMonthlyListModel->item(day-1, COL_BREAK);
	QStandardItem*	lpIstItem	= m_lpMonthlyListModel->item(day-1, COL_IS);
	QStandardItem*	lpSollItem	= m_lpMonthlyListModel->item(day-1, COL_SHOULD);
	QStandardItem*	lpDecItem	= m_lpMonthlyListModel->item(day-1, COL_HOURS_DEC);
	QStandardItem*	lpDiffItem	= m_lpMonthlyListModel->item(day-1, COL_DIFF);

	cBooking*		lpBooking	= lpPauseItem->data(DATA_BOOKING).value<cBooking*>();

	lpPauseItem->setText(lpBooking->pause().toString("hh:mm:ss"));
	lpIstItem->setText(lpBooking->ist().toString("hh:mm:ss"));
	lpDecItem->setText(QString::number(lpBooking->hoursDecimal(), 'f', 2));
	lpSollItem->setText(lpBooking->soll().toString("hh:mm:ss"));
	lpDiffItem->setText(lpBooking->diffString());

	for(int i = day;i <= m_date.daysInMonth();i++)
	{
		QStandardItem*	lpCurrentDiffItem	= m_lpMonthlyListModel->item(i-1, COL_CURRENT);
		cBooking*		lpCurrentBooking	= lpCurrentDiffItem->data(DATA_BOOKING).value<cBooking*>();
		lpCurrentDiffItem->setText(lpCurrentBooking->currentDiffString());
	}
}
