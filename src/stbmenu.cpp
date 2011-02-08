#include "headers/stbmenu.h"
#include "ui_stbmenu.h"

STBMenu::STBMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::STBMenu)
{
    ui->setupUi(this);
}

STBMenu::~STBMenu()
{
    delete ui;
}

void STBMenu::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
