#ifndef CMONTHLYITEMDELEGATE_H
#define CMONTHLYITEMDELEGATE_H


#include <QTimeEdit>
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QStandardItem>

#include <QMap>


class cMonthlyItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	cMonthlyItemDelegate(const QMap<QString, QString>& code, QObject* parent = nullptr);
	~cMonthlyItemDelegate();

	virtual QWidget*		createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void			setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void			setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:

private:
	QMap<QString, QString>	m_code;
};

#endif // CMONTHLYITEMDELEGATE_H
