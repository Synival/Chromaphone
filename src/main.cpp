#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QApplication>
#ifdef ANDROID
    #include <QJniObject>
    #include <QJniEnvironment>
#endif
#pragma GCC diagnostic pop

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef ANDROID
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([]() {
        QJniObject activity = QNativeInterface::QAndroidApplication::context();
        QJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

        static constexpr int FLAG_KEEP_SCREEN_ON = 128;
        window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);

        // Clear any possible pending exceptions.
        QJniEnvironment env;
        if (env->ExceptionCheck())
            env->ExceptionClear();
    });
#endif

    MainWindow window;
    window.show();

    return app.exec();
}