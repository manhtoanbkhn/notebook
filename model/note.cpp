#include "note.h"

namespace Model
{

NoteList::NoteList(QObject* parent)
    : QAbstractListModel{parent}
{}

int NoteList::rowCount(const QModelIndex& parent) const
{
  return notes_.size();
}

QVariant NoteList::data(const QModelIndex& index, int role) const
{
  switch(role)
  {
  case Qt::DisplayRole:
  {
    return notes_[index.row()].content;
  }
  }
}

QVariant NoteList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Vertical)
  {
    switch(role)
    {
    case Qt::DisplayRole:
      return notes_[section].title;
    }
  }
}

bool NoteList::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (role != Qt::EditRole) return false;
  notes_[index.row()].content = value.toString();
  return true;
}

bool NoteList::setHeaderData(int section, Qt::Orientation orientation,
                             const QVariant &value, int role)
{
  if (role != Qt::EditRole && orientation != Qt::Vertical) return false;
  notes_[section].title = value.toString();
  return true;
}

void NoteList::removeRow(int row, const QModelIndex &parent)
{
  if (row >= rowCount()) return;
  beginRemoveRows(parent, row, row);
  notes_.erase(notes_.constBegin() + row);
  endRemoveRows();
}

void NoteList::appendData(const Note& note)
{
  beginInsertRows(QModelIndex(), notes_.size(), notes_.size());
  notes_.push_back(note);
  endInsertRows();
}

Note* NoteList::getNote(int index)
{
  return &notes_[index];
}

const QVector<Note>& NoteList::notes() const
{
  return notes_;
}

} // namespace Model
