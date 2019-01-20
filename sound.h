#ifndef SOUND_H
#define SOUND_H

#include <QListWidgetItem>
#include <QString>
#include <QJsonObject>
#include <QHotkey>

class MainWindow;

class Sound : public QListWidgetItem
{
public:
    Sound(MainWindow *main, QString text = "New Sound");
    void setFileName(QString name);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    void reg();
    void unreg();

private:
    QString filename;

    int _key;
    MainWindow *main;
    QHotkey *hotkey;
};

#endif // SOUND_H
