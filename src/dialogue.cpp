#include "dialogue.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

namespace {

QStringList toStringList(const QJsonArray &array)
{
    QStringList result;
    result.reserve(array.size());
    for (const QJsonValue &value : array) {
        result.append(value.toString());
    }
    return result;
}

QString pickRandom(const QStringList &list, const QString &fallback)
{
    if (list.isEmpty()) {
        return fallback;
    }
    const int index = QRandomGenerator::global()->bounded(list.size());
    return list.at(index);
}

}

bool Dialogue::load(const QString &jsonPath)
{
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return false;
    }

    const QJsonObject root = doc.object();

    randomMessages = toStringList(root.value("random_messages").toArray());
    clickMessages = toStringList(root.value("click_messages").toArray());
    startupMessages = toStringList(root.value("startup_messages").toArray());

    const QString cry = root.value("cry_message").toString();
    if (!cry.isEmpty()) {
        cryMessage = cry;
    }

    return !randomMessages.isEmpty() || !clickMessages.isEmpty();
}

QString Dialogue::randomRandomMessage() const
{
    return pickRandom(randomMessages, "...");
}

QString Dialogue::randomClickMessage() const
{
    return pickRandom(clickMessages, "...");
}
