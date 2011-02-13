#ifndef STBMENU_H
#define STBMENU_H

#include <QWidget>
#include <QAction>
#include <QMenu>

namespace Ui {
    class STBMenu;
}

class STBMenu : public QWidget {
    Q_OBJECT
public:
    STBMenu(QWidget *parent = 0);
    ~STBMenu();

    void init();

protected:
    void changeEvent(QEvent *e);

    void addActions();

private:
    Ui::STBMenu *ui;

    QMenu *channelsMenu;
};

#endif // STBMENU_H
