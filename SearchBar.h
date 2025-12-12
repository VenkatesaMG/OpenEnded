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

signals:
    void searchSignal(const QString &text);
    void clearSearch();
};

#endif // SEARCHBAR_H
