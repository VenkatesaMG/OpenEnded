#ifndef PDFDOC_H
#define PDFDOC_H

#include <QWidget>
#include <QPdfDocument>
#include <QtPdfWidgets/QPdfView>
#include <QLabel>
#include <QPdfPageNavigator>
#include <QPdfLinkModel>
#include "CustomPdfView.h"

class PdfDoc : public QWidget
{
    Q_OBJECT
public:
    explicit PdfDoc(QWidget *parent = nullptr);
    QPdfSearchModel *getSearchModel() const { return m_searchModel; }

    // members
    qint64 pageIndex;
    QPdfDocument *document;
    CustomPdfView *m_view;
    QPdfPageNavigator *pageNavigator;
    QPdfSearchModel *m_searchModel;
    QPdfLinkModel *m_linkModel;

    static constexpr qreal ZOOM_STEP = 1.1;
    static constexpr qreal MIN_ZOOM = 0.5;
    static constexpr qreal MAX_ZOOM = 3.0;

    // methods
    void loadDocument(const QString &filePath);
    int totalPages();
    int currentPageIndex();
    void debugLinksOnCurrentPage();

public slots:
    void zoom_in();
    void zoom_out();
    void fitToPageWidth();
    void searchSlot(const QString &text);
    void clearSearch();

private slots:
    void handlePageChange(int pageNumber);

signals:
    void getCurrentPage(const QString &pageNumber);
};

#endif // PDFDOC_H
