#pragma once

#include <QPixmap>

#include "colourpresets.h"

QPixmap recolourSprite(const QPixmap &source, const ColourMap &colourMap, int tolerance = 6);
