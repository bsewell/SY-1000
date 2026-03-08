#ifndef CONSOLETOOLBAR_H
#define CONSOLETOOLBAR_H

#include <QWidget>

namespace Ui {
class ConsoleToolBar;
}

class ConsoleToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ConsoleToolBar(QWidget *parent = nullptr);
    ~ConsoleToolBar();

signals:
    void openClicked();
    void saveClicked();
    void uploadClicked();
    void settingsClicked();
    void helpClicked();

private:
    Ui::ConsoleToolBar *ui;
};

#endif // CONSOLETOOLBAR_H
