#include "fontUtils.h"
#include <QFont>
#include <QFontMetrics>

namespace FontUtils {
    int MaxFontSize(const QString& text, const QRect& rect) {
        int left = 6;
        int right = 30;
        while (left <= right) {
            int mid = (left + right) / 2;
            QFont font("Times New Roman", mid);
            QFontMetrics fm(font);
            QRect test = fm.boundingRect(rect, Qt::TextWordWrap | Qt::AlignCenter, text);
            if (rect.height() == test.height() && rect.width() == test.width()) {
                return mid;
            }
            else if (rect.height() < test.height() || rect.width() < test.width()) {
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }
        return left;
    }
}
