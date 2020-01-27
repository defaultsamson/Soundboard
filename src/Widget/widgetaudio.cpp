#include "widgetaudio.h"

#include <QPainter>

WidgetAudio::WidgetAudio(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void WidgetAudio::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);

    painter.setPen(palette().color(QPalette::Dark));
    painter.drawRect(QRect(painter.viewport().left(),
                           painter.viewport().top(),
                           painter.viewport().right(),
                           painter.viewport().bottom()));

    if (m_level > 0.0) {
        int pos = static_cast<int>((painter.viewport().right() - painter.viewport().left() - 1) * m_level);
        Qt::GlobalColor colour = m_level >= 0.85 ? Qt::red : (m_level >= 0.7 ? Qt::yellow : Qt::green);
        painter.fillRect(painter.viewport().left() + 1,
                         painter.viewport().top() + 1,
                         pos,
                         painter.viewport().height() - 2,
                         colour);
    }
}

void WidgetAudio::setLevel(qreal value)
{
    m_level = value;
    update();
}
