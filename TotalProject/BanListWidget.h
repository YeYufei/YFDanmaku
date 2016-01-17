#ifndef BANLISTWIDGET_H
#define BANLISTWIDGET_H
#include <QListWidget>
#include <QAction>
#include <QMenu>
#include <QMessageBox>

class BanListWidget:public QListWidget
{
    Q_OBJECT

public:
    BanListWidget(QWidget * parent=0);

protected:
    void contextMenuEvent(QContextMenuEvent * event);

private slots:
    void deleteListItem();

    void clearAll();

};
#endif // BANLISTWIDGET_H
