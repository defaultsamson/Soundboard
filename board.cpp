#include "board.h"
#include <QListWidgetItem>

Board::Board(std::string text) :
    QListWidgetItem(QString::fromStdString(text))
{

}

Board::~Board() {
    for (size_t i = 0; i < this->sounds.size(); ++i) delete this->sounds.at(i);
}

void Board::addSound(Sound *sound) {
    this->sounds.push_back(sound);
}

void Board::removeSound(int n) {
    delete this->sounds.at(n);
    this->sounds.erase(this->sounds.begin() + n);
}

void Board::populateList(QListWidget *list) {
    // Removes items from list
    for (int i = list->count() - 1; i >= 0; --i) list->takeItem(i);
    // Adds items to list
    for (size_t i = 0; i < this->sounds.size(); ++i) list->addItem(this->sounds.at(i));
}
