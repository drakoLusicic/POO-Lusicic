#include "editorprincipal.h"
#include "validadorcpp.h"
#include "validadorpython.h"
#include "validadorjava.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QFocusEvent>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QMessageBox>
#include <QSplitter>

EditorPrincipal::EditorPrincipal(QWidget *parent) : Pantalla(parent), validador(nullptr), settings("config.ini", QSettings::IniFormat) {
    inicializarUI();
    conectarEventos();
    cargarDatos();
}

void EditorPrincipal::inicializarUI() {
    setWindowTitle("Editor Multilenguaje");

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // Panel lateral
    panelLateral = new QWidget();
    panelLateral->setFixedWidth(280);
    panelLateral->setStyleSheet("background: #15181F; border: 1px solid #1F2430; border-radius: 24px; padding: 18px;");

    QVBoxLayout *lateralLayout = new QVBoxLayout(panelLateral);
    lateralLayout->setSpacing(18);

    QLabel *foto = new QLabel();
    foto->setFixedSize(180, 180);
    foto->setAlignment(Qt::AlignCenter);
    foto->setStyleSheet("background: #1C212A; border: 1px solid #3E3F4A; border-radius: 18px; color: #E6E8F0; font-size: 15px; font-weight: 700;");
    // Imagen empaquetada en recursos Qt; coloca foto.png en assets/ y se incluye en el .exe.
    QPixmap profilePixmap(":/assets/foto.png");
    if (!profilePixmap.isNull()) {
        foto->setPixmap(profilePixmap.scaled(foto->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        foto->setText("Foto");
    }

    QLabel *nombre = new QLabel("Drako Lusicic");
    nombre->setStyleSheet("color: #FFFFFF; font-size: 14px; font-weight: 700; max-height: 24px; margin-top: 8px;");
    QLabel *descripcion = new QLabel("Desarrollador Qt & POO con foco en interfaces modernas.");
    descripcion->setWordWrap(true);
    descripcion->setStyleSheet("color: #A8B2D1; font-size: 13px; line-height: 18px;");
    QLabel *habilidades = new QLabel("C++, Python, Java, Qt, UI/UX");
    habilidades->setStyleSheet("color: #E6E8F0; font-size: 13px; background: #171B22; border: 1px solid #2A2D36; border-radius: 14px; padding: 10px;");
    QLabel *contacto = new QLabel("drako@example.com");
    contacto->setStyleSheet("color: #A8B2D1; font-size: 13px; padding-top: 6px;");

    lateralLayout->addWidget(foto);
    lateralLayout->addWidget(nombre);
    lateralLayout->addWidget(descripcion);
    lateralLayout->addWidget(habilidades);
    lateralLayout->addWidget(contacto);

    // Editor
    QWidget *editorWidget = new QWidget();
    QVBoxLayout *editorLayout = new QVBoxLayout(editorWidget);

    selectorLenguaje = new QComboBox();
    selectorLenguaje->addItems({"C++", "Python", "Java"});

    editorTexto = new QPlainTextEdit();
    highlighter = new SyntaxHighlighter(editorTexto->document(), "C++");

    exportarButton = new QPushButton("Exportar a JPG");
    exportarButton->setStyleSheet("background: #202530; border: 1px solid #32394A; border-radius: 18px; color: #E6E8F0; padding: 14px 22px; font-weight: 600;");

    errorLabel = new QLabel("");
    errorLabel->setStyleSheet("color: #F6C8FF; font-weight: 600; font-size: 12px; background: rgba(126, 94, 255, 0.14); border-radius: 12px; padding: 8px; min-height: 26px;");

    editorLayout->addWidget(selectorLenguaje);
    editorLayout->addWidget(editorTexto, 1);
    editorLayout->addWidget(exportarButton);
    editorLayout->addWidget(errorLabel);

    splitter->addWidget(panelLateral);
    splitter->addWidget(editorWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);
}

void EditorPrincipal::conectarEventos() {
    connect(selectorLenguaje, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &EditorPrincipal::cambiarLenguaje);
    connect(editorTexto, &QPlainTextEdit::cursorPositionChanged, this, &EditorPrincipal::validarLineaActual);
    connect(exportarButton, &QPushButton::clicked, this, &EditorPrincipal::exportarJPG);
}

void EditorPrincipal::cargarDatos() {
    QString def = settings.value("lenguaje_defecto", "C++").toString();
    selectorLenguaje->setCurrentText(def);
    cambiarLenguaje();
}

void EditorPrincipal::validarEstado() {
    // No aplica
}

void EditorPrincipal::registrarEvento(const QString &evento) {
    registrarEventoBase("Editor: " + evento);
}

void EditorPrincipal::cambiarLenguaje() {
    QString lang = selectorLenguaje->currentText();
    delete highlighter;
    highlighter = new SyntaxHighlighter(editorTexto->document(), lang);
    delete validador;
    QString ejemplo;
    if (lang == "C++") {
        validador = new ValidadorCpp();
        ejemplo = "#include <iostream>\n\nint main() {\n    std::cout << \"Hello World!\" << std::endl;\n    return 0;\n}";
    } else if (lang == "Python") {
        validador = new ValidadorPython();
        ejemplo = "def main():\n    print(\"Hello World!\")\n\nif __name__ == \"__main__\":\n    main()";
    } else if (lang == "Java") {
        validador = new ValidadorJava();
        ejemplo = "public class Main {\n    public static void main(String[] args) {\n        System.out.println(\"Hello World!\");\n    }\n}";
    }
    editorTexto->setPlainText(ejemplo);
    registrarEvento("Cambio de lenguaje a " + lang);
}

void EditorPrincipal::validarLineaActual() {
    QTextCursor cursor = editorTexto->textCursor();
    int line = cursor.blockNumber();
    QString text = cursor.block().text();
    if (validador && !validador->validar(text)) {
        errorLabel->setText("Error en línea " + QString::number(line + 1) + ": " + validador->mensajeError());
        // Resaltar en rojo
        QTextCharFormat format;
        format.setForeground(Qt::red);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.setCharFormat(format);
    } else {
        errorLabel->setText("");
    }
}

void EditorPrincipal::exportarJPG() {
    QString ruta = settings.value("ruta_exportacion").toString();
    QPixmap pixmap(editorTexto->size());
    editorTexto->render(&pixmap);
    pixmap.save(ruta, "JPG");
    QMessageBox::information(this, "Exportado", "Código exportado a " + ruta);
    registrarEvento("Exportación a JPG");
}

void EditorPrincipal::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
        return;
    }
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_S) {
        // Guardar
        registrarEvento("Guardar (Ctrl+S)");
    }
    QWidget::keyPressEvent(event);
}

void EditorPrincipal::mousePressEvent(QMouseEvent *event) {
    registrarEvento("Clic del mouse en editor");
    QWidget::mousePressEvent(event);
}

void EditorPrincipal::resizeEvent(QResizeEvent *event) {
    registrarEvento("Redimensionar ventana");
    QWidget::resizeEvent(event);
}

void EditorPrincipal::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Salir", "¿Guardar antes de salir?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        // Guardar
        registrarEvento("Guardar al salir");
        event->accept();
    } else if (reply == QMessageBox::No) {
        event->accept();
    } else {
        event->ignore();
    }
}

void EditorPrincipal::focusInEvent(QFocusEvent *event) {
    registrarEvento("Foco en editor");
    QWidget::focusInEvent(event);
}

void EditorPrincipal::focusOutEvent(QFocusEvent *event) {
    validarLineaActual();
    registrarEvento("Foco fuera de editor");
    QWidget::focusOutEvent(event);
}