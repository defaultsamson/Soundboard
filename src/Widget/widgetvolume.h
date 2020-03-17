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
    int value() { return _volume; }

public slots:
    void setValue(int volumeInt);
    void setValue(int volumeInt, bool fromSlider, bool fromSpinBox);

signals:
    void valueChanged(int volumeInt);

private:
    int _volume = 0;
    bool _updtingFromSlider = false;
    QSlider* slider;
    QSpinBox* spinBox;
};

#endif // WIDGETVOLUME_H
