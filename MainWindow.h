#include <QMainWindow>
#include "PdfDoc.h"
#include "SearchBar.h"
#include <QLabel>
#include <QLineEdit>
#include <QWidgetList>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Member Variables
    PdfDoc *pdfWidget;
    QLabel *pageLabel;
    QLineEdit *pageNum;
    SearchBar *m_searchBar = nullptr;
    int currentSearchIndex = 0;
    // QWidgetList *thumbnails;

    // Methods
    void jumpToSearchResult(int pageIndex);

private slots:
    void openFile();
    void closeApp();
    void handleSearch();
    void onSearchNext();
    void onSearchPrev();

    // void displayThumbnails();

private:
    Ui::MainWindow *ui;
};
