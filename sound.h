#ifndef SOUND_H
#define SOUND_H

#include <QListWidgetItem>
#include <QString>
#include <QJsonObject>

class Sound : public QListWidgetItem
{
public:
    Sound(QString text = "New Sound");
    void setFileName(QString name);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);

private:
    QString filename;
};

#endif // SOUND_H
