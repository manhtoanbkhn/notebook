#ifndef CONTROLLER_NOTE_H
#define CONTROLLER_NOTE_H

#include <QObject>

#include "model/note.h"


namespace Controller {

constexpr char* kNotesDir = ".";

class Note : public QObject
{
  Q_OBJECT
public:
  explicit Note(QObject *parent = nullptr);
  ~Note();

  void load(const QString& notes_dir);
  bool save(const Model::Note* note);

  Q_INVOKABLE void createNewNote();
  Q_INVOKABLE void remove(int index);
  Q_INVOKABLE void setEditingNote(int index);
  Q_INVOKABLE void finishEditingNote(const Model::Note& note);
  Q_INVOKABLE void saveEditingNote(const QString& header, const QString& content);

  Model::NoteList* note_list() const
  {
    return note_list_;
  }

private:
  Model::NoteList* note_list_;
  QVector<QString> note_file_path_;
  int editing_note_index_;

signals:
  void createEditingNote(const QModelIndex& index);
};

} // namespace Controller

#endif // CONTROLLER_NOTE_H
