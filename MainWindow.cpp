#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "PdfDoc.h"
#include "SearchBar.h"
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QPdfDocument>
#include <QToolBar>
#include <QLineEdit>
#include <QWidgetList>
#include <QPdfSearchModel>
#include <QPdfLink>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // MenuBar

    // File
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *open = new QAction(tr("&Open"), this);
    QAction *close = new QAction(tr("&Close"), this);
    QAction *print = new QAction(tr("&Print"), this);

    open->setShortcut(QKeySequence::Open);
    close->setShortcut(QKeySequence(tr("Ctrl+W")));
    fileMenu->addAction(open);
    fileMenu->addAction(close);
    fileMenu->addAction(print);

    connect(open, &QAction::triggered, this, &MainWindow::openFile);
    connect(close, &QAction::triggered, this, &MainWindow::closeApp);

    // View
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QAction *zoom_in = new QAction(tr("&Zoom In"));
    QAction *zoom_out = new QAction(tr("&Zoom Out"));

    viewMenu->addAction(zoom_in);
    viewMenu->addAction(zoom_out);

    // Help
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    QAction *about = new QAction(tr("&About"));
    helpMenu->addAction(about);

    // ToolBar
    QToolBar *toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);

    toolBar->addAction(zoom_in);
    toolBar->addAction(zoom_out);

    QAction *fitToPageWidthAction = new QAction(tr("&Fit to Page/Width"));
    toolBar->addAction(fitToPageWidthAction);
    toolBar->addSeparator();

    pageNum = new QLineEdit(this);
    pageNum->setFixedSize(QSize(25, 25));
    pageLabel = new QLabel();
    toolBar->addWidget(pageNum);
    toolBar->addWidget(pageLabel);

    QAction *search = new QAction(tr("&Search"));
    toolBar->addAction(search);
    toolBar->addSeparator();

    QAction *draw = new QAction(tr("&Draw"));
    toolBar->addAction(draw);

    m_searchBar = new SearchBar(this);
    m_searchBar->hide();
    // QAction *showThumbnails = new QAction(tr("&Thumbnails"));
    // toolBar->addAction(showThumbnails);

    // PDF Canvas
    pdfWidget = new PdfDoc(this);
    setCentralWidget(pdfWidget);

    // Thumbnails
    connect(zoom_in, &QAction::triggered, pdfWidget, &PdfDoc::zoom_in);
    connect(zoom_out, &QAction::triggered, pdfWidget, &PdfDoc::zoom_out);
    connect(fitToPageWidthAction, &QAction::triggered, pdfWidget, &PdfDoc::fitToPageWidth);
    connect(pdfWidget, &PdfDoc::getCurrentPage, pageNum, &QLineEdit::setText);
    connect(search, &QAction::triggered, this, &MainWindow::handleSearch);
    connect(m_searchBar, &SearchBar::searchSignal, pdfWidget, &PdfDoc::searchSlot);
    connect(m_searchBar, &SearchBar::clearSearch, pdfWidget, &PdfDoc::clearSearch);
    connect(m_searchBar, &SearchBar::prevSearchSignal, this, &MainWindow::onSearchPrev);
    connect(m_searchBar, &SearchBar::nextSearchSignal, this, &MainWindow::onSearchNext);
    connect(draw, &QAction::triggered, this, [=](){
        bool curDrawState = pdfWidget->m_view->m_isDrawing;
        pdfWidget->m_view->m_isDrawing = !curDrawState;
    });
}

void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), QDir::homePath(), tr("Text Files (*.pdf);; All Files (*.*)"));
    if(!fileName.isEmpty()){
        qDebug() << "Selected file path:" << fileName;
        this->pdfWidget->loadDocument(fileName);
    } else {
        qDebug() << "File Selection cancelled";
    }
    this->pageNum->setText(QString::fromStdString("1"));
    std::string totalPages = std::to_string(this->pdfWidget->totalPages());
    std::string customPageLabel = "of " + totalPages;
    this->pageLabel->setText(QString::fromStdString(customPageLabel));
}

void MainWindow::handleSearch(){
    if (!m_searchBar->isVisible()) {
        m_searchBar->show();
        pdfWidget->m_view->update();
    }
}

void MainWindow::closeApp(){
    this->close();
}

void MainWindow::onSearchPrev(){
    auto *tmpSearchModel = pdfWidget->getSearchModel();
    if(!tmpSearchModel || tmpSearchModel->rowCount(QModelIndex()) == 0){
        return;
    }
    currentSearchIndex--;
    if(currentSearchIndex < 0){
        currentSearchIndex = tmpSearchModel->rowCount(QModelIndex()) - 1;
    }
    jumpToSearchResult(currentSearchIndex);
}

void MainWindow::onSearchNext(){
    auto *tmpSearchModel = pdfWidget->getSearchModel();
    if(!tmpSearchModel || tmpSearchModel->rowCount(QModelIndex()) == 0){
        return;
    }
    currentSearchIndex++;
    if(currentSearchIndex >= tmpSearchModel->rowCount(QModelIndex())){
        currentSearchIndex = 0;
    }
    jumpToSearchResult(currentSearchIndex);
}

void MainWindow::jumpToSearchResult(int pageIndex){
    QPdfLink link = pdfWidget->getSearchModel()->resultAtIndex(pageIndex);
    pdfWidget->pageNavigator->jump(link.page(), link.location(), pdfWidget->pageNavigator->currentZoom());
    statusBar()->showMessage(QString("Result %1 of %2").arg(pageIndex + 1).arg(pdfWidget->getSearchModel()->rowCount(QModelIndex())));
}

MainWindow::~MainWindow()
{
    delete ui;
}
