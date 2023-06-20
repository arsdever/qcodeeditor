#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>

#include "qcodeeditor_line_numbers_area.hpp"

#include "qcodeeditor.hpp"

QCodeEditorLineNumbersArea::QCodeEditorLineNumbersArea(QCodeEditor* parent)
    : QWidget(parent)
    , _codeEditor(parent)
{
}

QCodeEditorLineNumbersArea::~QCodeEditorLineNumbersArea() = default;

QSize QCodeEditorLineNumbersArea::sizeHint() const
{
    if (_codeEditor == nullptr)
        return QWidget::sizeHint();

    // Calculating width
    int digits = 1;
    int max = qMax(1, _codeEditor->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    digits = std::max(4, digits);

    int space =
        13 +
        _codeEditor->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return { space, 0 };
}

void QCodeEditorLineNumbersArea::linesNumberChanged(int newBlockCount)
{
    Q_UNUSED(newBlockCount);

    if (sizeHint() != _sizeHint) {
        _sizeHint = sizeHint();
        emit sizeChanged();
    }

    update();
}

void QCodeEditorLineNumbersArea::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.fillRect(event->rect(), Qt::lightGray);

    auto blockId = _codeEditor->firstVisibleBlockId();
    auto block = _codeEditor->document()->findBlockByNumber(blockId);
    auto top =
        static_cast<int>(_codeEditor->blockRect(block)
                             .translated(0, -_codeEditor->contentOffset().y())
                             .top());
    auto bottom =
        top + static_cast<int>(_codeEditor->blockRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            auto number = QString::number(block.blockNumber() + 1);
            painter.setPen(Qt::black);
            painter.drawText(
                0,
                top,
                width() - 5,
                _codeEditor->fontMetrics().height(),
                Qt::AlignRight,
                number
            );
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(_codeEditor->blockRect(block).height());
    }
}
