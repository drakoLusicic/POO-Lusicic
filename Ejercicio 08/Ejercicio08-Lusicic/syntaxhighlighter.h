#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    SyntaxHighlighter(QTextDocument *parent, const QString &language);

protected:
    void highlightBlock(const QString &text) override;

private:
    QString language;
    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat variableFormat;
};

#endif // SYNTAXHIGHLIGHTER_H