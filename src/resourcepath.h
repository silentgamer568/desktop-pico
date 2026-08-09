#pragma once

#include <QCoreApplication>
#include <QDir>
#include <QString>

inline QString resourcePath(const QString &relativePath)
{
    static const QString baseDir =
        QCoreApplication::applicationDirPath();

    return QDir(baseDir).filePath(relativePath);
}
