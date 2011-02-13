#include "headers/stbmenu.h"
#include "ui_stbmenu.h"

STBMenu::STBMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::STBMenu)
{
    ui->setupUi(this);
}

void STBMenu::init()
{
    channelsMenu = new QMenu();
    addActions();
}

void STBMenu::addActions()
{
    QAction * action = new QAction(QString().fromWCharArray(L"Россия 24"), this);

    QMenu *menu = channelsMenu->addMenu(QString().fromWCharArray(L"Любимые"));
    menu->addAction(action);

    action = new QAction(QString().fromWCharArray(L"Россия 2"), this);
    menu->addAction(action);


    this->ui->channelsButton->setMenu(channelsMenu);
}

STBMenu::~STBMenu()
{
    delete channelsMenu;

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
