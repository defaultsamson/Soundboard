#ifndef SOUND_H
#define SOUND_H

#include <QListWidgetItem>
#include <QString>
#include <QJsonObject>
#include <QHotkey>

class MainWindow;

class ListItemSound : public QListWidgetItem
{
public:
    static QString NEW_SOUND;
    ListItemSound(MainWindow *main, QString text = NEW_SOUND);
    void setFileName(QString name);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    void reg();
    void unreg();

private:
    MainWindow *main;

    QString filename;
    int _key;
    QHotkey *hotkey;
};

#endif // SOUND_H