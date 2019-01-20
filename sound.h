#ifndef SOUND_H
#define SOUND_H

#include <QListWidgetItem>

class Sound : public QListWidgetItem
{
public:
    Sound(std::string text);
    void setFileName(std::string name);

private:
    std::string filename;
};

#endif // SOUND_H
