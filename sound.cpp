#include "sound.h"
#include <QListWidgetItem>

Sound::Sound(std::string text) :
    QListWidgetItem(QString::fromStdString(text))
{

}

void Sound::setFileName(std::string name) {
    this->filename = name;
}
