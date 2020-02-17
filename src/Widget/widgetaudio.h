#ifndef WIDGETAUDIO_H
#define WIDGETAUDIO_H

#include <QWidget>

class WidgetAudio : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetAudio(QWidget* parent = nullptr);
    void setLevel(qreal value);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    qreal m_level = 0;
    QPixmap m_pixmap;
};

#endif // WIDGETAUDIO_H
