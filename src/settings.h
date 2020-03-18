#ifndef SETTINGS_H
#define SETTINGS_H

#include "fileutil.h"

#include <QString>
#include <QSettings>

class Settings
{
public:
    static QSettings* QSETTINGS;

    static Settings DARK_THEME;
    static Settings ACTIVE_BOARD;
    static Settings SETTINGS_TAB;

    static Settings HK_ENABLE_KEYBINDS_KEY;
    static Settings HK_DISABLE_KEYBINDS_KEY;
    static Settings HK_STOP_SOUNDS_KEY;
    static Settings HK_PAUSE_SOUNDS_KEY;
    static Settings HK_RESUME_SOUNDS_KEY;
    static Settings HK_MUTE_INPUT_KEY;
    static Settings HK_UNMUTE_INPUT_KEY;
    static Settings HK_TOGGLE_MUTE_INPUT_KEY;
    static Settings NON_NATIVE_KEYNAMING;

    static Settings REMEMBER_WINDOW_SIZES;
    static Settings WINDOW_MAIN_GEOMETRY;
    static Settings WINDOW_MAIN_SPLITTER0;
    static Settings WINDOW_MAIN_SPLITTER1;
    static Settings WINDOW_SETTINGS_WIDTH;
    static Settings WINDOW_SETTINGS_HEIGHT;
    static Settings WINDOW_BOARD_WIDTH;
    static Settings WINDOW_BOARD_HEIGHT;
    static Settings WINDOW_SOUND_WIDTH;
    static Settings WINDOW_SOUND_HEIGHT;
    static Settings SHOW_SETTINGS_OUTPUT0;
    static Settings SHOW_SETTINGS_OUTPUT1;
    static Settings SHOW_SETTINGS_INPUT0;
    static Settings SHOW_SETTINGS_AUDIO_FILE;

    static Settings EXPLICIT_NO_OUTPUT_DEVICES;
    static Settings EXPLICIT_NO_INPUT_DEVICES;
    static Settings OUTPUT_INDEX0;
    static Settings OUTPUT_INDEX1;
    static Settings INPUT_INDEX0;
    static Settings OUTPUT_VOLUME0;
    static Settings OUTPUT_VOLUME1;
    static Settings INPUT_VOLUME0;
    static Settings TEST_VOLUME;
    static Settings INPUT_OUT0;
    static Settings INPUT_OUT1;
    static Settings INPUT_MUTED;
    static Settings SHOW_MUTE_BUTTON;
    static Settings SHOW_DRIVERS;
    static Settings TEST_FILE;

private:
    Settings(const QString key, const QVariant defaultValue = QVariant())
        : _key(key), _defaultValue(defaultValue) {}

    const QString _key;
    const QVariant _defaultValue;

public:
    const QVariant value();
    void setValue(const QVariant value);
    bool hasValue();
    void clearValue();
};

#endif // SETTINGS_H
