#include "controller/note.h"
#include "model/note.h"
#include <QDir>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QCryptographicHash>
#include <QByteArrayView>


namespace Controller {


static QString generateFileName()
{
  return QCryptographicHash::hash(Note::getTimeNowStr(true).toUtf8(),
                                  QCryptographicHash::Sha256).toHex();
}

Note::Note(QObject *parent)
    : QObject{parent}
{
  note_list_ = new Model::NoteList(this);
  editing_note_index_ = -1;
  load(kNotesDir);
  qDebug() << "current dir: " << QDir::current().absolutePath();
}

Note::~Note()
{
  if (editing_note_index_ != -1) {
    finishEditingNote(*note_list_->getNote(editing_note_index_));
  }
}

void Note::load(const QString& notes_dir)
{
  QDir dir(notes_dir);
  qDebug() << "notes_dir: " << dir.absolutePath();
  auto entries = dir.entryList(QDir::Files, QDir::Time);
  qDebug() << "entries=" << entries;
  for (const auto& entry : entries)
  {
    qDebug() << "entry = " << entry;
    QFile file(entry);
    if (!file.open(QIODevice::ReadOnly))
    {
      qDebug() << tr("Cannot open note %1").arg(entry);
      continue;
    }
    note_file_path_.push_back(entry);
    auto file_content = file.readAll();
    qDebug() << "content=" << file_content;
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file_content, &error);
    if (error.error != QJsonParseError::NoError) {
      qDebug() << tr("parse file `%1` error: `%2`").arg(entry, error.errorString());
      continue;
    }
    QJsonObject object = doc.object();

    QString title = object.contains("title") ? object["title"].toString() : "";
    QString content = object.contains("content") ? object["content"].toString() : "";
    QString time = object.contains("time") ? object["time"].toString() : "";

    Model::Note note{title, content, time};

    qDebug() << "note.title=" << note.title << ", note.content=" << note.content;

    note_list_->appendData(note);
  }
}

bool Note::save(const Model::Note* note)
{
  QString file_path = note_file_path_.at(editing_note_index_);
  QFile file(file_path);
  if (!file.open(QIODevice::WriteOnly)) {
    qWarning() << "cannot open for writing: " << file_path;
    return false;
  }
  bool success = file.write(QJsonDocument(note->toJson()).toJson()) != -1;
  file.close();
  return success;
}

void Note::createNewNote()
{
  note_list_->appendData(Model::Note{
      "",
      "",
      ""
  });
  editing_note_index_ = note_list_->rowCount() - 1;
  note_file_path_.push_back(generateFileName());
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
  qDebug() << "finish editing note: " << note.title << ", " << note.content << ", " << editing_note_index_;
  if (editing_note_index_ == -1) return;

  Model::Note* editing_note = note_list_->getNote(editing_note_index_);

  if (note.content != editing_note->content ||
    note.title != editing_note->title) {
    bool saved = save(&note);
    if (!saved) {
      qWarning() << tr("Finish editing note without save last state.");
    }
    else {
      QModelIndex index = note_list_->index(editing_note_index_, 0);
      note_list_->setHeaderData(editing_note_index_, Qt::Vertical, note.title, Model::NoteList::TitleRole);
      note_list_->setData(index, note.content, Model::NoteList::ContentRole);
      note_list_->setData(index, note.time, Model::NoteList::TimeRole);
    }
  }
  editing_note_index_ = -1;
}

void Note::saveEditingNote(const QString& header, const QString& content)
{
  qDebug() << "saveEditingNote: " << header << ", " << content << ", " << editing_note_index_;
  if (editing_note_index_ == -1) {
    qDebug() << "editing note index = -1";
    return;
  }

  Model::Note* editing_note = note_list_->getNote(editing_note_index_);
  if (editing_note->title == header && editing_note->content == content) {
    qDebug() << "is same";
    return;
  }

  Model::Note note {header, content, Note::getTimeNowStr()};

  if (!save(&note)) {
    qWarning() << "Failed to save editing note";
    return;
  }
  qDebug() << "saved success";
  QModelIndex index = note_list_->index(editing_note_index_, 0);

  note_list_->setHeaderData(editing_note_index_, Qt::Vertical, note.title, Model::NoteList::TitleRole);
  note_list_->setData(index, note.content, Model::NoteList::ContentRole);
  note_list_->setData(index, note.time, Model::NoteList::TimeRole);
}

void Note::remove(int index)
{
  note_list_->removeRow(index);
  QFile::remove(note_file_path_[index]);
  note_file_path_.erase(note_file_path_.constBegin() + index);
}

} // namespace Controller
