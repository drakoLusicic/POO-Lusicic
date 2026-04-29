#ifndef EDITORPRINCIPAL_H
#define EDITORPRINCIPAL_H

#include "pantalla.h"
#include <QPlainTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include "validadorsintaxis.h"
#include "syntaxhighlighter.h"
#include <QSettings>

class EditorPrincipal : public Pantalla {
    Q_OBJECT
public:
    explicit EditorPrincipal(QWidget *parent = nullptr);

    void inicializarUI() override;
    void conectarEventos() override;
    void cargarDatos() override;
    void validarEstado() override;
    void registrarEvento(const QString &evento) override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private slots:
    void cambiarLenguaje();
    void validarLineaActual();
    void exportarJPG();

private:
    QComboBox *selectorLenguaje;
    QPlainTextEdit *editorTexto;
    QPushButton *exportarButton;
    QLabel *errorLabel;
    QWidget *panelLateral;
    SyntaxHighlighter *highlighter;
    ValidadorSintaxis *validador;
    QSettings settings;
};

#endif // EDITORPRINCIPAL_H