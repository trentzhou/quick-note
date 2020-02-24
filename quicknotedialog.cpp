#include "quicknotedialog.h"
#include "ui_quicknotedialog.h"
#include <QUrl>

QuickNoteDialog::QuickNoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickNoteDialog)
{
    ui->setupUi(this);
    quickNote = new QuickNote(this);
    recording = false;
    connect(ui->buttonRecordAudio, SIGNAL(clicked(bool)), this, SLOT(toggleRecord()));
    connect(ui->buttonSaveText, SIGNAL(clicked(bool)), this, SLOT(saveText()));
    connect(ui->buttonAddTodo, SIGNAL(clicked(bool)), this, SLOT(addTodo()));
    connect(ui->listTodo, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(finishTodo(QListWidgetItem*)));
    ui->editTodo->setFocus();
    refreshTodoList();
}

QuickNoteDialog::~QuickNoteDialog()
{
    delete quickNote;
    delete ui;
}

void QuickNoteDialog::toggleRecord()
{
    if (!quickNote->isRecording())
    {
        quickNote->startRecord();
        ui->buttonRecordAudio->setText("Stop Record");
        ui->buttonRecordAudio->setBackgroundRole(QPalette::BrightText);
    }
    else
    {
        quickNote->stopRecord();
        ui->buttonRecordAudio->setText("Start Record");
    }
}

void QuickNoteDialog::saveText()
{
    QString text = ui->editNote->toPlainText();
    if (text.length() > 0)
    {
        ui->editNote->setPlainText("");
        quickNote->writeText(text);
    }
}

void QuickNoteDialog::addTodo()
{
    QString text = ui->editTodo->text();
    if (!text.isEmpty())
    {
        quickNote->addTodo(text);
        refreshTodoList();
    }
}

void QuickNoteDialog::refreshTodoList()
{
    // update the todo list
    ui->listTodo->clear();
    QStringList items = quickNote->getTodoItems();
    ui->listTodo->addItems(items);
    ui->editTodo->clear();
    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->listTodo->count(); ++i){
        item = ui->listTodo->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void QuickNoteDialog::finishTodo(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked)
    {
        QString text = item->text();
        quickNote->finishTodo(text);
        refreshTodoList();
    }
}
