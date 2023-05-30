// MainWindow.cpp
// --------------
// The main window created at application launch.

#include "MainWindow.hpp"

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioSource>
#include <QThread>
#include <QMutexLocker>
#include "ui_MainWindow.h"
#pragma GCC diagnostic pop

#include "AudioWorker.hpp"
#include "LftWorker.hpp"

constexpr double RENDER_FPS = 60.00;

// Macro for debugging so we can use __func__
#define DEBUG() \
    (qDebug().nospace() << "MainWindow: " << __func__ << "(): ")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(std::make_unique<Ui::MainWindow>()),
    _renderTimer(this)
{
    _ui->setupUi(this);
#ifdef ANDROID
    int cmLeft, cmTop, cmRight, cmBottom;
    _ui->controlPanelGridLayout->getContentsMargins(&cmLeft, &cmTop, &cmRight, &cmBottom);
    _ui->controlPanelGridLayout->setContentsMargins(cmLeft + 10, cmTop, cmRight + 10, cmBottom + 25);
    _ui->controlPanelGridLayout->setSpacing(5);
#endif
    _ui->renderArea->setStats(_audioData.getLftStats());

    // Setup audio input thread
    _audioThread = std::make_unique<QThread>(this);
    QThread *audioThread = _audioThread.get();
    _audioWorker = new AudioWorker(_audioData);
    _audioWorker->moveToThread(audioThread);
    connect(audioThread, &QThread::finished, _audioWorker, &QObject::deleteLater);

    // Setup LFT processing thread
    _lftThread = std::make_unique<QThread>(this);
    QThread *lftThread = _lftThread.get();
    _lftWorker = new LftWorker(_audioData);
    _lftWorker->moveToThread(lftThread);
    connect(lftThread, &QThread::finished, _lftWorker, &QObject::deleteLater);

    // Connect events between threads
    connect(_audioWorker, &AudioWorker::audioRead, _lftWorker, &LftWorker::onAudioRead);
    connect(_lftWorker, &LftWorker::lftResultsUpdated, this, &MainWindow::onLftResultsUpdated);
    connect(this, &MainWindow::readyForNewLftResults, _lftWorker, &LftWorker::onReadyForNewLftResults);

    // Threads connected; start them
    audioThread->start();
    lftThread->start();

    // Start getting input and updating our LFT display
    onComboBoxInputDeviceChanged(_ui->inputSource->getInputDevice());

    // What a silly button! ^_^
    _ui->closeButton->hide();

    // Connect signals to slots
    connect(_ui->renderArea, &RenderArea::clicked, this, &MainWindow::onRenderAreaClicked);
    connect(_ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);
    connect(_ui->inputSource, &AudioInputSourceComboBox::inputDeviceChanged, this, &MainWindow::onComboBoxInputDeviceChanged);
    connect(this, &MainWindow::inputDeviceChanged, _audioWorker, &AudioWorker::onInputDeviceChanged);
    connect(_ui->magnitudeSlider, &QSlider::valueChanged, this, &MainWindow::onMagnitudeValueChanged);
    connect(&_renderTimer, &QTimer::timeout, this, &MainWindow::onRenderTimerTimeout);

    _ui->magnitudeSlider->setValue(500);
    emit _ui->inputSource->inputDeviceChanged(_ui->inputSource->getInputDevice());

    // Start the render timer
    _renderTimer.start(1000 / RENDER_FPS);
}

MainWindow::~MainWindow()
{
    std::array<QThread*, 2> threads {
        _audioThread.get(),
        _lftThread.get()
    };

    for (auto thread : threads) {
        if (thread && thread->isRunning()) {
            thread->requestInterruption();
            thread->quit();
        }
    }

    for (auto thread : threads)
        if (thread && thread->isRunning())
            thread->wait();
}

void MainWindow::onCloseButtonClicked(bool /*checked*/)
    { QApplication::quit(); }

void MainWindow::onRenderAreaClicked()
{
    if (_ui->controlPanel->isHidden())
        _ui->controlPanel->show();
    else
        _ui->controlPanel->hide();
}

void MainWindow::onMagnitudeValueChanged(int value)
{
    double newMagnitude = (double) std::pow(2.00, (value - 100) / 100.00);
    _audioData.setMagnitude(newMagnitude);
    _ui->magnitudeSliderLabel->setText("Sensitivity: " + QString::number(static_cast<int>(newMagnitude * 100.00)) + "%");
}

void MainWindow::onRenderTimerTimeout()
{
    if (!_renderNecessary)
        return;

    emit readyForNewLftResults();
    _ui->renderArea->update();
    _renderNecessary = false;
}

void MainWindow::onLftResultsUpdated()
{
    _ui->renderArea->setResults(_audioData.getLftResults());
    _renderNecessary = true;
}

void MainWindow::onComboBoxInputDeviceChanged(QAudioDevice *device)
{
    if (device)
        emit inputDeviceChanged(QAudioDevice { *device });
    else
        emit inputDeviceChanged(QAudioDevice {});
}