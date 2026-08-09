#include "pico.h"

#include <QApplication>
#include <QMouseEvent>
#include <QRandomGenerator>

#include "resourcepath.h"
#include "spriterecolour.h"

namespace {
constexpr int kSpriteSize = 64;
constexpr int kAnimIntervalMinMs = 10000;
constexpr int kAnimIntervalMaxMs = 20000;
constexpr int kMessageIntervalMinMs = 600000;
constexpr int kMessageIntervalMaxMs = 900000;
constexpr int kSpeechDurationMs = 5000;
constexpr int kCryDurationMs = 5000;
constexpr int kClickResetMs = 2000;
constexpr int kClickCryThreshold = 5;
}

Pico::Pico(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowDoesNotAcceptFocus);

    setAttribute(Qt::WA_TranslucentBackground);
    resize(kSpriteSize, kSpriteSize);
    setCursor(Qt::OpenHandCursor);

    m_spriteLabel = new QLabel(this);
    m_spriteLabel->resize(kSpriteSize, kSpriteSize);

    m_bubble = new SpeechBubble();

    loadAssets();

    m_spriteLabel->setPixmap(m_idleSprite);

    m_animationTimer.setSingleShot(true);
    connect(&m_animationTimer, &QTimer::timeout, this, &Pico::playRandomAnimation);
    scheduleNextAnimation();

    m_messageTimer.setSingleShot(true);
    connect(&m_messageTimer, &QTimer::timeout, this, &Pico::randomMessage);
    m_messageTimer.start(QRandomGenerator::global()->bounded(
        kMessageIntervalMinMs, kMessageIntervalMaxMs));

    m_clickResetTimer.setSingleShot(true);
    connect(&m_clickResetTimer, &QTimer::timeout, this, &Pico::resetClickCount);
}

void Pico::loadAssets()
{
    m_presets.load(resourcePath("data/colours.json"));
    m_dialogue.load(resourcePath("data/dialogue.json"));

    const QString presetName = m_presets.randomPresetName();
    qInfo("Pico: %s", qUtf8Printable(presetName));

    const ColourMap colourMap = m_presets.presetByName(presetName);

    const QPixmap baseIdle(resourcePath("assets/sprites/MelonIdle.png"));
    const QPixmap baseHop(resourcePath("assets/sprites/MelonTipToe.png"));
    const QPixmap baseHappy(resourcePath("assets/sprites/MelonHappy.png"));
    const QPixmap baseCry(resourcePath("assets/sprites/MelonCry.png"));
    const QPixmap baseDizzy(resourcePath("assets/sprites/MelonDizzy.png"));
    const QPixmap baseSpeak(resourcePath("assets/sprites/MelonSpeak.png"));

    m_idleSprite = recolourSprite(baseIdle, colourMap);
    m_hopSprite = recolourSprite(baseHop, colourMap);
    m_happySprite = recolourSprite(baseHappy, colourMap);
    m_crySprite = recolourSprite(baseCry, colourMap);
    m_dizzySprite = recolourSprite(baseDizzy, colourMap);
    m_speakSprite = recolourSprite(baseSpeak, colourMap);

    m_animations = {
        { "hop", m_hopSprite, 1000, 500 },
        { "happy", m_happySprite, 1000, 500 },
        { "dizzy", m_dizzySprite, 1200, 100 },
        { "speak", m_speakSprite, 5000, 10 },
    };
}


void Pico::speak(const QString &message)
{
    if (m_isCrying) {
        return;
    }

    m_isTalking = true;
    m_spriteLabel->setPixmap(m_speakSprite);

    m_bubble->setMessage(message);
    updateBubblePosition();
    m_bubble->show();

    QTimer::singleShot(kSpeechDurationMs, this, &Pico::hideSpeech);
}

void Pico::hideSpeech()
{
    m_bubble->hide();
    m_isTalking = false;

    if (!m_playingAnimation) {
        m_spriteLabel->setPixmap(m_idleSprite);
    }
}

void Pico::randomMessage()
{
    speak(m_dialogue.randomRandomMessage());

    m_messageTimer.start(QRandomGenerator::global()->bounded(
        kMessageIntervalMinMs, kMessageIntervalMaxMs));
}


void Pico::updateBubblePosition()
{
    m_bubble->move(
        x() + width() / 2 - m_bubble->width() / 2,
        y() - m_bubble->height() - 10);
}


void Pico::scheduleNextAnimation()
{
    m_animationTimer.start(QRandomGenerator::global()->bounded(
        kAnimIntervalMinMs, kAnimIntervalMaxMs));
}

void Pico::playRandomAnimation()
{
    if (m_isTalking || m_isCrying) {
        scheduleNextAnimation();
        return;
    }

    int totalWeight = 0;
    for (const AnimationEntry &entry : m_animations) {
        totalWeight += entry.weight;
    }

    int roll = QRandomGenerator::global()->bounded(totalWeight);
    const AnimationEntry *chosen = &m_animations.first();
    for (const AnimationEntry &entry : m_animations) {
        if (roll < entry.weight) {
            chosen = &entry;
            break;
        }
        roll -= entry.weight;
    }

    m_playingAnimation = true;
    m_spriteLabel->setPixmap(chosen->sprite);

    QTimer::singleShot(chosen->durationMs, this, &Pico::finishAnimation);
}

void Pico::finishAnimation()
{
    m_playingAnimation = false;

    if (m_isCrying) {
        return;
    }

    m_spriteLabel->setPixmap(m_isTalking ? m_speakSprite : m_idleSprite);
    scheduleNextAnimation();
}


void Pico::resetClickCount()
{
    m_clickCount = 0;
}

void Pico::startCrying()
{
    m_isCrying = true;
    m_clickCount = 0;

    m_bubble->setMessage(m_dialogue.cryMessage);
    updateBubblePosition();
    m_bubble->show();

    m_spriteLabel->setPixmap(m_crySprite);

    QTimer::singleShot(kCryDurationMs, this, &Pico::stopCrying);
}

void Pico::stopCrying()
{
    m_isCrying = false;
    m_bubble->hide();
    m_spriteLabel->setPixmap(m_idleSprite);
}


void Pico::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void Pico::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        updateBubblePosition();
        event->accept();
    }
}


void Pico::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        QApplication::quit();
        return;
    }

    if (event->button() != Qt::LeftButton) {
        return;
    }

    if (m_isCrying) {
        return;
    }

    m_clickCount++;
    m_clickResetTimer.start(kClickResetMs);

    if (m_clickCount >= kClickCryThreshold) {
        startCrying();
        return;
    }

    speak(m_dialogue.randomClickMessage());
}


void Pico::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        QApplication::quit();
    }
}
