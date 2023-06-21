#pragma once

#include <QTextEdit>

class QCodeEditorLineNumbersArea;
class QTextBlock;

class QCodeEditor : public QTextEdit
{
public:
    explicit QCodeEditor(QWidget* parent = nullptr);
    ~QCodeEditor() override;

    QPoint contentOffset() const;
    int firstVisibleBlockId() const;
    QRectF blockRect(const QTextBlock& block) const;

protected:
    void resizeEvent(QResizeEvent* event) override;
    void scrollContentsBy(int dx, int dy) override;

private:
    QCodeEditorLineNumbersArea* _lineNumbersArea = nullptr;
};
