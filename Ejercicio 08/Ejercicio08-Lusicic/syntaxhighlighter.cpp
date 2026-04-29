#include "syntaxhighlighter.h"
#include <QRegularExpression>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, const QString &lang) : QSyntaxHighlighter(parent), language(lang) {
    keywordFormat.setForeground(QColor("#FFB6C1")); // Rosa suave
    classFormat.setForeground(QColor("#AEC6CF")); // Azul pastel
    functionFormat.setForeground(QColor("#FFD700")); // Amarillo suave
    variableFormat.setForeground(QColor("#98FB98")); // Verde pastel
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    if (language == "C++") {
        // Palabras clave
        QStringList keywords = {"int", "void", "class", "public", "private", "if", "for", "while"};
        for (const QString &keyword : keywords) {
            QRegularExpression expression("\\b" + keyword + "\\b");
            QRegularExpressionMatchIterator i = expression.globalMatch(text);
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                setFormat(match.capturedStart(), match.capturedLength(), keywordFormat);
            }
        }
        // Clases
        QRegularExpression classExpression("\\bclass\\s+(\\w+)");
        QRegularExpressionMatch match = classExpression.match(text);
        if (match.hasMatch()) {
            setFormat(match.capturedStart(1), match.capturedLength(1), classFormat);
        }
        // Funciones
        QRegularExpression funcExpression("\\b(\\w+)\\s*\\(");
        QRegularExpressionMatchIterator fi = funcExpression.globalMatch(text);
        while (fi.hasNext()) {
            QRegularExpressionMatch fmatch = fi.next();
            setFormat(fmatch.capturedStart(1), fmatch.capturedLength(1), functionFormat);
        }
    } else if (language == "Python") {
        // Similar, pero para Python
        QStringList keywords = {"def", "class", "if", "for", "import"};
        for (const QString &keyword : keywords) {
            QRegularExpression expression("\\b" + keyword + "\\b");
            QRegularExpressionMatchIterator i = expression.globalMatch(text);
            while (i.hasNext()) {
                QRegularExpressionMatch match = i.next();
                setFormat(match.capturedStart(), match.capturedLength(), keywordFormat);
            }
        }
    } else if (language == "Java") {
        // Similar a C++
    }
}