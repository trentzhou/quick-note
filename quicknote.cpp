#include "quicknote.h"
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QAudioRecorder>
#include <QUrl>
#include <QDir>
#include <QMediaRecorder>

QuickNote::QuickNote(QObject *parent) : QObject(parent)
{
    recorder = NULL;
    QString home;
    home += getenv("HOME");
    rootFolder = home + "/Documents/quick-note";
    QDir().mkpath(rootFolder);
    todoFilePath = rootFolder + "/todo.txt";
    doneFilePath = rootFolder + "/done.txt";
}

QuickNote::~QuickNote()
{
    stopRecord();
}

void QuickNote::writeText(const QString& text)
{
    QString notePath = getNotePath();
    QTime now = QTime::currentTime();
    QFile file(notePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&file);
        out << "[" << now.toString() << "]:\n" << text <<"\n\n";
        file.flush();
        file.close();
    }
}

QString QuickNote::getAudioNotePath()
{
    QDateTime now = QDateTime::currentDateTime();
    QString folder = rootFolder + "/audio/";
    QDir dir(folder);
    if (!dir.exists())
    {
        dir.mkpath(folder);
    }
    return  folder + now.toString("yyyy-MM-dd-HH-mm-ss") + ".mp3";
}

void QuickNote::startRecord()
{
    QString path = getAudioNotePath();
    audioPath = path;
    if (recorder == NULL)
    {
        recorder = new QAudioRecorder(this);

        QAudioEncoderSettings audioSettings;
        audioSettings.setCodec("audio/mpeg, mpegversion=(int)1, layer=(int)3");
        audioSettings.setQuality(QMultimedia::HighQuality);
        //recorder->setContainerFormat("audio/mpeg, mpegversion=(int)1");
        QString container = "audio/mpeg, mpegversion=(int)1";
        recorder->setEncodingSettings(audioSettings, QVideoEncoderSettings(), container);
        recorder->setOutputLocation(QUrl::fromLocalFile(path));
        recorder->record();
    }
}

void QuickNote::stopRecord()
{
    if (recorder != NULL)
    {
        recorder->stop();
        delete recorder;
        recorder = NULL;
    }
}

bool QuickNote::isRecording()
{
    return recorder != NULL;
}

QString QuickNote::getNotePath()
{
    QDateTime now = QDateTime::currentDateTime();
    QString folder = rootFolder + "/text/";
    QDir dir(folder);
    if (!dir.exists())
    {
        dir.mkpath(folder);
    }
    return  folder + now.toString("yyyy-MM-dd") + ".txt";
}

void QuickNote::addTodo(const QString& text)
{
    QFile todoFile(todoFilePath);
    if (todoFile.open(QIODevice::Append | QIODevice::WriteOnly))
    {
        QTextStream out(&todoFile);
        QDateTime now = QDateTime::currentDateTime();
        out << "[" << now.toString("yyyy-MM-dd HH:mm:ss") << "] " << text << "\n";
        todoFile.close();
    }
}

void QuickNote::finishTodo(const QString& text)
{
    QFile doneFile(doneFilePath);
    if (doneFile.open(QIODevice::Append | QIODevice::WriteOnly))
    {
        QTextStream out(&doneFile);
        QDateTime now = QDateTime::currentDateTime();
        out << text << " [" << now.toString("yyyy-MM-dd HH:mm:ss") << "] " << "\n";
        doneFile.close();
    }
    // save the todo list
    QStringList pendingTodo = getTodoItems(text);
    QFile todoFile(todoFilePath);
    if (todoFile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&todoFile);
        foreach (QString line, pendingTodo)
        {
            out << line << "\n";
        }
        todoFile.close();
    }
}

QStringList QuickNote::getTodoItems(const QString& match)
{
    QFile todoFile(todoFilePath);
    QStringList result;
    if (todoFile.exists())
    {
        // open it
        if (todoFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&todoFile);
            while (true)
            {
                QString line;
                if (!in.readLineInto(&line))
                    break;
                if (line != match)
                    result.append(line);
            }
            todoFile.close();
        }
    }
    return result;
}
