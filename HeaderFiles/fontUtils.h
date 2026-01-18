#ifndef FONTUTILS_H
#define FONTUTILS_H
#include <QString>
#include <QRect>

namespace FontUtils {
    // максимальный размер шрифта для конкретного текста и области
    int MaxFontSize(const QString& text, const QRect& rect);
}

#endif // FONTUTILS_H
