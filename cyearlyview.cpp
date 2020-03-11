#include "common.h"

#include "cyearlyview.h"
#include "ui_cyearlyview.h"

#include <QDebug>


cYearlyView::cYearlyView(const QDate& date, const QMap<QString, QString> code, cMonthlyBookingList* lpMonthlyBookingList, cPublicHoliday *lpPublicHoliday, cBookingList* lpBookingList, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cYearlyView),
	m_loading(true),
	m_code(code),
	m_lpMonthlyBookingList(lpMonthlyBookingList),
	m_lpPublicHoliday(lpPublicHoliday),
	m_lpBookingList(lpBookingList),
	m_date(date),
	m_lpMonthlyBooking(nullptr)
{
	ui->setupUi(this);

	m_lpYearlyListModel	= new QStandardItemModel(0, 1);
	ui->m_lpYearlyList->setModel(m_lpYearlyListModel);

	setDate(date);

	m_loading	= false;
}

cYearlyView::~cYearlyView()
{
	delete ui;
}

QStandardItem*	newItem(const QString& string, const QFont& font, const Qt::Alignment& alignment)
{
	QStandardItem*	lpItem	= new QStandardItem(string);
	lpItem->setFont(font);
	lpItem->setTextAlignment(alignment);
	return(lpItem);
}

void cYearlyView::setDate(const QDate& date)
{
	m_loading	= true;
	m_date		= date;

	m_lpPublicHoliday->setYear(static_cast<qint16>(date.year()));

	m_lpYearlyListModel->clear();

	ui->m_lpYear->setDate(date);

	QStringList	strings;
	QFont		font			= ui->m_lpYearlyList->font();
	QFont		fontBold		= font;
	QBrush		weekend			= QBrush(COLOR_WEEKEND);
	QBrush		gleitzeit		= QBrush(COLOR_GLEITZEIT);
	QBrush		urlaub			= QBrush(COLOR_URLAUB);
	QBrush		sonderurlaub	= QBrush(COLOR_SONDERURLAUB);
	QBrush		krank			= QBrush(COLOR_KRANK);
	QBrush		training		= QBrush(COLOR_TRAINING);
	QList<int>	spanList;

	fontBold.setBold(true);

	for(int x = 0;x < 31;x++)
		strings.append(QString::number(x+1));

	strings.append("SOLL");
	strings.append("IST");
	strings.append("+/-");
	strings.append("Gleitzeit");
	strings.append("Krank");
	strings.append("Urlaub");
	strings.append("Resturlaub");
	strings.append("Sonderurlaub");
//	strings.append("Sonderurlaub Rest");

	ui->m_lpYearlyList->horizontalHeader()->setMinimumSectionSize(1);
	ui->m_lpYearlyList->setMinimumWidth(1);
	ui->m_lpYearlyList->verticalHeader()->setMinimumSectionSize(1);
	ui->m_lpYearlyList->setMinimumHeight(1);

	QList<QStandardItem*>	itemList;

	itemList.append(newItem("Day",			fontBold,	Qt::AlignHCenter));
	itemList.append(newItem("January",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("February",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("March",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("April",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("May",			font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("June",			font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("July",			font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("August",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("September",	font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("October",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("November",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("December",		font,		Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));	itemList.append(newItem("",	font,	Qt::AlignHCenter));
	itemList.append(newItem("Day",			fontBold,	Qt::AlignHCenter));

	for(int x = 0;x < itemList.count();x++)
		itemList[x]->setData(QBrush(Qt::gray), Qt::BackgroundRole);

	m_lpYearlyListModel->appendRow(itemList);

	for(int x = 0;x < 12;x++)
	{
		ui->m_lpYearlyList->setSpan(0, x*3+1, 1, 3);

		ui->m_lpYearlyList->setColumnWidth(x*3+1, 23);
		ui->m_lpYearlyList->setColumnWidth(x*3+2, 60);
		ui->m_lpYearlyList->setColumnWidth(x*3+3, 19);
	}

	for(int x = 0;x < strings.count();x++)
	{
		spanList.clear();

		QList<QStandardItem*>	itemList;

		itemList.append(newItem(strings[x], fontBold, Qt::AlignHCenter));
		itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);

		if(x < 31)
		{
			for(int y = 0;y < 12;y++)
			{
				QDate	d(m_date.year(), y+1, x+1);

				if(d.isValid())
				{
					cBooking*	lpBooking	= m_lpBookingList->find(d);

					itemList.append(newItem(d.toString("ddd").left(2), font, Qt::AlignRight));
					itemList.append(newItem("", font, Qt::AlignRight));
					itemList.append(newItem("", font, Qt::AlignLeft));

					if(m_lpPublicHoliday->isPublicHoliday(d))
					{
						itemList[itemList.count()-3]->setData(weekend, Qt::BackgroundRole);
						itemList[itemList.count()-2]->setData(weekend, Qt::BackgroundRole);
						itemList[itemList.count()-1]->setData(weekend, Qt::BackgroundRole);

						itemList[itemList.count()-2]->setText(m_lpPublicHoliday->name(d));

						spanList.append(itemList.count()-2);
					}
					else if(d.dayOfWeek() >= 6)
					{
						itemList[itemList.count()-3]->setData(weekend, Qt::BackgroundRole);
						itemList[itemList.count()-2]->setData(weekend, Qt::BackgroundRole);
						itemList[itemList.count()-1]->setData(weekend, Qt::BackgroundRole);
					}
					else if(lpBooking)
					{
						itemList[itemList.count()-2]->setData(QVariant::fromValue(lpBooking), Qt::UserRole);

						if(lpBooking->code() == "G")
						{
							itemList[itemList.count()-3]->setData(gleitzeit, Qt::BackgroundRole);
							itemList[itemList.count()-2]->setData(gleitzeit, Qt::BackgroundRole);
							itemList[itemList.count()-1]->setData(gleitzeit, Qt::BackgroundRole);

							itemList[itemList.count()-1]->setText(lpBooking->code());
						}
						else if(lpBooking->code() == "U")
						{
							itemList[itemList.count()-3]->setData(urlaub, Qt::BackgroundRole);
							itemList[itemList.count()-2]->setData(urlaub, Qt::BackgroundRole);
							itemList[itemList.count()-1]->setData(urlaub, Qt::BackgroundRole);

							itemList[itemList.count()-1]->setText(lpBooking->code());
						}
						else if(lpBooking->code() == "SU")
						{
							itemList[itemList.count()-3]->setData(sonderurlaub, Qt::BackgroundRole);
							itemList[itemList.count()-2]->setData(sonderurlaub, Qt::BackgroundRole);
							itemList[itemList.count()-1]->setData(sonderurlaub, Qt::BackgroundRole);

							itemList[itemList.count()-1]->setText(lpBooking->code());
						}
						else if(lpBooking->code() == "K")
						{
							itemList[itemList.count()-3]->setData(krank, Qt::BackgroundRole);
							itemList[itemList.count()-2]->setData(krank, Qt::BackgroundRole);
							itemList[itemList.count()-1]->setData(krank, Qt::BackgroundRole);

							itemList[itemList.count()-1]->setText(lpBooking->code());
						}
						else if(lpBooking->code() == "T")
						{
							itemList[itemList.count()-3]->setData(training, Qt::BackgroundRole);
							itemList[itemList.count()-2]->setData(training, Qt::BackgroundRole);
							itemList[itemList.count()-1]->setData(training, Qt::BackgroundRole);

							itemList[itemList.count()-1]->setText(lpBooking->code());
						}
						else
						{
							itemList[itemList.count()-2]->setText(lpBooking->ist().toString("hh:mm:ss"));
						}
					}
				}
				else
				{
					itemList.append(newItem("", font, Qt::AlignRight));
					itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);
					itemList.append(newItem("", font, Qt::AlignHCenter));
					itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);
					itemList.append(newItem("", font, Qt::AlignLeft));
					itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);
				}
			}
		}
		else
		{
			for(int y = 0;y < 12;y++)
			{
				itemList.append(newItem("", font, Qt::AlignRight));
				itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);
				itemList.append(newItem("", font, Qt::AlignRight));
				itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);
				itemList.append(newItem("", font, Qt::AlignLeft));
				itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);
			}
		}

		itemList.append(newItem(strings[x], fontBold, Qt::AlignHCenter));
		itemList[itemList.count()-1]->setData(QBrush(Qt::gray), Qt::BackgroundRole);

		m_lpYearlyListModel->appendRow(itemList);

		for(int y = 0;y < spanList.count();y++)
			ui->m_lpYearlyList->setSpan(m_lpYearlyListModel->rowCount()-1, spanList[y], 1, 2);
	}

	for(int x = 0;x < m_lpYearlyListModel->rowCount();x++)
		ui->m_lpYearlyList->setRowHeight(x, 15);

	ui->m_lpYearlyList->resizeColumnToContents(0);
	ui->m_lpYearlyList->resizeColumnToContents(m_lpYearlyListModel->columnCount()-1);

	qint32			sollT			= 0;
	qint32			istT			= 0;
	qint16			gT				= 0;
	qint16			kT				= 0;
	qint16			uT				= 0;
	qint16			suT				= 0;
	qint16			tT				= 0;
	qint16			restUrlaub		= 0;

	for(int col = 0;col < 12;col++)
	{
		qint32			soll			= 0;
		qint32			ist				= 0;
		qint16			g				= 0;
		qint16			k				= 0;
		qint16			u				= 0;
		qint16			su				= 0;
		qint16			t				= 0;

		cBooking*		lpFirstBooking	= nullptr;

		for(int row = 0;row < 31;row++)
		{
			QStandardItem*	lpItem		= m_lpYearlyListModel->item(row+1, col*3+2);

			if(lpItem)
			{
				cBooking*	lpBooking	= lpItem->data(Qt::UserRole).value<cBooking*>();
				if(lpBooking)
				{
					if(!lpFirstBooking)
						lpFirstBooking	= lpBooking;

					soll	+= lpBooking->sollSecs();
					ist		+= lpBooking->istSecs();

					if(lpBooking->code() == "G")
						g++;
					else if(lpBooking->code() == "K")
						k++;
					else if(lpBooking->code() == "U")
						u++;
					else if(lpBooking->code() == "SU")
						su++;
					else if(lpBooking->code() == "T")
						t++;
				}
			}
		}

		m_lpYearlyListModel->item(32, col*3+2)->setText(secs2String(soll));
		m_lpYearlyListModel->item(33, col*3+2)->setText(secs2String(ist));
		m_lpYearlyListModel->item(34, col*3+2)->setText(secs2String(ist-soll));
		if(ist-soll < 0)
			m_lpYearlyListModel->item(34, col*3+2)->setData(QBrush(Qt::red), Qt::ForegroundRole);

		m_lpYearlyListModel->item(35, col*3+2)->setText(QString::number(g));
		m_lpYearlyListModel->item(36, col*3+2)->setText(QString::number(k));
		m_lpYearlyListModel->item(37, col*3+2)->setText(QString::number(u));

		if(lpFirstBooking)
		{
			restUrlaub	= lpFirstBooking->vacation()-u;
			m_lpYearlyListModel->item(38, col*3+2)->setText(QString::number(restUrlaub));
			if(restUrlaub < 0)
				m_lpYearlyListModel->item(38, col*3+2)->setData(QBrush(Qt::red), Qt::ForegroundRole);
		}
		else
			m_lpYearlyListModel->item(38, col*3+2)->setText("0");

		m_lpYearlyListModel->item(39, col*3+2)->setText(QString::number(su));

		sollT	+= soll;
		istT	+= ist;
		gT		+= g;
		kT		+= k;
		uT		+= u;
		suT		+= su;
		tT		+= t;
	}

	m_lpYearlyListModel->item(32, 37)->setText(secs2String(sollT));
	m_lpYearlyListModel->item(33, 37)->setText(secs2String(istT));
	m_lpYearlyListModel->item(34, 37)->setText(secs2String(istT-sollT));
	if(istT-sollT < 0)
		m_lpYearlyListModel->item(34, 37)->setData(QBrush(Qt::red), Qt::ForegroundRole);

	m_lpYearlyListModel->item(35, 37)->setText(QString::number(gT));
	m_lpYearlyListModel->item(36, 37)->setText(QString::number(kT));
	m_lpYearlyListModel->item(37, 37)->setText(QString::number(uT));
	m_lpYearlyListModel->item(38, 37)->setText(QString::number(restUrlaub));
	if(restUrlaub < 0)
		m_lpYearlyListModel->item(38, 37)->setData(QBrush(Qt::red), Qt::ForegroundRole);

	m_lpYearlyListModel->item(39, 37)->setText(QString::number(suT));

	m_loading	= false;
}

void cYearlyView::onPrevYear()
{
	if(m_loading)
		return;

	setDate(ui->m_lpYear->date().addMonths(-12));
}

void cYearlyView::onNextYear()
{
	if(m_loading)
		return;

	setDate(ui->m_lpYear->date().addMonths(12));
}

void cYearlyView::onDateChanged(const QDate& date)
{
	if(m_loading)
		return;

	m_loading	= true;

	setDate(date);

	m_loading	= false;
}
