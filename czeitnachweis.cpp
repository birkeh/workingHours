#include "czeitnachweis.h"

#include <podofo/podofo.h>
#include <QDebug>


using namespace PoDoFo;

cEntry::cEntry(entry type) :
	m_type(type),
	m_day(-1),
	m_von(QTime()),
	m_bis(QTime()),
	m_urlaub(false)
{
}

void cEntry::setDay(const int& day)
{
	m_day	= day;
}

int cEntry::day()
{
	return(m_day);
}

void cEntry::setVon(const QTime& time)
{
	m_von	= time;
}

QTime cEntry::von()
{
	return(m_von);
}

void cEntry::setBis(const QTime& time)
{
	m_bis	= time;
}

QTime cEntry::bis()
{
	return(m_bis);
}

void cEntry::setUrlaub(bool u)
{
	m_urlaub	= u;
}

bool cEntry::urlaub()
{
	return(m_urlaub);
}

cEntry* cEntryList::add(cEntry::entry type)
{
	cEntry*	lpNew	= new cEntry(type);
	append(lpNew);
	return(lpNew);
}

cZeitnachweis::cZeitnachweis(const QString& fileName, QObject *parent) :
	QObject(parent),
	m_fileName(fileName),
	m_gedrucktAm(QDate()),
	m_mitarbeiterNummer(""),
	m_mitarbeiterName(""),
	m_azpRegel(""),
	m_personalbereich(""),
	m_kostenstelle(""),
	m_von(QDate()),
	m_bis(QDate()),
	m_glzSaldoVorperiode(0),
	m_glzSaldoAktPeriode(0),
	m_uestLtDurchrechnungsz(0),
	m_glzSaldoGesamt(0),
	m_resturlaubstage(0),
	m_angeordneteUest(0),
	m_ersatzruhe(0),
	m_monatssummeAnger(0),
	m_reisezeiten(0)
{
	loadFile();
}

void cZeitnachweis::setValues(const QDate& /*von*/, const QDate& /*bis*/, const int& day)
{
	cEntry*	lpEntry	= m_entryList.add(cEntry::entry_normal);
	lpEntry->setDay(day);
	lpEntry->setUrlaub(true);
}

void cZeitnachweis::setValues(const QDate& /*von*/, const QDate& /*bis*/, const int& day, const QTime& start, const QTime& end)
{
	cEntry*	lpEntry	= m_entryList.add(cEntry::entry_normal);
	lpEntry->setDay(day);
	lpEntry->setVon(start);
	lpEntry->setBis(end);
}

bool cZeitnachweis::loadFile()
{
	PdfMemDocument	pdf(m_fileName.toStdWString().c_str());
	if(!pdf.IsLoaded())
		return(false);

	qreal		xPos	= 0.0;
	qreal		yPos	= 0.0;
	qreal		oldYPos	= -1.0;
	QStringList	lines;

	for(int pn = 0; pn < pdf.GetPageCount();pn++)
	{
		PdfPage* page = pdf.GetPage(pn);

		PdfContentsTokenizer		tok(page);
		const char*					token	= nullptr;
		PoDoFo::PdfVariant			var;
		PoDoFo::EPdfContentsType	type;

		while(tok.ReadNext(type, token, var))
		{
			if(token)
			{
				if(strstr(token, "BT"))
				{
					tok.ReadNext(type, token, var);
					xPos	= atof(token);
					tok.ReadNext(type, token, var);
					yPos	= atof(token);
				}
				else if(strstr(token, "Tj") || strstr(token, "TJ"))
				{
					PdfString		str		= var.GetString();
					QString			str1;

					str1	= QString::fromStdWString(str.GetStringW());

					if(oldYPos != yPos)
					{
						oldYPos	= yPos;
						lines.append(str1);
					}
					else
						lines.last().append(str1);
				}
			}
		}
	}

	for(int x = 0;x < lines.count();x++)
		qDebug() << lines[x];

	if(!lines[0].contains("Zeitnachweisliste"))
		return(false);

	QString		tmp;

	m_gedrucktAm		= QDate::fromString(lines[1].mid(13, 10), "dd.MM.yyyy");

	tmp					= lines[2].mid(17);
	m_mitarbeiterNummer	= tmp.left(tmp.indexOf(" "));
	m_mitarbeiterName	= tmp.mid(tmp.indexOf(" ")+1).simplified();

	m_azpRegel			= lines[3].mid(lines[3].indexOf("AZPRegel")+12).simplified();

	m_personalbereich	= lines[4].mid(16).simplified();

	m_kostenstelle		= lines[5].mid(17).simplified();

	tmp					= lines[6].mid(lines[6].indexOf("Zeitraum")+16);
	m_von				= QDate::fromString(tmp.left(10), "dd.MM.yyyy");
	m_bis				= QDate::fromString(tmp.mid(tmp.indexOf(" bis")+5, 10), "dd.MM.yyyy");

	int		day			= -1;
	QTime	start;
	QTime	end;
	bool	urlaub		= false;
	bool	feiertag	= false;
	int		i;

	for(i = 10;i < lines.count();i++)
	{
		QString	line	= lines[i];

		if(line.startsWith("***************"))
			continue;

		if(line.contains("Summenübersicht"))
			break;

		if(!line.startsWith("   "))
		{
			if(day != -1)
			{
				if(urlaub)
				{
					urlaub		= false;
					setValues(m_von, m_bis, day);
				}
				else if(feiertag)
					feiertag	= false;
				else
					setValues(m_von, m_bis, day, start, end);
			}

			day			= line.left(2).toInt();

			if(line.contains("Gebührenurla"))
				urlaub		= true;
			else if(line.mid(17, 5) == "     ")
				feiertag	= true;
			else
			{
				start		= QTime::fromString(line.mid(17, 5), "hh:mm");
				end			= QTime::fromString(line.mid(23, 5), "hh:mm");
			}
		}
		else if(line.contains("Dienstgang"))
			end			= QTime::fromString(line.mid(23, 5), "hh:mm");
		else if(line.startsWith("               "))
		{
			QTime	t	= QTime::fromString(line.mid(17, 5), "hh:mm");

			if(t != end)
			{
				setValues(m_von, m_bis, day, start, end);
				start	= t;
			}
			end			= QTime::fromString(line.mid(23, 5), "hh:mm");
		}
	}
	setValues(m_von, m_bis, day, start, end);

	for(;i < lines.count();i++)
	{
		QString	line	= lines[i];

		if(line.contains("GLZ-Saldo Vorperiode"))
			m_glzSaldoVorperiode	= line.mid(25, 13).simplified().toDouble();
		else if(line.contains("GLZ-Saldo akt. Periode"))
			m_glzSaldoAktPeriode	= line.mid(25, 13).simplified().toDouble();
		else if(line.contains("Üst lt. Durchrechnungsz."))
			m_uestLtDurchrechnungsz	= line.mid(25, 13).simplified().toDouble();
		else if(line.contains("GLZ-Saldo gesamt"))
			m_glzSaldoGesamt		= line.mid(25, 13).simplified().toDouble();

		if(line.contains("Resturlaubstage"))
			m_resturlaubstage		= line.mid(63).simplified().toDouble();
		else if(line.contains("angeordnete ÜST"))
			m_angeordneteUest		= line.mid(63).simplified().toDouble();
		else if(line.contains("Ersatzruhe"))
			m_ersatzruhe			= line.mid(63).simplified().toDouble();
		else if(line.contains("Monatssumme anger."))
			m_monatssummeAnger		= line.mid(63).simplified().toDouble();
		else if(line.contains("davon Reisezeiten"))
			m_reisezeiten			= line.mid(63).simplified().toDouble();
	}

	return(true);
}
