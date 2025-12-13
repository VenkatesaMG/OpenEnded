#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);

private slots:
    void customChangedText(const QString &currentText);
    void closeButtonClicked();
    void prevSearchSlot();
    void nextSearchSlot();

signals:
    void searchSignal(const QString &text);
    void clearSearch();
    void prevSearchSignal();
    void nextSearchSignal();
};

#endif // SEARCHBAR_H
