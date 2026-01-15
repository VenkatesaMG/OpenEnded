#include "CustomPdfView.h"
#include <QPdfView>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QPaintEvent>
#include <QResizeEvent>

CustomPdfView::CustomPdfView(QWidget *parent)
    : QPdfView{parent}
{
    m_isDrawing = false;

    m_pen.setColor(Qt::red);
    m_pen.setWidth(3);
    m_pen.setCapStyle(Qt::RoundCap);
    m_pen.setJoinStyle(Qt::RoundJoin);
}

void CustomPdfView::setDocument(QPdfDocument *doc){
    QPdfView::setDocument(doc);
}

void CustomPdfView::mousePressEvent(QMouseEvent *event){
    if(m_isDrawing && (event->buttons() & Qt::LeftButton)){
        m_lastPoint = event->position().toPoint();
        return;
    }
    QPdfView::mousePressEvent(event);
}

void CustomPdfView::mouseMoveEvent(QMouseEvent *event){
    if(m_isDrawing && (event->buttons() & Qt::LeftButton)){
        drawLineTo(event->position().toPoint());
        return;
    }
    QPdfView::mouseMoveEvent(event);
}

void CustomPdfView::mouseReleaseEvent(QMouseEvent *event){
    if(m_isDrawing && (event->buttons() & Qt::LeftButton)){
        drawLineTo(event->position().toPoint());
        return;
    }
    QPdfView::mouseReleaseEvent(event);
}

void CustomPdfView::drawLineTo(const QPoint &endPoint){
    QPainter painter(&m_canvasLayer);

    painter.setPen(m_pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(m_lastPoint, endPoint);

    int rad = 4;
    QRect dirtyRect = QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad);
    viewport()->update(dirtyRect);

    m_lastPoint = endPoint;
}

void CustomPdfView::paintEvent(QPaintEvent *event){
    QPdfView::paintEvent(event);
    QPainter painter(viewport());
    painter.drawPixmap(0, 0, m_canvasLayer);
}

void CustomPdfView::resizeEvent(QResizeEvent *event)
{
    if (width() > m_canvasLayer.width() || height() > m_canvasLayer.height()) {

        QPixmap newMap(size());
        newMap.fill(Qt::transparent);

        QPainter p(&newMap);
        p.drawPixmap(0, 0, m_canvasLayer);

        m_canvasLayer = newMap;
    }
    QPdfView::resizeEvent(event);
}
