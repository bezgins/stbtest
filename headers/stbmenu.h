#ifndef STBMENU_H
#define STBMENU_H

#include <QWidget>

namespace Ui {
    class STBMenu;
}

class STBMenu : public QWidget {
    Q_OBJECT
public:
    STBMenu(QWidget *parent = 0);
    ~STBMenu();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::STBMenu *ui;
};

#endif // STBMENU_H
