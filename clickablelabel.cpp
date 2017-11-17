#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent)
{
    // Since this is only really a wrapper around a label, the only extra stuff is in the mouse press event.
}

ClickableLabel::~ClickableLabel()
{
    // Since this is only really a wrapper around a label, the only extra stuff is in the mouse press event.
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked(); // Send a clicked signal when the label is clicked by the mouse.
}
