#pragma once

#include <QString>
#include <QStringList>

struct Dialogue
{
    QStringList randomMessages;
    QStringList clickMessages;
    QStringList startupMessages;
    QString cryMessage = "My head hurts...";

    bool load(const QString &jsonPath);

    QString randomRandomMessage() const;
    QString randomClickMessage() const;
};
