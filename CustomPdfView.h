#ifndef CUSTOMPDFVIEW_H
#define CUSTOMPDFVIEW_H

#include <QWidget>
#include <QPdfView>
#include <QPixmap>
#include <QPen>
#include <QMouseEvent>

class CustomPdfView : public QPdfView
{
    Q_OBJECT
public:
    explicit CustomPdfView(QWidget *parent = nullptr);
    void setDocument(QPdfDocument *doc);
    bool m_isDrawing;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPixmap m_canvasLayer;
    QPoint m_lastPoint;
    QPen m_pen;
    void drawLineTo(const QPoint &endPoint);
signals:
};

#endif // CUSTOMPDFVIEW_H
