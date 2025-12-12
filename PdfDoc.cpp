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
#include <QPdfSearchModel>

PdfDoc::PdfDoc(QWidget *parent)
    : QWidget{parent}
{
    document = new QPdfDocument(this);
    m_view = new QPdfView(this);
    m_searchModel = new QPdfSearchModel(this);
    m_searchModel->setDocument(document);

    m_view->setDocument(document);
    m_view->setSearchModel(m_searchModel);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_view);
    m_view->setPageMode(QPdfView::PageMode::MultiPage);
    pageNavigator = m_view->pageNavigator();
    connect(pageNavigator, &QPdfPageNavigator::currentPageChanged, this, &PdfDoc::handlePageChange);
}

void PdfDoc::loadDocument(const QString &filePath){
    this->document->load(filePath);
}

void PdfDoc::zoom_in(){
    this->m_view->setZoomMode(QPdfView::ZoomMode::Custom);
    qreal currentFactor = this->m_view->zoomFactor();
    qreal newFactor = currentFactor * this->ZOOM_STEP;
    if(newFactor <= this->MAX_ZOOM){
        this->m_view->setZoomFactor(newFactor);
    }
}

void PdfDoc::zoom_out(){
    this->m_view->setZoomMode(QPdfView::ZoomMode::Custom);
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

void PdfDoc::fitToPageWidth(){
    if(m_view){
        if(m_view->zoomMode() == QPdfView::ZoomMode::Custom){
            m_view->setZoomMode(QPdfView::ZoomMode::FitInView);
        } else if (m_view->zoomMode() == QPdfView::ZoomMode::FitToWidth){
            m_view->setZoomMode(QPdfView::ZoomMode::FitInView);
        } else if(m_view->zoomMode() == QPdfView::ZoomMode::FitInView){
            m_view->setZoomMode(QPdfView::ZoomMode::FitToWidth);
        }
    }
}

void PdfDoc::handlePageChange(int pageNumber){
    std::string pageStringNumber = std::to_string(pageNumber + 1);
    emit getCurrentPage(QString::fromStdString(pageStringNumber));
}

void PdfDoc::searchSlot(const QString &text){
    m_searchModel->setSearchString(text);
    m_searchModel->searchString();
    m_view->repaint();
}

void PdfDoc::clearSearch(){
    m_searchModel->setSearchString("");
}
