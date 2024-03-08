#include "controller/note.h"
#include "model/note.h"
#include <QDir>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>


namespace Controller {

Note::Note(QObject *parent)
    : QObject{parent}
{
  note_list_ = new Model::NoteList(this);
  editing_note_index_ = -1;
  load(kNotesDir);
  qDebug() << "current dir: " << QDir::current().absolutePath();
}

void Note::load(const QString& notes_dir)
{
  QDir dir(notes_dir);
  qDebug() << "notes_dir: " << dir.absolutePath();
  auto entries = dir.entryList(QStringList("note_*.json"), QDir::Files, QDir::Time);
  for (const auto& entry : entries)
  {
    QFile file(entry);
    if (!file.open(QIODevice::ReadOnly))
    {
      qDebug() << tr("Cannot open note %1").arg(entry);
      continue;
    }
    note_file_path_.push_back(entry);
    auto content = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonObject object = doc.object();

    Model::Note note{object["title"].toString(), object["content"].toString()};

    note_list_->appendData(note);
  }
}

bool Note::save(const QString& note_file_path, const Model::Note* note)
{
  QString temp_file = "temp_note.json";
  QFile file(temp_file);
  if (!file.open(QIODevice::WriteOnly))
  {
    qDebug() << tr("Cannot open file for saving note %1").arg(note->title);
    return false;
  }
  file.write(QJsonDocument(note->toJson()).toJson());
  file.close();
  file.rename(note_file_path);
  return true;
}

void Note::createNewNote()
{
  note_list_->appendData(Model::Note{
      "",
      ""
  });
  editing_note_index_ = note_list_->rowCount() - 1;
  note_file_path_.push_back(tr("note_%1.json").arg(editing_note_index_));
  QModelIndex model_index = note_list_->index(editing_note_index_, 0);
  emit createEditingNote(model_index);
}

void Note::setEditingNote(int index)
{
  editing_note_index_ = index;
  QModelIndex model_index = note_list_->index(index, 0);
  emit createEditingNote(model_index);
}

void Note::finishEditingNote(const Model::Note& note)
{
  qDebug() << "finish editing note: " << note.title << ", " << note.content;
  if (editing_note_index_ > -1) {
    if (!save(note_file_path_[editing_note_index_], &note)) {
      return;
    }
    QModelIndex index = note_list_->index(editing_note_index_, 0);

    note_list_->setHeaderData(editing_note_index_, Qt::Vertical, note.title, Qt::EditRole);
    note_list_->setData(index, note.content, Qt::EditRole);
    editing_note_index_ = -1;
  }
}

void Note::remove(int index)
{
  note_list_->removeRow(index);
  QFile::remove(note_file_path_[index]);
  note_file_path_.erase(note_file_path_.constBegin() + index);
}

} // namespace Controller
