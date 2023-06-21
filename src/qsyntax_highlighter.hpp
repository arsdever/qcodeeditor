#pragma once

#include <QObject>

class QSyntaxHighlighter : public QObject
{
public:
    explicit QSyntaxHighlighter(QObject* parent = nullptr);
    ~QSyntaxHighlighter() override;
};
