#include "cmonthlyitemdelegate.h"
#include "common.h"


cMonthlyItemDelegate::cMonthlyItemDelegate(const QMap<QString, QString>& code, QObject* parent) :
	QStyledItemDelegate(parent),
	m_code(code)
{
}

cMonthlyItemDelegate::~cMonthlyItemDelegate()
{
}

QWidget* cMonthlyItemDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	switch(index.column())
	{
	case COL_COME1:
	case COL_GO1:
	case COL_COME2:
	case COL_GO2:
	case COL_COME3:
	case COL_GO3:
	case COL_COME4:
	case COL_GO4:
	case COL_COME5:
	case COL_GO5:
	{
		QTimeEdit*	lpTimeEdit	= new QTimeEdit(parent);
		lpTimeEdit->setDisplayFormat("HH:mm:ss");
		return(lpTimeEdit);
	}
	case COL_CODE:
	{
		if(index.data(Qt::UserRole+1).toBool())
			return(nullptr);

		QComboBox*	lpComboBox	= new QComboBox(parent);

		for(QMap<QString, QString>::const_iterator i = m_code.constBegin();i != m_code.constEnd();i++)
			lpComboBox->addItem(i.value());
		return(lpComboBox);
	}
	case COL_INFORMATION:
		return(QStyledItemDelegate::createEditor(parent, option, index));
	default:
		return(nullptr);
	}
}

void cMonthlyItemDelegate::setEditorData ( QWidget *editor, const QModelIndex &index ) const
{
	switch(index.column())
	{
	case COL_COME1:
	case COL_GO1:
	case COL_COME2:
	case COL_GO2:
	case COL_COME3:
	case COL_GO3:
	case COL_COME4:
	case COL_GO4:
	case COL_COME5:
	case COL_GO5:
	{
		QTimeEdit*	lpTimeEdit	= qobject_cast<QTimeEdit*>(editor);
		lpTimeEdit->setTime(QTime::fromString(index.data(Qt::EditRole).toString()));
		break;
	}
	case COL_CODE:
	{
		QComboBox*	lpComboBox	= qobject_cast<QComboBox*>(editor);
		QString	currentText	= index.data(Qt::EditRole).toString();
		int		cbIndex		= lpComboBox->findText(m_code.value(currentText));

		if(cbIndex >= 0)
			lpComboBox->setCurrentIndex(cbIndex);

		break;
	}
	case COL_INFORMATION:
		QStyledItemDelegate::setEditorData(editor, index);
		break;
	default:
		break;
	}
}

void cMonthlyItemDelegate::setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
	switch(index.column())
	{
	case COL_COME1:
	case COL_GO1:
	case COL_COME2:
	case COL_GO2:
	case COL_COME3:
	case COL_GO3:
	case COL_COME4:
	case COL_GO4:
	case COL_COME5:
	case COL_GO5:
	{
		QTimeEdit*	lpTimeEdit	= qobject_cast<QTimeEdit*>(editor);
		if(!lpTimeEdit->time().hour() &&
				!lpTimeEdit->time().minute() &&
				!lpTimeEdit->time().second() &&
				index.column() != COL_BREAK)
			model->setData(index, "", Qt::EditRole);
		else
			model->setData(index, lpTimeEdit->time().toString(), Qt::EditRole);

		emit timeChanged(index.row()+1, index.column(), lpTimeEdit->time());

		break;
	}
	case COL_CODE:
	{
		QComboBox*	lpComboBox	= qobject_cast<QComboBox*>(editor);
		model->setData(index, m_code.key(lpComboBox->currentText()), Qt::EditRole);

		emit textChanged(index.row()+1, index.column(), lpComboBox->currentText());

		break;
	}
	case COL_INFORMATION:
		QStyledItemDelegate::setModelData(editor, model, index);

		emit textChanged(index.row()+1, index.column(), model->data(index, Qt::EditRole).toString());

		break;
	default:
		break;
	}
}
