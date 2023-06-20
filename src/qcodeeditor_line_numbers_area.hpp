#pragma once

#include <QWidget>

class QCodeEditor;

class QCodeEditorLineNumbersArea : public QWidget
{
    Q_OBJECT

public:
    explicit QCodeEditorLineNumbersArea(QCodeEditor* parent = nullptr);
    ~QCodeEditorLineNumbersArea() override;

    QSize sizeHint() const override;

public slots:
    void linesNumberChanged(int newBlockCount);

signals:
    void sizeChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QCodeEditor* _codeEditor;
    QSize _sizeHint;
};
