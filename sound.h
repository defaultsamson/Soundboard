#ifndef SOUND_H
#define SOUND_H

#include <QListWidgetItem>

class Sound : public QListWidgetItem
{
public:
    Sound(std::string text);
};

#endif // SOUND_H
