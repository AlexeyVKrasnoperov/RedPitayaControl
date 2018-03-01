#ifndef LEDCONTROLGROUPBOX_H
#define LEDCONTROLGROUPBOX_H

#include <QGroupBox>
#include <QButtonGroup>

namespace Ui {
class LedControlGroupBox;
}

class RPClient;

class LedControlGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit LedControlGroupBox(QWidget *parent = 0);
    ~LedControlGroupBox();
    void setRPClient(RPClient *client = 0);
protected slots:
    void ledButtonClickedSlot(int id);

    void responseSlot(const QString & cmd, const QByteArray & response);

private:
    Ui::LedControlGroupBox *ui;
    QButtonGroup ledButtons;
    RPClient *rpClient;
};

#endif // LEDCONTROLGROUPBOX_H
