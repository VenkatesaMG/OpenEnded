#include <QMainWindow>
#include "PdfDoc.h"
#include <QLabel>
#include <QLineEdit>

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

private slots:
    void openFile();
    void closeApp();

private:
    Ui::MainWindow *ui;
};
