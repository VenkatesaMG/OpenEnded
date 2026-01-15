#include "SearchBar.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

SearchBar::SearchBar(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    QLineEdit *searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText("Find...");
    searchInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QPushButton *prev = new QPushButton("<", this);
    prev->setFixedSize(25, 25);

    QPushButton *next = new QPushButton(">", this);
    next->setFixedSize(25, 25);

    QPushButton *close = new QPushButton("x", this);
    close->setFixedSize(25, 25);

    layout->addWidget(searchInput);
    layout->addWidget(prev);
    layout->addWidget(next);
    layout->addWidget(close);

    connect(close, &QPushButton::clicked, this, &SearchBar::closeButtonClicked);
    connect(searchInput, &QLineEdit::textChanged, this, &SearchBar::customChangedText);
    connect(prev, &QPushButton::clicked, this, &SearchBar::prevSearchSlot);
    connect(next, &QPushButton::clicked, this, &SearchBar::nextSearchSlot);

    this->setFixedSize(200, 25);
}

void SearchBar::customChangedText(const QString &currentText){
    emit searchSignal(currentText);
}

void SearchBar::closeButtonClicked()
{
    this->hide();
    emit clearSearch();
}

void SearchBar::prevSearchSlot(){
    emit prevSearchSignal();
}

void SearchBar::nextSearchSlot(){
    emit nextSearchSignal();
}
