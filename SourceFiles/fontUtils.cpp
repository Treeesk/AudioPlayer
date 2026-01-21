#include "fontUtils.h"
#include <QFont>
#include <QFontMetrics>

namespace FontUtils {
    int MaxFontSize(const QString& text, const QRect& rect) {
        int left = 6;
        int right = 50;
        int best = left;
        while (left <= right) {
            int mid = (left + right) / 2;
            QFont font("Times New Roman", mid);
            QFontMetrics fm(font);
            int width_test = fm.horizontalAdvance(text); // количество пикселей для этого текст с таким шрифтом
            int height_test = fm.ascent() + fm.descent(); // высота над базовой линией + расстояние вниз от базовой линией
            if (rect.height() >= height_test && rect.width() >= width_test) {
                best = mid;
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return best;
    }
}
