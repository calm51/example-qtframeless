#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>

#include "qtframeless/qtframeless.h"

namespace Ui {
class Dialog1;
}

class Dialog1 : public QDialog {
    Q_OBJECT

  public:
    explicit Dialog1(QWidget *parent = nullptr);
    ~Dialog1();

    QFL::QtFrameless *qfl = nullptr;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    bool eventFilter(QObject *o, QEvent *e);
    void showEvent(QShowEvent *event);
    void loadthemesetting();
    void accept_();
    void reject_();

  private:
    Ui::Dialog1 *ui;
};

#endif // DIALOG1_H
