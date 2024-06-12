#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controller/note.h"


int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  qmlRegisterType<Model::NoteList>("model.notelist", 1, 0, "ModelNoteList");

  Controller::Note* note_controller = new Controller::Note();
  Model::NoteCreator* note_creator = new Model::NoteCreator();

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("note_controller", note_controller);
  engine.rootContext()->setContextProperty("note_creator", note_creator);
  engine.rootContext()->setContextProperty("note_list", note_controller->note_list());
  const QUrl url(u"qrc:/my_note/Main.qml"_qs);
  QObject::connect(
      &engine,
      &QQmlApplicationEngine::objectCreationFailed,
      &app,
      []() { QCoreApplication::exit(-1); },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
