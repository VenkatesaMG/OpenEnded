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
#include <QPdfLinkModel>
#include <QDebug>
#include "CustomPdfView.h"

PdfDoc::PdfDoc(QWidget *parent)
    : QWidget{parent}
{
    document = new QPdfDocument(this);
    m_view = new CustomPdfView(this);
    m_searchModel = new QPdfSearchModel(this);
    m_searchModel->setDocument(document);
    m_linkModel = new QPdfLinkModel(this);
    m_linkModel->setDocument(document);

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
    debugLinksOnCurrentPage();
    emit getCurrentPage(QString::fromStdString(pageStringNumber));
}

void PdfDoc::searchSlot(const QString &text){
    m_searchModel->setSearchString(text);
    m_searchModel->searchString();
    m_view->update();
}

void PdfDoc::clearSearch(){
    m_searchModel->setSearchString("");
}


void PdfDoc::debugLinksOnCurrentPage()
{
    // 1. Create a transient model to scan the page
    QPdfLinkModel model;
    model.setDocument(document);
    model.setPage(pageNavigator->currentPage());

    int count = model.rowCount(QModelIndex());
    qDebug() << "--- Link Debugger ---";
    qDebug() << "Page" << pageNavigator->currentPage() + 1 << "has" << count << "links.";

    for (int i = 0; i < count; ++i) {
        // 1. Get the QModelIndex for the current row
        QModelIndex index = model.index(i, 0);

        // 2. Retrieve the QPdfLink object using the explicit integer value for the role
        // This is the most robust way when the LinkRole enum is missing or private.
        // ⚡ FIX: Use Qt::UserRole + 1 (The standard value for the first custom role)
        QPdfLink link = index.data(Qt::UserRole + 1).value<QPdfLink>();

        // 3. Now you have the QPdfLink object, proceed to print the details
        for (const QRectF &rect : link.rectangles()) {
            qDebug() << "  Link" << i << "Rect:" << rect << "Dest Page:" << link.page();
        }
    }
}
