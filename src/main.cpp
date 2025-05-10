#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QPair>


#include "./maintitlebarbutton.h"


#if defined Q_OS_ANDROID
#include "QtAndroid"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroidExtras>

#include <qtandroidcls/qtandroidcls.h>
#endif

#include "qtframeless/qtframeless.h"
#include "qtframeless/qtframeless_global.h"
#include "qtframeless/titlebar/titlebarlikeandroid.h"
#include "qtframeless/titlebar/titlebarlikewindows10.h"
#include "qtframeless/utils.h"


#include <qtturnblack/qtturnblack.h>
#include <QDebug>
#include <QStyleFactory>


void loadthemesetting() {
    QFL::__global__ &fg = QFL::G();
#ifdef Q_OS_ANDROID
    QtAndroidCls *qac = QtAndroidCls::instance();
    if (fg.currentTheme == QFL::Theme::Light || fg.currentTheme == QFL::Theme::White) {
        qac->to_statusbar_text_black();
    } else {
        qac->to_statusbar_text_white();
    }
#endif

#ifdef Q_OS_ANDROID
    int _fontsize = 14;
#elif defined Q_OS_WIN
    int _fontsize = 9;
#elif defined Q_OS_LINUX
    int _fontsize = 9;
#endif


    QString _t(R"(
*{font:字体大小pt \"微软雅黑\";outline:0px;}
)");
    _t = _t.replace(QString("字体大小"), QString::number(_fontsize));

#ifdef Q_OS_LINUX
    _t = _t.replace(QString("微软雅黑"), "Ubuntu");
#endif

    Qtturnblack::qtb qtb;
    if (fg.currentTheme == QFL::Theme::Gray) { // 暗色
        qtb = Qtturnblack::get(Qtturnblack::Color::Gray);
    } else if (fg.currentTheme == QFL::Theme::Light) {
        qtb = Qtturnblack::get(Qtturnblack::Color::Light);
    }else if (fg.currentTheme == QFL::Theme::White) {
        qtb = Qtturnblack::get(Qtturnblack::Color::Default);
    }
    _t += qtb.styleSheet;

    _t += R"(
QPushButton {
    background-color: transparent;
    color: palette(button-text);
    padding: 4px 16px;
    margin: 2px;
    border: none;
    border-radius: 0px;
}
QPushButton:hover {
    background-color: palette(button);
}
QPushButton:pressed {
    background-color: palette(midlight);
}
QPushButton:disabled {
    background-color: transparent;
    color: palette(button-text-x);
}
)"; // flat button

    qApp->setPalette(qtb.palette);
    qApp->setStyleSheet(_t);
}

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include "qx11info_x11.h"
#endif
#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
#include <X11/Xlib.h>

#include <X11/Xutil.h>
#include <QX11Info>

#endif


int main(int argc, char *argv[]) {
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    a.setAttribute(Qt::ApplicationAttribute::AA_UseHighDpiPixmaps);
    a.setQuitOnLastWindowClosed(false);
    a.setStyle("Fusion");
#if defined Q_OS_ANDROID
    QtAndroidCls *qac = QtAndroidCls::instance("ca/calm/framelessdemo/framelessdemo");
#endif
    a.setWindowIcon(QIcon(":/resource/icon/main.ico"));

    QFL::__global__ &fg = QFL::G();
    // fg.theme_config[QFL::Theme::White].round4 = {28,28,0,0};
    // fg.theme_config[QFL::Theme::Light].round4 = {15, 15, 0, 0};
    //    fg.theme_config[QFL::Theme::Dark].round4 = {0,0,0,0};
    //    fg.theme_config[QFL::Theme::Gray].round4 = {0,0,0,0};
    fg.swithTheme(QFL::Theme::White);


    MainWindow w;
    w.setWindowTitle("framelessdemo");
    w.connect(&fg.SIGNAL, &QFL::__signal__::themeChanged, &w, [&]() { loadthemesetting(); });
    loadthemesetting();
#if defined Q_OS_ANDROID
    w.connect(qac, &QtAndroidCls::statusbarHeightChanged, &w, [=](const qint32 &height) {});
#endif

#ifndef Q_OS_ANDROID
    auto qfl_tb = QFL::template_windows10<QFL::TB::TitlebarLikeWindows10>(w, 28, 44);
    Maintitlebarbutton *maintitlebarbutton = new Maintitlebarbutton(&w);
    qfl_tb.second->w3layout()->addWidget(maintitlebarbutton);
    qfl_tb.second->set_title("v1.0");
    qfl_tb.second->set_title_enable(false);
    qfl_tb.first->rubberBand_enable = {true,false,false};
#else
    auto qfl_tb = QFL::template_windows10<QFL::TB::TitlebarLikeAndroid>(w, qac->get_statusbar_qwiget_height());
    w.show();
    // W_startscreen *w_ss = new W_startscreen(&w);
    // w_ss->showFullScreen();
    QTimer::singleShot(50, qac, [&]() {
        qac->to_statusbar_text_white();
        QtAndroid::hideSplashScreen(100);
        // w_ss->exec();
        if (fg.currentTheme == QFL::Theme::Light || fg.currentTheme == QFL::Theme::White) {
            qac->to_statusbar_text_black();
        } else {
            qac->to_statusbar_text_white();
        }
    });
    qac->up_statusbar_height();
#endif
    a.installNativeEventFilter(qfl_tb.first);


    //    Frameless::Frameless fl(&w);
    //    a.installNativeEventFilter(&fl);
    //#ifndef Q_OS_ANDROID
    //    fl.titlebar_MinimumHeight = 28; // 24-40 28-44 28-46
    //    fl.init(w); fl.loadthemesetting(); w.fl = &fl;
    //    Frameless::TitlebarLikeWindows10 titlebar(&fl,44,&w);
    //    titlebar.loadthemesetting();
    //    fl.set_titlebar(titlebar);
    //    fl.titlebar_widget->setWindowIcon(w.windowIcon());
    //    Maintitlebarbutton* maintitlebarbutton = new Maintitlebarbutton(&w);
    //    titlebar.w3layout()->addWidget(maintitlebarbutton);
    //    titlebar.set_title("v1.0");
    //    titlebar.set_title_enable(false);
    //#else
    //    fl.titlebar_MinimumHeight = qac->get_statusbar_qwiget_height();
    //    fl.init(w); fl.loadthemesetting(); w.fl = &fl;
    //    Frameless::TitlebarLikeAndroid titlebar(&fl,&w);
    //    titlebar.loadthemesetting();
    //    titlebar.setFixedHeight(fl.titlebar_MinimumHeight);
    //    fl.set_titlebar(titlebar);
    //    w.show();
    //    // W_startscreen *w_ss = new W_startscreen(&w);
    //    // w_ss->showFullScreen();
    //    QTimer::singleShot(50,qac,[&](){
    //        qac->to_statusbar_text_white();
    //        QtAndroid::hideSplashScreen(100);
    //        // w_ss->exec();
    //        if (fg.currentTheme==Frameless::Theme::Light){
    //            qac->to_statusbar_text_black();
    //        }else{
    //            qac->to_statusbar_text_white();
    //        }
    //    });
    //    qac->up_statusbar_height();
    //#endif


    w.show();
#ifndef Q_OS_ANDROID
    // w.trayicon->show();
#else
#endif

    return a.exec();
}
