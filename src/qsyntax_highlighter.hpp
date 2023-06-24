#pragma once

#include <QObject>

class QSyntaxHighlighter : public QObject
{
public:
	struct Token
	{
		size_t _startPos;
		size_t _endPos;
		enum class Category
		{
			Comment,
			Keyword,
			Number,
			String,
			Char,
			Operator,
			Identifier,
			Other,
		} _category;
	};

public:
    explicit QSyntaxHighlighter(QObject* parent = nullptr);
    ~QSyntaxHighlighter() override;

	void parse(std::string_view text);

private:
	std::vector<Token> _tokens;
};
