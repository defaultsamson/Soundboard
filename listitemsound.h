#ifndef SOUND_H
#define SOUND_H

#include <QListWidgetItem>
#include <QString>
#include <QJsonObject>
#include <QHotkey>

class MainWindow;
class ListItemBoard;

class ListItemSound : public QListWidgetItem
{
public:
    static QString NEW_SOUND;
    ListItemSound(MainWindow *main, ListItemBoard *board);
    void setFileName(QString name);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    int key();
    void setKey(int key);
    void reg();
    void unreg();
    ListItemBoard *board();

private:
    MainWindow *main;
    ListItemBoard *_board;
    QString filename;
    int _key;
    QHotkey *hotkey;
};

#endif // SOUND_H
