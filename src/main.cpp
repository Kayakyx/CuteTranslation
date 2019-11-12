#include <QApplication>
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QDir>
#include <QVector>

#include "floatbutton.h"
#include "xdotool.h"
#include "systemtrayicon.h"
#include "configtool.h"
#include "configwindow.h"
#include "shortcut.h"

Xdotool xdotool;
ConfigTool configTool;

int main(int argc, char *argv[])
{
    // TODO analyse picker's text language type

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 支持HighDPI缩放
    QApplication a(argc, argv);

    QVector<QString> depends;
    depends.push_back("BaiduTranslate.py");
    depends.push_back("translate_demo.py");
    depends.push_back("BaiduOCR.py");
    depends.push_back("interpret_js_2.html");


    QDir appDir(QCoreApplication::applicationDirPath());
    for (auto file : depends)
    {
        if (!appDir.exists(file))
        {
            qDebug() << "file is missing: " << file;
            return 1;
        }
    }

    if (!QDir::home().exists(".config/CuteTranslation/config.ini"))
    {
        qDebug() << "file is missing: " << "config.ini";
        return 1;
    }


    // 获取屏幕可用的大小
    xdotool.screenWidth = QGuiApplication::primaryScreen()->availableSize().width();
    xdotool.screenHeight = QGuiApplication::primaryScreen()->availableSize().height();
    picker = new Picker();
    ConfigWindow cw;
    MainWindow w;
    w.setGeometry(800, 200, 400, 300);

    FloatButton f;

    SystemTrayIcon tray;
    tray.show();

    ShortCut shortcut;

    // cw.show();

    QObject::connect(picker, &Picker::wordsPicked, &f, &FloatButton::onWordPicked);

    QObject::connect(&f, &FloatButton::floatButtonPressed, &w, &MainWindow::onFloatButtonPressed);

    QObject::connect(&tray.config_action, &QAction::triggered, &cw, &ConfigWindow::show );

    QObject::connect(&shortcut, &ShortCut::OCRCompleted, &w, &MainWindow::onOCRCompleted);

    // 全局鼠标监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &f, &FloatButton::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonPress, picker, &Picker::buttonPressed, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, &f, &FloatButton::onMouseButtonReleased, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::buttonRelease, picker, &Picker::buttonReleased, Qt::QueuedConnection);

    QObject::connect(&xdotool.eventMonitor, &EventMonitor::mouseWheel, &f, &FloatButton::hide, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::mouseWheel, &w, &MainWindow::onMouseButtonPressed, Qt::QueuedConnection);

    // 全局键盘监听
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &f, &FloatButton::onKeyPressed, Qt::QueuedConnection);
    QObject::connect(&xdotool.eventMonitor, &EventMonitor::keyPress, &shortcut, &ShortCut::onKeyPressed, Qt::QueuedConnection);

    xdotool.eventMonitor.start();
    return a.exec();
}
