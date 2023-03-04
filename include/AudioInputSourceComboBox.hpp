// AudioInputSourceSelection.hpp
// -----------------------------
// QComboBox-derived class for selecting an audio input source.

#ifndef PITCH_QT_LOG_AUDIOINPUTSOURCECOMBOBOX_HPP_INCLUDED
#define PITCH_QT_LOG_AUDIOINPUTSOURCECOMBOBOX_HPP_INCLUDED

#include <memory>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QComboBox>
#include <QMediaDevices>
#pragma GCC diagnostic pop

// Forward declarations
class QAudioDevice;

// Different types of selection items in our selection combo box
enum class AudioInputSourceSelectionType { None, Default, Explicit };

// Info used to determine a selected QAudioDevice
class AudioInputSourceSelection {
public:
    // Don't actually use this! Just required for QVariant
    AudioInputSourceSelection();

    // Constructor for types 'None' or 'Default'
    AudioInputSourceSelection(AudioInputSourceSelectionType type);

    // Constructor for type 'Explicit'
    AudioInputSourceSelection(const QAudioDevice &device);

    // Copy constructor
    AudioInputSourceSelection(const AudioInputSourceSelection &selection);

    void operator=(const AudioInputSourceSelection &other);
    bool operator==(const AudioInputSourceSelection &other);

    // Returns the current QAudioDevice based on the combo box selection.
    // Can return 'nullptr' to indicate no device
    QAudioDevice *getAudioDevice(QList<QAudioDevice> &devices) const;

private:
    AudioInputSourceSelectionType _selectionType;
    QByteArray _id;
};

Q_DECLARE_METATYPE(AudioInputSourceSelection)

// A combo box that auto-populates with the current input devices and updates accordingly.
// Sends a signal when the QAudioDevice has changed.
class AudioInputSourceComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit AudioInputSourceComboBox(QWidget *parent = nullptr);
    ~AudioInputSourceComboBox();

    // Returns the currently selected audio input source.
    // Can return 'nullptr' if no device is selected or available.
    QAudioDevice *getInputDevice();

signals:
    // Sent when the result of getInputDevice() changes.
    void inputDeviceChanged(QAudioDevice *device);

private slots:
    void onCurrentIndexChanged(int index);
    void onAudioInputsChanged();

private:
    // Updates the internal selection, updates the combo box, and sends a signal when necessary
    void setInputDevice(AudioInputSourceSelection selection);

    // Clears and repopulates the list of available devices
    void refreshItems();

    // Selects the matching combo box item for the internally-tracked selection
    void selectCurrentInputDevice();

    // Instance of QMediaDevice for listening to signals
    std::unique_ptr<QMediaDevices> _mediaDevices;

    // List of current available input devices, updated on QMediaDevices::audioInputsChanged
    QList<QAudioDevice> _inputDevices;

    // The currently selected data from the combo box
    AudioInputSourceSelection _selectedDevice;

    // The device id of the current selection. Will be an empty QByteArray if the QAudioDevice is 'nullptr'.
    QByteArray _selectedDeviceId;

    // When greater than zero, refreshItems() is in progress
    int _refreshingItems = 0;
};

#endif // PITCH_QT_LOG_AUDIOINPUTSOURCECOMBOBOX_HPP_INCLUDED
