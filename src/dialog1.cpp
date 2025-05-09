#include "dialog1.h"
#include "ui_dialog1.h"

#include <qtframeless/utils.h>

#if defined Q_OS_ANDROID
#include "QtAndroid"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroidExtras>

#include <qtandroidcls/qtandroidcls.h>
#endif


Dialog1::Dialog1(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog1) {
    ui->setupUi(this);

#ifdef Q_OS_ANDROID
    QtAndroidCls *qac = QtAndroidCls::instance();
    connect(qac, &QtAndroidCls::statusbarHeightChanged, this, [=](const qint32 &height) {
        if (qfl && height > 4) {
            qfl->titlebar_MinimumHeight = height;
            if (qfl->titlebar_widget) {
                qfl->titlebar_widget->setFixedHeight(qfl->titlebar_MinimumHeight);
            }
            qfl->load();
        }
    });
#endif

    QFL::__global__ &fg = QFL::G();
    connect(&fg.SIGNAL, &QFL::__signal__::themeChanged, this, [=]() { loadthemesetting(); });
}


Dialog1::~Dialog1() {
    delete ui;
}

bool Dialog1::eventFilter(QObject *o, QEvent *e) {
    return QDialog::eventFilter(o, e);
}
bool Dialog1::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    if (qfl) {
        return qfl->nativeEvent(eventType, message, result);
    }
    return false;
}
void Dialog1::showEvent(QShowEvent *event) {
    QFL::template_dialog_tocenter(this, this->parentWidget());
    return QDialog::showEvent(event);
}
void Dialog1::loadthemesetting() {
    QFL::__global__ &fg = QFL::G();
    if (fg.currentTheme == QFL::Theme::Light) {
        ui->widget->setStyleSheet(R"(
.QWidget,.QScrollArea{background:transparent;}

)");
    } else if (fg.currentTheme == QFL::Theme::Dark) {
        ui->widget->setStyleSheet(QString(R"(
.QWidget,.QScrollArea{background:transparent;}

)"));
    }
}
void Dialog1::accept_() {
    //    save();
    this->accept();
}
void Dialog1::reject_() {
    this->reject();
}
