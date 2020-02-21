#ifndef WIDGETVOLUME_H
#define WIDGETVOLUME_H

#include <QWidget>
#include <QSlider>
#include <QSpinBox>

class WidgetVolume : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetVolume(QWidget *parent = nullptr);
    int volume() { return _volume; }

public slots:
    void setVolume(int volumeInt);
    void setVolume(int volumeInt, bool fromSlider, bool fromSpinBox);

signals:
    void volumeChanged(int volumeInt);

private:
    int _volume = 0;
    bool _updtingFromSlider = false;
    QSlider* slider;
    QSpinBox* spinBox;
};

#endif // WIDGETVOLUME_H
