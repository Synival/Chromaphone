// AudioInputSourceSelection.cpp
// -----------------------------
// QComboBox-derived class for selecting an audio input source.

#include "AudioInputSourceComboBox.hpp"

#include <stdexcept>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QDebug>
#pragma GCC diagnostic pop

#include "Utils.hpp"

// NOTE: change these if the combo box order changes!!
//constexpr int DEVICE_INDEX_NONE = 1;
constexpr int DEVICE_INDEX_DEFAULT = 2;

// Macro for debugging so we can use __func__
#define DEBUG() \
    (qDebug().nospace() << "AudioInputSourceComboBox: " << __func__ << "(): ")

// -------------------------
// AudioInputSourceSelection
// -------------------------

AudioInputSourceSelection::AudioInputSourceSelection() : _selectionType(), _id()
    { throw std::runtime_error(std::string(__func__) + "(): we shouldn't ever use this!"); }

AudioInputSourceSelection::AudioInputSourceSelection(AudioInputSourceSelectionType type) :
    _selectionType(type),
    _id()
{
    switch (type) {
        case AudioInputSourceSelectionType::None:
        case AudioInputSourceSelectionType::Default:
            break;
        default:
            throw std::invalid_argument(std::string(__func__) + "(): invalid type");
    }
}

AudioInputSourceSelection::AudioInputSourceSelection(const QAudioDevice &device) :
    _selectionType(AudioInputSourceSelectionType::Explicit),
    _id(device.id())
{}

AudioInputSourceSelection::AudioInputSourceSelection(const AudioInputSourceSelection &selection) :
    _selectionType(selection._selectionType),
    _id(selection._id)
{}

void AudioInputSourceSelection::operator=(const AudioInputSourceSelection &other)
{
    _selectionType = other._selectionType;
    _id = other._id;
}

bool AudioInputSourceSelection::operator==(const AudioInputSourceSelection &other)
{
    if (_selectionType != other._selectionType)
        return false;
    if (_selectionType == AudioInputSourceSelectionType::Explicit && _id != other._id)
        return false;
    return true;
}

QAudioDevice *AudioInputSourceSelection::getAudioDevice(QList<QAudioDevice> &devices) const
{
    switch (_selectionType) {
        case AudioInputSourceSelectionType::None:
            return nullptr;

        case AudioInputSourceSelectionType::Default: {
            auto deviceIter = FIND_IF(devices, [](QAudioDevice &ad) { return ad.isDefault(); });
            if (deviceIter == devices.end())
                deviceIter = devices.begin();
            return (deviceIter != devices.end()) ? &(*deviceIter) : nullptr;
        }

        case AudioInputSourceSelectionType::Explicit: {
            auto deviceIter = FIND_IF(devices, [&](QAudioDevice &ad) { return ad.id() == _id; });
            return (deviceIter != devices.end()) ? &(*deviceIter) : nullptr;
        }

        default:
            throw std::invalid_argument(std::string(__func__) + "(): invalid type");
    }
};

// ------------------------
// AudioInputSourceComboBox
// ------------------------

static QByteArray getDeviceId(QAudioDevice *device)
    { return device ? device->id() : QByteArray {}; }

AudioInputSourceComboBox::AudioInputSourceComboBox(QWidget *parent) :
    QComboBox(parent),
    _mediaDevices(std::make_unique<QMediaDevices>(this)),
    _inputDevices(QMediaDevices::audioInputs()),
    _selectedDevice(AudioInputSourceSelectionType::Default),
    _selectedDeviceId(getDeviceId(_selectedDevice.getAudioDevice(_inputDevices)))
{
    // Populate our combo box and select the appropriate item.
    refreshItems();

    connect(_mediaDevices.get(), &QMediaDevices::audioInputsChanged, this, &AudioInputSourceComboBox::onAudioInputsChanged);
    connect(this, &QComboBox::currentIndexChanged, this, &AudioInputSourceComboBox::onCurrentIndexChanged);
}

AudioInputSourceComboBox::~AudioInputSourceComboBox()
{}

void AudioInputSourceComboBox::setInputDevice(AudioInputSourceSelection selection)
{
    DEBUG() << "updating selected device";

    // Remember the old actual audio device
    _selectedDevice = selection;

    // Update the combo box.
    selectCurrentInputDevice();

    // If the actual device didn't change, we're done.
    QAudioDevice *newDevice = getInputDevice();
    QByteArray newDeviceId = newDevice ? newDevice->id() : QByteArray {};

    if (_selectedDeviceId == newDeviceId) {
        DEBUG() << "actual device unchanged";
        return;
    }
    _selectedDeviceId = newDeviceId;

    // Device changed; emit a signal.
    if (newDevice)
        DEBUG() << "actual device changed to " << newDevice->description() << " / " << newDevice->id();
    else
        DEBUG() << "actual device changed to (none)";

    emit inputDeviceChanged(newDevice);
}

QAudioDevice *AudioInputSourceComboBox::getInputDevice()
    { return _selectedDevice.getAudioDevice(_inputDevices); }

void AudioInputSourceComboBox::onCurrentIndexChanged(int index)
{
    if (_refreshingItems) {
        DEBUG() << "performing refresh, ignoring changed items";
        return;
    }

    DEBUG() << "new index = " << index;

    // The item data has all the device info, so just run setInputDevice() with it
    setInputDevice(qvariant_cast<AudioInputSourceSelection>(currentData()));
}

void AudioInputSourceComboBox::onAudioInputsChanged()
{
    DEBUG();
    refreshItems();
}

void AudioInputSourceComboBox::refreshItems()
{
    DEBUG();
    {
        // Temporarily increase '_refreshingItems' to suppress certain events while refreshing
        [[maybe_unused]] const auto &ic = IncreaseCounter { _refreshingItems };

        // To be safe, completely clear all items
        clear();

        // Add special devices for selection
        // NOTE: if you change this, please change DEVICE_INDEX_* constants!!
        addItem("(none)", QVariant::fromValue(AudioInputSourceSelection { AudioInputSourceSelectionType::None }));
        addItem("(default)", QVariant::fromValue(AudioInputSourceSelection { AudioInputSourceSelectionType::Default }));

        // Add all known devices
        _inputDevices = QMediaDevices::audioInputs();
        for (const auto &ad : _inputDevices)
            addItem(ad.description(), QVariant::fromValue(AudioInputSourceSelection { ad }));
    }

    // Re-select the appropriate combo box item.
    selectCurrentInputDevice();
}

void AudioInputSourceComboBox::selectCurrentInputDevice()
{
    // Do nothing if the item is already selected
    auto comboDevice = qvariant_cast<AudioInputSourceSelection>(currentData());
    if (_selectedDevice == comboDevice) {
        DEBUG() << "correct item already selected";
        return;
    }

    // Look for a combo box item with matching data to _selectedDevice
    int itemCount = this->count();
    int deviceIndex;
    for (deviceIndex = 0; deviceIndex < itemCount; ++deviceIndex) {
        auto itemDevice = qvariant_cast<AudioInputSourceSelection>(itemData(deviceIndex));
        if (_selectedDevice == itemDevice)
            break;
    }

    // If no matching data was found, we have a problem; just use '(default)'
    if (deviceIndex == itemCount) {
        DEBUG() << "old audio input device not found; selecting default";
        _selectedDevice = AudioInputSourceSelection { AudioInputSourceSelectionType::Default };
        deviceIndex = DEVICE_INDEX_DEFAULT;
    }

    // Finally update our combo box!
    DEBUG() << "selecting item: " << deviceIndex;
    setCurrentIndex(deviceIndex);
}
