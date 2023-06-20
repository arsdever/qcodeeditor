#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QTextBlock>

#include "qcodeeditor.hpp"

#include "qcodeeditor_line_numbers_area.hpp"

QCodeEditor::QCodeEditor(QWidget* parent)
    : QTextEdit(parent)
    , _lineNumbersArea(new QCodeEditorLineNumbersArea(this))
{
    setViewportMargins(_lineNumbersArea->sizeHint().width(), 0, 0, 0);

    connect(
        document(),
        &QTextDocument::blockCountChanged,
        _lineNumbersArea,
        &QCodeEditorLineNumbersArea::linesNumberChanged
    );

    connect(
        _lineNumbersArea,
        &QCodeEditorLineNumbersArea::sizeChanged,
        this,
        [ this ]() {
        setViewportMargins(_lineNumbersArea->sizeHint().width(), 0, 0, 0);
        });
}

QCodeEditor::~QCodeEditor() = default;

QPoint QCodeEditor::contentOffset() const
{
    return { horizontalScrollBar()->value(), verticalScrollBar()->value() };
}

int QCodeEditor::firstVisibleBlockId() const
{
    auto firstBlock = document()->firstBlock();
    if (!firstBlock.isValid())
        return 0;

    auto block = firstBlock;
    QRectF rect = blockRect(block);

    while (!rect.intersects(viewport()->rect())) {
        block = block.next();
        if (!block.isValid())
            return 0;

        rect = blockRect(block);
    }

    return block.blockNumber();
}

QRectF QCodeEditor::blockRect(const QTextBlock& block) const
{
    auto documentLayout = document()->documentLayout();
    return documentLayout->blockBoundingRect(block);
}

void QCodeEditor::resizeEvent(QResizeEvent* event)
{
    QTextEdit::resizeEvent(event);

    auto cr = contentsRect();
    _lineNumbersArea->setGeometry(QRect(
        cr.left(), cr.top(), _lineNumbersArea->sizeHint().width(), cr.height()
    ));
}
