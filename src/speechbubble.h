#pragma once

#include <QLabel>
#include <QWidget>

class SpeechBubble : public QWidget
{
    Q_OBJECT

public:
    explicit SpeechBubble(QWidget *parent = nullptr);

    void setMessage(const QString &text);

private:
    QLabel *m_outerLabel;
    QLabel *m_label;
};
