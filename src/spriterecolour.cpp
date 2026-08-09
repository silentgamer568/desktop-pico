#include "spriterecolour.h"

#include <QImage>
#include <algorithm>

namespace {

inline int colourDistanceSquared(int r1, int g1, int b1, int r2, int g2, int b2)
{
    const int dr = r1 - r2;
    const int dg = g1 - g2;
    const int db = b1 - b2;
    return dr * dr + dg * dg + db * db;
}

inline int clampChannel(int value)
{
    return std::clamp(value, 0, 255);
}

inline bool isNeutralBackgroundOrOutline(int r, int g, int b)
{
    constexpr int kWhiteThreshold = 235;
    constexpr int kBlackThreshold = 40;

    const bool nearWhite = r > kWhiteThreshold && g > kWhiteThreshold && b > kWhiteThreshold;
    const bool nearBlack = r < kBlackThreshold && g < kBlackThreshold && b < kBlackThreshold;

    return nearWhite || nearBlack;
}

}

QPixmap recolourSprite(const QPixmap &source, const ColourMap &colourMap, int /*tolerance*/)
{
    if (colourMap.isEmpty()) {
        return source;
    }

    QImage image = source.toImage().convertToFormat(QImage::Format_ARGB32);

    const int width = image.width();
    const int height = image.height();

    struct Entry { int sr, sg, sb, tr, tg, tb; };
    QVector<Entry> entries;
    entries.reserve(colourMap.size());
    for (auto it = colourMap.constBegin(); it != colourMap.constEnd(); ++it) {
        entries.append({
            qRed(it.key()), qGreen(it.key()), qBlue(it.key()),
            qRed(it.value()), qGreen(it.value()), qBlue(it.value())
        });
    }

    for (int y = 0; y < height; ++y) {
        QRgb *row = reinterpret_cast<QRgb *>(image.scanLine(y));

        for (int x = 0; x < width; ++x) {
            const QRgb pixel = row[x];
            const int alpha = qAlpha(pixel);

            if (alpha == 0) {
                continue;
            }

            const int r = qRed(pixel);
            const int g = qGreen(pixel);
            const int b = qBlue(pixel);

            if (isNeutralBackgroundOrOutline(r, g, b)) {
                continue;
            }

            int bestDistance = -1;
            const Entry *best = &entries.first();
            for (const Entry &entry : entries) {
                const int distance = colourDistanceSquared(r, g, b, entry.sr, entry.sg, entry.sb);
                if (bestDistance < 0 || distance < bestDistance) {
                    bestDistance = distance;
                    best = &entry;
                }
            }

            const int newR = clampChannel(best->tr + (r - best->sr));
            const int newG = clampChannel(best->tg + (g - best->sg));
            const int newB = clampChannel(best->tb + (b - best->sb));

            row[x] = qRgba(newR, newG, newB, alpha);
        }
    }

    return QPixmap::fromImage(image);
}