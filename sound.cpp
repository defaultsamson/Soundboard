#include "sound.h"
#include <QListWidgetItem>

Sound::Sound(std::string text) :
    QListWidgetItem(QString::fromStdString(text))
{

}
