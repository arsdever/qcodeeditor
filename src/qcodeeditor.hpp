#pragma once

#include <QTextEdit>

class QCodeEditor : public QTextEdit
{
public:
    explicit QCodeEditor(QWidget* parent = nullptr);
    ~QCodeEditor() override;
};
