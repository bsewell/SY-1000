#include "consoletoolbar.h"
#include "ui_consoletoolbar.h"
#include <QPushButton>

ConsoleToolBar::ConsoleToolBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConsoleToolBar)
{
    ui->setupUi(this);
    connect(ui->btnOpen,     &QPushButton::clicked, this, &ConsoleToolBar::openClicked);
    connect(ui->btnSave,     &QPushButton::clicked, this, &ConsoleToolBar::saveClicked);
    connect(ui->btnUpload,   &QPushButton::clicked, this, &ConsoleToolBar::uploadClicked);
    connect(ui->btnSettings, &QPushButton::clicked, this, &ConsoleToolBar::settingsClicked);
    connect(ui->btnHelp,     &QPushButton::clicked, this, &ConsoleToolBar::helpClicked);
}

ConsoleToolBar::~ConsoleToolBar()
{
    delete ui;
}
