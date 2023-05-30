// MainWindow.hpp
// --------------
// The main window created at application launch.

#ifndef PITCH_QT_MAIN_WINDOW_HPP_INCLUDED
#define PITCH_QT_MAIN_WINDOW_HPP_INCLUDED

#include <memory>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QMainWindow>
#include <QAbstractButton>
#include <QTimer>
#include <QAudioDevice>
#pragma GCC diagnostic pop

#include "SharedAudioData.hpp"

// Forward declarations
namespace Ui {
    class MainWindow;
}

class QAudioDevice;
class QAudioSource;
class QIODevice;
class QThread;
class AudioWorker;
class LftWorker;

class log_transformer_filter_i;

// The main window created at application launch.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCloseButtonClicked(bool checked);
    void onMagnitudeValueChanged(int value);
    void onRenderTimerTimeout();
    void onRenderAreaClicked();
    void onLftResultsUpdated();
    void onComboBoxInputDeviceChanged(QAudioDevice *device);

signals:
    void readyForNewLftResults();
    void inputDeviceChanged(QAudioDevice device);

private:
    std::unique_ptr<Ui::MainWindow> _ui {};
    std::unique_ptr<QThread> _audioThread {};
    std::unique_ptr<QThread> _lftThread {};
    AudioWorker *_audioWorker = nullptr; // Lifetime managed in _audioThread. Do not manually delete or access after _audioThread has been deleted.
    LftWorker *_lftWorker = nullptr;     // Lifetime managed in _lftThread. Do not manually delete or access after _lftThread has been deleted.
    SharedAudioData _audioData {};

    QTimer _renderTimer {};
    bool _renderNecessary = true;
};

#endif // PITCH_QT_MAIN_WINDOW_HPP_INCLUDED
