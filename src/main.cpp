#include <QApplication>
#include <QTimer>

#include "pico.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Pico pico;
    pico.show();

    QTimer::singleShot(1000, &pico, [&pico]() {
        pico.speak("Hi! I'm your little desktop Pico Buddy!");
    });

    QTimer::singleShot(8000, &pico, [&pico]() {
        pico.speak("Drag 'n drop me to where I should sit.");
    });

    return app.exec();
}
