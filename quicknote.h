#ifndef QUICKNOTE_H
#define QUICKNOTE_H

#include <QObject>
#include <QAudioRecorder>

class QuickNote : public QObject
{
    Q_OBJECT
public:
    explicit QuickNote(QObject *parent = 0);
    ~QuickNote();

public slots:
    void writeText(const QString& text);
    void startRecord();
    void stopRecord();
    bool isRecording();
    void addTodo(const QString& text);
    void finishTodo(const QString& text);
    QStringList getTodoItems(const QString& match="");

public:
    QString getAudioNotePath();
private:
    QAudioRecorder* recorder;
    QString getNotePath();
    QString audioPath;
    QString rootFolder;
    QString todoFilePath;
    QString doneFilePath;
};

#endif // QUICKNOTE_H
