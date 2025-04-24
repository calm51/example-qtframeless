#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "./dialog1.h"

#include "qtframeless/qtframeless.h"
#include "qtframeless/qtframeless_global.h"
#include "qtframeless/titlebar/titlebarlikeandroid.h"
#include "qtframeless/titlebar/titlebarlikewindows10.h"
#include "qtframeless/utils.h"

#if defined Q_OS_ANDROID
#include "QtAndroid"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroidExtras>

#include <qtandroidcls/qtandroidcls.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

#ifdef Q_OS_ANDROID
    QtAndroidCls *qac = QtAndroidCls::instance();
    connect(qac, &QtAndroidCls::statusbarHeightChanged, this, [=](const qint32 &height) {
        qDebug() << "heightheightheight" << height;
        if (fl && height > 4) {
            fl->titlebar_MinimumHeight = height;
            if (fl->titlebar_widget) {
                fl->titlebar_widget->setFixedHeight(fl->titlebar_MinimumHeight);
            }
            fl->load();
        }
    });
#endif

    connect(ui->pushButton_2, &QPushButton::clicked, this, [=]() {
        QFL::__global__ &fg = QFL::G();
        if (fg.currentTheme == QFL::Theme::Light) {
            fg.swithTheme(QFL::Theme::Dark);
        } else if (fg.currentTheme == QFL::Theme::Dark) {
            fg.swithTheme(QFL::Theme::White);
        } else if (fg.currentTheme == QFL::Theme::White) {
            fg.swithTheme(QFL::Theme::Gray);
        } else if (fg.currentTheme == QFL::Theme::Gray) {
            fg.swithTheme(QFL::Theme::Light);
        }
    });

    connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
        Dialog1 *dialog1 = new Dialog1(this);
        dialog1->setWindowTitle(tr("new Dialog1"));
        dialog1->setWindowIcon(QIcon(":/resource/icon/main.ico"));

        QFL::__global__ &fg = QFL::G();
#ifndef Q_OS_ANDROID
        auto fl_tb = QFL::template_windows10<QFL::TB::TitlebarLikeWindows10>(*dialog1, 24, 40);
        fl_tb.first->allow_showMaximized = false;
        fl_tb.first->allow_showMinimized = false;
        fl_tb.second->hide_button = {true, true, false};
        fl_tb.second->set_title(dialog1->windowTitle()); // fl_tb.second->setWindowTitle(dialog1->windowTitle());
        fl_tb.second->loadthemesetting();
        if (fg.currentTheme == QFL::Theme::Light) {
            fl_tb.second->colors[9] = QColor(219, 170, 240);
        } else if (fg.currentTheme == QFL::Theme::Dark) {
            fl_tb.second->colors[9] = QColor(82, 54, 95);
        }
        fl_tb.second->load();
#else
        QtAndroidCls *qac = QtAndroidCls::instance();
        auto fl_tb = QFL::template_windows10<QFL::TB::TitlebarLikeAndroid>(*dialog1, qac->get_statusbar_qwiget_height());
        if (fg.currentTheme == QFL::Theme::Light) {
            fl_tb.second->colors[1] = QColor(240, 170, 170);
        } else if (fg.currentTheme == QFL::Theme::Dark) {
            fl_tb.second->colors[1] = QColor(72, 49, 49);
        }
        fl_tb.second->load();
#endif

        //        Frameless::Frameless *fl = new Frameless::Frameless(dialog1);
        //#ifdef Q_OS_ANDROID
        //        QtAndroidCls *qac = QtAndroidCls::instance();
        //        fl->titlebar_MinimumHeight = qac->get_statusbar_qwiget_height();
        //#else
        //        fl->titlebar_MinimumHeight = 24;
        //        fl->resize_rect_in = 0;
        //        fl->allow_showMaximized = false;
        //        fl->allow_showMinimized = false;
        //#endif
        //        fl->init(*dialog1); fl->loadthemesetting(); dialog1->fl = fl;
        //        Frameless::__global__ & fg = Frameless::G();
        //#ifdef Q_OS_ANDROID
        //        Frameless::TitlebarLikeAndroid *titlebar = new Frameless::TitlebarLikeAndroid(fl, dialog1);
        //        titlebar->loadthemesetting();
        //        titlebar->setFixedHeight(fl->titlebar_MinimumHeight);
        //        fl->set_titlebar(*titlebar);
        //        if (fg.currentTheme==Frameless::Theme::Light){
        //            titlebar->colors[1] = QColor(240,170,170);
        //        }else if (fg.currentTheme==Frameless::Theme::Dark){
        //            titlebar->colors[1] = QColor(72,49,49);
        //        }titlebar->load();
        //#else
        //        Frameless::TitlebarLikeWindows10 *titlebar = new Frameless::TitlebarLikeWindows10(fl, 40, dialog1);
        //        titlebar->hide_button = {true,true,false};
        //        titlebar->loadthemesetting();
        //        // titlebar.setFixedHeight(fl.titlebar_MinimumHeight);
        //        fl->set_titlebar(*titlebar);
        //        fl->titlebar_widget->setWindowTitle(dialog1->windowTitle());
        //        fl->titlebar_widget->setWindowIcon(dialog1->windowIcon());
        //        if (fg.currentTheme==Frameless::Theme::Light){
        //            titlebar->colors[9] = QColor(219,170,240);
        //        }else if (fg.currentTheme==Frameless::Theme::Dark){
        //            titlebar->colors[9] = QColor(82,54,95);
        //        }titlebar->load();
        //#endif


        // W_dialog_buttonboxs<W_contacts*> *w_dialog_buttonboxs = new W_dialog_buttonboxs<W_contacts*>(w_contacts,w_contacts);
        // w_dialog_buttonboxs->setTitle(w_contacts->windowTitle());
#ifdef Q_OS_ANDROID
        // fl->set_menubar(*w_dialog_buttonboxs);
        dialog1->showFullScreen();
#else
        // fl->set_bottombar(*w_dialog_buttonboxs);
        // w_dialog_buttonboxs->setVisible(false);
#endif
        // dialog1->show();
        if (dialog1->exec() == QDialog::Accepted) {
        }
        delete dialog1;
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *o, QEvent *e) {
    return QMainWindow::eventFilter(o, e);
}
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    if (fl) {
        return fl->nativeEvent(eventType, message, result);
    }
    return false;
}
void MainWindow::showEvent(QShowEvent *event) {
    return QMainWindow::showEvent(event);
}
void MainWindow::loadthemesetting() {
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

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    qApp->exit(0);
}
