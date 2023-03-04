#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioSource>

#include "ui_MainWindow.h"
#pragma GCC diagnostic pop

#include "MainWindow.hpp"

constexpr QAudioFormat::SampleFormat SAMPLE_FORMAT = QAudioFormat::SampleFormat::Int16;
using SAMPLE_TYPE = int16_t;
constexpr size_t SAMPLE_TYPE_SIZE = sizeof(SAMPLE_TYPE);
constexpr double SAMPLE_DIVISOR = (1 << ((8 * SAMPLE_TYPE_SIZE) - 1));
constexpr double RENDER_FPS = 60.00;

// Macro for debugging so we can use __func__
#define DEBUG() \
    (qDebug().nospace() << "MainWindow: " << __func__ << "(): ")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(std::make_unique<Ui::MainWindow>()),
    _audioSource(),
    _audioInputStream(),
    _lft(),
    _magnitude(1.00),
    _lftResults(),
    _renderTimer(this)
{
    _ui->setupUi(this);
#ifdef ANDROID
    int cmLeft, cmTop, cmRight, cmBottom;
    _ui->controlPanelGridLayout->getContentsMargins(&cmLeft, &cmTop, &cmRight, &cmBottom);
    _ui->controlPanelGridLayout->setContentsMargins(cmLeft + 10, cmTop, cmRight + 10, cmBottom + 25);
    _ui->controlPanelGridLayout->setSpacing(5);
#endif
    _ui->renderArea->setStats(_lft.getStats());

    // Start getting input and updating our LFT display
    updateInputDevice(_ui->inputSource->getInputDevice());

    // What a silly button! ^_^
    _ui->closeButton->hide();

    // Connect signals to slots
    connect(_ui->renderArea, &RenderArea::clicked, this, &MainWindow::onRenderAreaClicked);
    connect(_ui->closeButton, &QPushButton::clicked, this, &MainWindow::onCloseButtonClicked);
    connect(_ui->inputSource, &AudioInputSourceComboBox::inputDeviceChanged, this, &MainWindow::onInputDeviceChanged);
    connect(_ui->magnitudeSlider, &QSlider::valueChanged, this, &MainWindow::onMagnitudeValueChanged);
    connect(&_renderTimer, &QTimer::timeout, this, &MainWindow::onRenderTimerTimeout);

    _ui->magnitudeSlider->setValue(500);

    // Start the render timer
    _renderTimer.start(1000 / RENDER_FPS);
}

MainWindow::~MainWindow()
{}

void MainWindow::onCloseButtonClicked(bool /*checked*/)
    { QApplication::quit(); }

void MainWindow::onRenderAreaClicked()
{
    if (_ui->controlPanel->isHidden())
        _ui->controlPanel->show();
    else
        _ui->controlPanel->hide();
}

void MainWindow::onAudioInputStreamReadyRead()
{
    const size_t sampleBufSize = _audioSource->bufferSize();
    const double magnitude = _magnitude / SAMPLE_DIVISOR;

    size_t bytesRead;

    do {
        // Attempt to read the entire buffer available.
        char buf[sampleBufSize];
        bytesRead = _audioInputStream->read(buf, sampleBufSize);
        if (bytesRead == 0)
            break;

        // Convert from type 'SAMPLE_TYPE' to normalized 'double'.
        size_t data_len = bytesRead / SAMPLE_TYPE_SIZE;
        auto data_int = reinterpret_cast<SAMPLE_TYPE *>(buf);
        double data_float[data_len];

        for (size_t i = 0; i < data_len; ++i)
            data_float[i] = (double) data_int[i] * magnitude;

        // Push data to the LFT buffer.
        _lft.push(data_float, data_len);

        // Keep reading until we've read less than the full buffer.
    } while (bytesRead == sampleBufSize);

    if (_needsLftUpdate)
        updateLft();
}

void MainWindow::onInputDeviceChanged(QAudioDevice *device)
    { updateInputDevice(device); }

void MainWindow::onMagnitudeValueChanged(int value)
{
    _magnitude = (double) std::pow(2.00, (value - 100) / 100.00);
    value = _magnitude * 100;
    _ui->magnitudeSliderLabel->setText("Sensitivity: " + QString::number(value) + "%");
}

void MainWindow::onRenderTimerTimeout()
{
    _ui->renderArea->update();
    _needsLftUpdate = true;
}

void MainWindow::updateLft()
{
    _lftResults = _lft.getResults();
    _ui->renderArea->setResults(*(_lftResults.get()));
    _needsLftUpdate = false;
}

void MainWindow::updateInputDevice(QAudioDevice *device)
{
    DEBUG() << "audio input device updated";

    // Stop previous audio stream and sources and delete instances
    if (_audioInputStream) {
        DEBUG() << "closing previous audio input stream";
        _audioInputStream->close();
        _audioInputStream = nullptr;
    }
    if (_audioSource) {
        DEBUG() << "closing previous audio source";
        _audioSource->stop();
        _audioSource.reset();
    }

    // Reset data and clear the LFT display
    DEBUG() << "resetting lft signal data";
    _lft.resetSignalData();

    DEBUG() << "updating lft";
    updateLft();

    // If there's no device, we're done.
    if (device == nullptr) {
        DEBUG() << "no audio source";
        return;
    }

    DEBUG() << "audio source: " << device->description() << ", " << device->id();

    // Figure out which format to use
    QAudioFormat format = _lft.getAudioFormat(SAMPLE_FORMAT);

    if (!device->isFormatSupported(format)) {
        DEBUG() << "raw audio format not supported by device";
        return;
    }

    // Create our audio source and start the input stream
    _audioSource = std::make_unique<QAudioSource>(*device, format, this);

    // Use 1/20th of a second worth of data
    _audioSource->setBufferSize(format.sampleRate() / 20 * SAMPLE_TYPE_SIZE);
    _audioInputStream = _audioSource->start();

    // Listen!
    connect(_audioInputStream, &QIODevice::readyRead, this, &MainWindow::onAudioInputStreamReadyRead);
}