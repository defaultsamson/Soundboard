#include "settings.h"

const QVariant Settings::value() {
    return QSETTINGS->value(_key, _defaultValue);
}

void Settings::setValue(const QVariant value) {
    QSETTINGS->setValue(_key, value);
}

bool Settings::hasValue() {
    QVariant val = QSETTINGS->value(_key);
    return !val.isNull() && val.isValid();
}

void Settings::clearValue() {
    setValue(QVariant());
}

QSettings* Settings::QSETTINGS = new QSettings(FileUtil::DEFAULT_DIR + FileUtil::SETTINGS_FILE, QSettings::IniFormat);

Settings Settings::DARK_THEME = Settings("dark_theme", false);
Settings Settings::ACTIVE_BOARD = Settings("active_board");
Settings Settings::SETTINGS_TAB = Settings("settings_tab", 0);

Settings Settings::HK_ENABLE_KEYBINDS_KEY = Settings("hk_enable_keybinds_key");
Settings Settings::HK_DISABLE_KEYBINDS_KEY = Settings("hk_disable_keybinds_key");
Settings Settings::HK_STOP_SOUNDS_KEY = Settings("hk_stop_sounds_key");
Settings Settings::HK_PAUSE_SOUNDS_KEY = Settings("hk_pause_sounds_key");
Settings Settings::HK_RESUME_SOUNDS_KEY = Settings("hk_resume_sounds_key");
Settings Settings::HK_MUTE_INPUT_KEY = Settings("hk_mute_input_key");
Settings Settings::HK_UNMUTE_INPUT_KEY = Settings("hk_unmute_input_key");
Settings Settings::HK_TOGGLE_MUTE_INPUT_KEY = Settings("hk_toggle_mute_input_key");
Settings Settings::NON_NATIVE_KEYNAMING = Settings("non_native_keynaming", true);

Settings Settings::REMEMBER_WINDOW_SIZES = Settings("remember_window_sizes", true);
Settings Settings::WINDOW_MAIN_GEOMETRY = Settings("window_main_geometry");
Settings Settings::WINDOW_MAIN_SPLITTER0 = Settings("window_main_slider0");
Settings Settings::WINDOW_MAIN_SPLITTER1 = Settings("window_main_slider1");
Settings Settings::WINDOW_SETTINGS_WIDTH = Settings("window_settings_width", 500);
Settings Settings::WINDOW_SETTINGS_HEIGHT = Settings("window_settings_height", 500);
Settings Settings::WINDOW_BOARD_WIDTH = Settings("window_board_width", 500);
Settings Settings::WINDOW_BOARD_HEIGHT = Settings("window_board_height", 500);
Settings Settings::WINDOW_SOUND_WIDTH = Settings("window_sound_width", 500);
Settings Settings::WINDOW_SOUND_HEIGHT = Settings("window_sound_height", 500);
Settings Settings::SHOW_SETTINGS_OUTPUT0 = Settings("show_settings_output0", true);
Settings Settings::SHOW_SETTINGS_OUTPUT1 = Settings("show_settings_output1", true);
Settings Settings::SHOW_SETTINGS_INPUT0 = Settings("show_settings_input0", true);
Settings Settings::SHOW_SETTINGS_AUDIO_FILE = Settings("show_settings_audio_file", true);

Settings Settings::EXPLICIT_NO_OUTPUT_DEVICES = Settings("explicit_no_outputs", false);
Settings Settings::EXPLICIT_NO_INPUT_DEVICES = Settings("explicit_no_inputs", false);
Settings Settings::OUTPUT_INDEX0 = Settings("output_index0");
Settings Settings::OUTPUT_INDEX1 = Settings("output_index1");
Settings Settings::INPUT_INDEX0 = Settings("input_index0");
Settings Settings::OUTPUT_VOLUME0 = Settings("output_volume0", 100);
Settings Settings::OUTPUT_VOLUME1 = Settings("output_volume1", 100);
Settings Settings::INPUT_VOLUME0 = Settings("input_volume0", 100);
Settings Settings::TEST_VOLUME = Settings("test_volume", 100);
Settings Settings::INPUT_OUT0 = Settings("input_out0", false);
Settings Settings::INPUT_OUT1 = Settings("input_out1", false);
Settings Settings::INPUT_MUTED = Settings("input_muted", false);
Settings Settings::SHOW_MUTE_BUTTON = Settings("show_mute_button", true);
Settings Settings::SHOW_DRIVERS = Settings("show_drivers", false);
Settings Settings::TEST_FILE = Settings("test_file", FileUtil::DEFAULT_TEST_FILE);
