#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "cmonthlyview.h"

#include <QMainWindow>
#include <QCloseEvent>


namespace Ui {
class cMainWindow;
}

class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit cMainWindow(QWidget *parent = nullptr);
	~cMainWindow();

private:
	Ui::cMainWindow*	ui;
	cMonthlyView*		m_lpMonthlyView;

protected:
	void				closeEvent(QCloseEvent* event);
};

#endif // CMAINWINDOW_H
