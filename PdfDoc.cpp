#include "PdfDoc.h"
#include <QPdfDocument>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QSize>
#include <QLabel>
#include <QPixmap>
#include <QPdfView>
#include <QVBoxLayout>
#include <QPdfPageNavigator>

PdfDoc::PdfDoc(QWidget *parent)
    : QWidget{parent}
{
    document = new QPdfDocument(this);
    m_view = new QPdfView(this);

    m_view->setDocument(document);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_view);
    m_view->setPageMode(QPdfView::PageMode::MultiPage);
}

void PdfDoc::loadDocument(const QString &filePath){
    this->document->load(filePath);
}

void PdfDoc::zoom_in(){
    qreal currentFactor = this->m_view->zoomFactor();
    qreal newFactor = currentFactor * this->ZOOM_STEP;
    if(newFactor <= this->MAX_ZOOM){
        this->m_view->setZoomFactor(newFactor);
    }
}

void PdfDoc::zoom_out(){
    qreal currentFactor = this->m_view->zoomFactor();
    qreal newFactor = currentFactor / this->ZOOM_STEP;
    if(newFactor >= this->MIN_ZOOM){
        this->m_view->setZoomFactor(newFactor);
    }
}

int PdfDoc::currentPageIndex(){
    if(!this->m_view || !this->document){
        return -1;
    }
    return this->m_view->pageNavigator()->currentPage();
}

int PdfDoc::totalPages(){
    if(!this->m_view || !this->document){
        return -1;
    }
    return this->document->pageCount();
}
