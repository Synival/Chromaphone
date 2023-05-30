// main.cpp
// --------
// Application entry point.

#include "MainWindow.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QApplication>
#ifdef ANDROID
    #include <QJniObject>
    #include <QJniEnvironment>
#endif
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef ANDROID
    // Prevent the devices from going to sleep.
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

    // Create our main window.
    MainWindow window;
    window.show();

    // Run!
    return app.exec();
}
