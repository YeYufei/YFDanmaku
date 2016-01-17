#include "BanListWidget.h"

BanListWidget::BanListWidget(QWidget *parent)                                    //可能因为数据传递原因要废弃的类
    :QListWidget(parent)
{

}
void BanListWidget::deleteListItem()
{
    QMessageBox box;
    box.setWindowTitle("警告");
    box.setIcon(QMessageBox::Warning);
    box.setText("确定要删除此屏蔽词吗？");
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setFixedSize(187,123);
    if(box.exec()==QMessageBox::Yes){
        QListWidgetItem * list1=this->currentItem();
        this->takeItem(this->currentRow ());
        if(list1)
            delete list1;
    }

}

void BanListWidget::clearAll()
{
    QMessageBox box;
    box.setWindowTitle("警告");
    box.setIcon(QMessageBox::Warning);
    box.setText("确定要删除全部屏蔽词吗？");
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setFixedSize(187,123);
    if(box.exec()==QMessageBox::Yes){
        this->clear();
    }
}

void BanListWidget::contextMenuEvent(QContextMenuEvent *)
{

    QCursor cur=this->cursor();

    QAction * takeItem=new QAction("删除",this);
    QAction * clearall=new QAction("删除全部",this);
    QMenu * listMenu=new QMenu();

    connect(takeItem,SIGNAL(triggered()),this,SLOT(deleteListItem()));
    connect(clearall,SIGNAL(triggered()),this,SLOT(clearAll()));

    listMenu->addAction(takeItem);
    listMenu->addAction(clearall);

    listMenu->setStyleSheet("QMenu{"
                            "padding:5px;"
                                     "background:white;"
                            "border:1px solid gray;"
                             "}"
                            " QMenu::item{"
                            "padding:0px 40px 0px 30px;"
                            "height:25px;"
                             "}"
                             "QMenu::item:selected:enabled{"
                             "       background:rgb(90,182,253);"
                             "       color:white;"
                             "}"
                             " QMenu::item:selected:!enabled{"
                             "       background:transparent;"
                             "}"
                             "QMenu::separator{"
                                    "height:1px;"
                                    "background:lightgray;"
                            "margin:5px 0px 5px 0px;"
                             "}");

    listMenu->exec(cur.pos());
    listMenu->deleteLater();

}
