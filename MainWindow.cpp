#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "PdfDoc.h"
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

    toolBar->addAction(open);
    toolBar->addSeparator();

    QAction *prevPageAction = new QAction(tr("&Prev"));
    QAction *nextPageAction = new QAction(tr("&Next"));
    toolBar->addAction(prevPageAction);
    toolBar->addAction(nextPageAction);
    toolBar->addSeparator();

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

    // PDF Canvas
    pdfWidget = new PdfDoc(this);
    setCentralWidget(pdfWidget);

    connect(zoom_in, &QAction::triggered, pdfWidget, &PdfDoc::zoom_in);
    connect(zoom_out, &QAction::triggered, pdfWidget, &PdfDoc::zoom_out);
}

void MainWindow::openFile(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), QDir::homePath(), tr("Text Files (*.pdf);; All Files (*.*)"));
    if(!fileName.isEmpty()){
        qDebug() << "Selected file path:" << fileName;
        this->pdfWidget->loadDocument(fileName);
    } else {
        qDebug() << "File Selection cancelled";
    }
    std::string totalPages = std::to_string(this->pdfWidget->totalPages());
    this->pageNum->setText(QString::fromStdString(totalPages));
    std::string customPageLabel = "of " + totalPages;
    this->pageLabel->setText(QString::fromStdString(customPageLabel));
}

void MainWindow::closeApp(){
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
