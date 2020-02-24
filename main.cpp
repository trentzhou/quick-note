#include "quicknotedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QuickNoteDialog w;
    w.show();

    return a.exec();
}
