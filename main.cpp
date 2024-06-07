#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Устанавливаем стиль Fusion, чтобы избежать адаптивных изменений темы
    a.setStyle("Fusion");

    // Создаем объект палитры для белого цвета
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::AlternateBase, Qt::lightGray);
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, Qt::white);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(0, 0, 255));

    palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    palette.setColor(QPalette::HighlightedText, Qt::white);

    // Применяем палитру к приложению
    a.setPalette(palette);

    MainWindow w;
    w.show();
    return a.exec();
}
