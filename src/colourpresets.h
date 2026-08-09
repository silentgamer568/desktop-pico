#pragma once

#include <QColor>
#include <QHash>
#include <QString>
#include <QStringList>

using ColourMap = QHash<QRgb, QRgb>;

class ColourPresets
{
public:
    bool load(const QString &jsonPath);

    QStringList presetNames() const;

    ColourMap presetByName(const QString &name) const;

    QString randomPresetName() const;

    bool isEmpty() const { return m_presets.isEmpty(); }

private:
    QHash<QString, ColourMap> m_presets;
};
