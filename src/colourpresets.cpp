#include "colourpresets.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>

bool ColourPresets::load(const QString &jsonPath)
{
    m_presets.clear();

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        return false;
    }

    const QJsonObject root = doc.object();

    for (auto presetIt = root.constBegin(); presetIt != root.constEnd(); ++presetIt) {
        if (!presetIt.value().isObject()) {
            continue;
        }

        ColourMap map;
        const QJsonObject mapping = presetIt.value().toObject();

        for (auto colourIt = mapping.constBegin(); colourIt != mapping.constEnd(); ++colourIt) {
            const QColor src(colourIt.key());
            const QColor dst(colourIt.value().toString());

            if (src.isValid() && dst.isValid()) {
                map.insert(src.rgb(), dst.rgb());
            }
        }

        if (!map.isEmpty()) {
            m_presets.insert(presetIt.key(), map);
        }
    }

    return !m_presets.isEmpty();
}

QStringList ColourPresets::presetNames() const
{
    return m_presets.keys();
}

ColourMap ColourPresets::presetByName(const QString &name) const
{
    return m_presets.value(name);
}

QString ColourPresets::randomPresetName() const
{
    const QStringList names = presetNames();
    if (names.isEmpty()) {
        return QString();
    }

    const int index = QRandomGenerator::global()->bounded(names.size());
    return names.at(index);
}
