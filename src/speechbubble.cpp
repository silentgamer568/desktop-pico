#include "speechbubble.h"

#include <QFontDatabase>

#include "resourcepath.h"

SpeechBubble::SpeechBubble(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowDoesNotAcceptFocus);

    setAttribute(Qt::WA_TranslucentBackground);

    const int fontId = QFontDatabase::addApplicationFont(
        resourcePath("assets/fonts/PixelOperator.ttf"));

    QString fontFamily = "Arial";
    if (fontId != -1) {
        const QStringList families = QFontDatabase::applicationFontFamilies(fontId);
        if (!families.isEmpty()) {
            fontFamily = families.first();
        }
    }

    m_outerLabel = new QLabel(this);
    m_outerLabel->setStyleSheet(
        "QLabel { background-color: white; border: none; }");

    m_label = new QLabel(this);
    m_label->setStyleSheet(QString(
        "QLabel {"
        "  background-color: #ffedd6;"
        "  color: black;"
        "  border: 1px solid black;"
        "  padding: 6px;"
        "  font-family: \"%1\";"
        "  font-size: 16px;"
        "}").arg(fontFamily));

    hide();
}

void SpeechBubble::setMessage(const QString &text)
{
    m_label->setText(text);
    m_label->adjustSize();

    constexpr int outline = 2;

    m_outerLabel->resize(m_label->width() + outline * 2,
                          m_label->height() + outline * 2);

    m_label->move(outline, outline);

    resize(m_outerLabel->width(), m_outerLabel->height());
}
