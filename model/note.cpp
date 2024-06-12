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
  case ContentRole:
  case Qt::DisplayRole:
  {
    return notes_[index.row()].content;
  }
  case TimeRole:
  {
    return notes_[index.row()].time;
  }
  }
  return QVariant();
}

QVariant NoteList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Vertical)
  {
    switch(role)
    {
    case TitleRole:
    case Qt::DisplayRole:
      return notes_[section].title;
    }
  }
  return QVariant();
}

bool NoteList::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (role == ContentRole || role == Qt::EditRole)
    notes_[index.row()].content = value.toString();
  else if (role == TimeRole)
    notes_[index.row()].time = value.toString();
  else
    return false;
  return true;
}

bool NoteList::setHeaderData(int section, Qt::Orientation orientation,
                             const QVariant &value, int role)
{
  if (role != Qt::EditRole && role != TitleRole && orientation != Qt::Vertical) return false;
  notes_[section].title = value.toString();
  return true;
}

QHash<int, QByteArray> NoteList::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[ContentRole] = "ContentRole";
  roles[TimeRole] = "TimeRole";
  roles[TitleRole] = "TitleRole";
  return roles;
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
