#pragma once

#include <QLabel>
#include <QPixmap>
#include <QPoint>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "colourpresets.h"
#include "dialogue.h"
#include "speechbubble.h"

class Pico : public QWidget
{
    Q_OBJECT

public:
    explicit Pico(QWidget *parent = nullptr);

    void speak(const QString &message);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    struct AnimationEntry
    {
        QString name;
        QPixmap sprite;
        int durationMs;
        int weight;
    };

    void loadAssets();
    void hideSpeech();
    void randomMessage();
    void updateBubblePosition();
    void scheduleNextAnimation();
    void playRandomAnimation();
    void finishAnimation();
    void resetClickCount();
    void startCrying();
    void stopCrying();

    ColourPresets m_presets;
    Dialogue m_dialogue;

    QPixmap m_idleSprite;
    QPixmap m_hopSprite;
    QPixmap m_happySprite;
    QPixmap m_crySprite;
    QPixmap m_dizzySprite;
    QPixmap m_speakSprite;

    QVector<AnimationEntry> m_animations;

    QLabel *m_spriteLabel;
    SpeechBubble *m_bubble;

    bool m_isTalking = false;
    bool m_isCrying = false;
    bool m_playingAnimation = false;
    int m_clickCount = 0;
    QPoint m_dragPosition;

    QTimer m_animationTimer;
    QTimer m_messageTimer;
    QTimer m_clickResetTimer;
};
