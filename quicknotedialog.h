#ifndef QUICKNOTEDIALOG_H
#define QUICKNOTEDIALOG_H

#include <QDialog>
#include <QAudioRecorder>
#include <QListWidgetItem>
#include "quicknote.h"

namespace Ui {
class QuickNoteDialog;
}

class QuickNoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuickNoteDialog(QWidget *parent = 0);
    ~QuickNoteDialog();

public slots:
    void toggleRecord();
    void saveText();
    void addTodo();
    void refreshTodoList();
    void finishTodo(QListWidgetItem* item);

private:
    Ui::QuickNoteDialog *ui;
    bool recording;
    QuickNote* quickNote;
};

#endif // QUICKNOTEDIALOG_H
