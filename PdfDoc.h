#ifndef PDFDOC_H
#define PDFDOC_H

#include <QWidget>
#include <QPdfDocument>
#include <QtPdfWidgets/QPdfView>
#include <QLabel>

class PdfDoc : public QWidget
{
    Q_OBJECT
public:
    explicit PdfDoc(QWidget *parent = nullptr);
    // members
    qint64 pageIndex;
    QPdfDocument *document;
    QPdfView *m_view;
    static constexpr qreal ZOOM_STEP = 1.1;
    static constexpr qreal MIN_ZOOM = 0.5;
    static constexpr qreal MAX_ZOOM = 3.0;

    // methods
    void loadDocument(const QString &filePath);
    int currentPageIndex();
    int totalPages();


public slots:
    void zoom_in();
    void zoom_out();

signals:
};

#endif // PDFDOC_H
