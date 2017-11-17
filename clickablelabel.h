#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    /**
     * @brief ClickableLabel Default Constructor. The only constructer, as this is just a wrapper around a QLabel.
     * @param parent The parent widget for the Label.
     * @param f Flags that are passed to the QLabel constructor call.
     */
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    /**
     * @brief Default constructor. Nothing to delete off the heap though.
     */
    ~ClickableLabel();

signals:
    /**
     * @brief clicked A signal that is emitted to indicate that the Label has been clicked on by the user.
     */
    void clicked();

protected:
    /**
     * @brief mousePressEvent The function that is called to handle when a user clicks on this object.
     * @param event A Mouse Event, generated from mouse input. Not used, but needed for overriding.
     */
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_H
