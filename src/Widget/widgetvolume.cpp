#include "widgetvolume.h"

#include <QHBoxLayout>
#include <iostream>

WidgetVolume::WidgetVolume(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(QMargins(0, 0, 0, 0));

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 200);
    slider->setValue(100);
    layout->addWidget(slider);
    QObject::connect(slider, &QSlider::valueChanged, this, [&](int value){ setValue(value, true, false); });

    spinBox = new QSpinBox();
    spinBox->setRange(0, 1000);
    layout->addWidget(spinBox);
    QObject::connect(spinBox, &QSpinBox::valueChanged, this, [&](int value){ setValue(value, false, true); });

    setLayout(layout);
}

void WidgetVolume::setValue(int volumeInt) {
    setValue(volumeInt, false, false);
}

void WidgetVolume::setValue(int volumeInt, bool fromSlider, bool fromSpinBox) {
    if (fromSlider) {
        // Allow users to edit the number in the spinbox past what the slider goes to
        if (!(volumeInt == slider->maximum() && spinBox->value() > volumeInt)) {
            _volume = volumeInt;
            spinBox->setValue(_volume);
        }

    } else if (fromSpinBox) {
        _volume = volumeInt;
        slider->setValue(_volume);
        // Only emit volumeChanged here, because each setValue call to "spinBox" and "slider"
        // Will result in this function being called again. emitting volumeChanged here
        // makes it so that it only emits once.
        emit valueChanged(volumeInt);

    } else {
        _volume = volumeInt;
        // The order of these setValue calls matters
        spinBox->setValue(_volume);
        slider->setValue(_volume);
    }
}
