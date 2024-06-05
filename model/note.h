#ifndef MODEL_NOTE_H
#define MODEL_NOTE_H

#include <QObject>
#include <QAbstractListModel>
#include <QJsonObject>

namespace Model {

struct Note {
  Q_GADGET
public:
  QString title;
  QString content;

  Q_PROPERTY(QString title MEMBER title)
  Q_PROPERTY(QString content MEMBER content)

  QJsonObject toJson() const {
    QJsonObject json;
    json["title"] = title;
    json["content"] = content;
    return json;
  }
};

class NoteCreator : public QObject {
  Q_OBJECT
public slots:
  Note create() { return Note(); }
};

class NoteList : public QAbstractListModel
{
 Q_OBJECT
 public:
  explicit NoteList(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;
  void removeRow(int row, const QModelIndex &parent = QModelIndex());

  void appendData(const Note& note);
  const QVector<Note>& notes() const;

public slots:
  Note* getNote(int index);

 signals:


 private:
  QVector<Note> notes_;
};

}

#endif // MODEL_NOTE_H
