#ifndef CZEITNACHWEIS_H
#define CZEITNACHWEIS_H


#include <QObject>
#include <QString>
#include <QDate>
#include <QTime>

#include <QMetaType>

#include <QList>


class cEntry
{
public:
	enum entry
	{
		entry_normal	= 1,
		entry_urlaub	= 2,
	};

	cEntry(entry type);

	void		setDay(const int& day);
	int			day();

	void		setVon(const QTime& time);
	QTime		von();

	void		setBis(const QTime& time);
	QTime		bis();

	void		setUrlaub(bool u);
	bool		urlaub();

private:
	entry		m_type;
	int			m_day;
	QTime		m_von;
	QTime		m_bis;
	bool		m_urlaub;
};

Q_DECLARE_METATYPE(cEntry*)

class cEntryList : public QList<cEntry*>
{
public:
	cEntry*		add(cEntry::entry type);
};

class cZeitnachweis : public QObject
{
	Q_OBJECT
public:
	explicit		cZeitnachweis(const QString& fileName, QObject *parent = nullptr);

	bool			loadFile();

private:
	QString			m_fileName;

	QDate			m_gedrucktAm;
	QString			m_mitarbeiterNummer;
	QString			m_mitarbeiterName;
	QString			m_azpRegel;
	QString			m_personalbereich;
	QString			m_kostenstelle;
	QDate			m_von;
	QDate			m_bis;
	qreal			m_glzSaldoVorperiode;
	qreal			m_glzSaldoAktPeriode;
	qreal			m_uestLtDurchrechnungsz;
	qreal			m_glzSaldoGesamt;
	qreal			m_resturlaubstage;
	qreal			m_angeordneteUest;
	qreal			m_ersatzruhe;
	qreal			m_monatssummeAnger;
	qreal			m_reisezeiten;

	cEntryList		m_entryList;

	void			setValues(const QDate& von, const QDate& bis, const int& day);
	void			setValues(const QDate& von, const QDate& bis, const int& day, const QTime& start, const QTime& end);

signals:

public slots:
};

#endif // CZEITNACHWEIS_H
