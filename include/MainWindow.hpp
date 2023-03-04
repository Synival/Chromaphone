#ifndef PITCH_QT_MAINWINDOW_HPP_INCLUDED
#define PITCH_QT_MAINWINDOW_HPP_INCLUDED

#include <memory>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QMainWindow>
#include <QAbstractButton>
#include <QTimer>
#pragma GCC diagnostic pop

#include "Lft.hpp"

namespace Ui {
    class MainWindow;
}

class QAudioDevice;
class QAudioSource;
class QIODevice;

class log_transformer_filter_i;

template<typename T>
class transform_results;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCloseButtonClicked(bool checked);
    void onAudioInputStreamReadyRead();
    void onInputDeviceChanged(QAudioDevice *device);
    void onMagnitudeValueChanged(int value);
    void onRenderTimerTimeout();
    void onRenderAreaClicked();

private:
    void updateLft();
    void updateInputDevice(QAudioDevice *device);

    std::unique_ptr<Ui::MainWindow> _ui;

    std::unique_ptr<QAudioSource> _audioSource;
    QIODevice *_audioInputStream;

    Lft _lft;
    double _magnitude;

    std::unique_ptr<transform_results<double>> _lftResults;
    QTimer _renderTimer;

    mutable bool _needsLftUpdate = true;
};

#endif // PITCH_QT_MAINWINDOW_HPP_INCLUDED