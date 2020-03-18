#include "fileutil.h"

const QString FileUtil::DEFAULT_DIR = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Soundboard/";
const QString FileUtil::DEFAULT_SOUNDBOARD_FILE = "default.json";
const QString FileUtil::SETTINGS_FILE = "settings.ini";
const QString FileUtil::DEFAULT_TEST_FILE = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/test.ogg";
