#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pintura.h"
#include "logindialog.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextStream>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_pintura(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Ejercicio 09 - Pintura Futurista");
    setMinimumSize(900, 600);
    setupStyleSheet();
    setupPaintingArea();
    loadLastDrawing();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::initializeDatabase()
{
    if (!openDatabase()) {
        QMessageBox::critical(this, tr("Error de base"), tr("No se pudo abrir la base de datos SQLite."));
        return false;
    }
    createTablesIfNeeded();
    seedDefaultUsers();
    return true;
}

bool MainWindow::openDatabase()
{
    const QString dbPath = QDir(QCoreApplication::applicationDirPath()).filePath("ejercicio09.db");
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbPath);
    if (!m_database.open()) {
        QMessageBox::critical(this, tr("Error SQLite"), tr("No se pudo abrir la base de datos: %1").arg(m_database.lastError().text()));
        return false;
    }
    return true;
}

void MainWindow::createTablesIfNeeded()
{
    QSqlQuery query(m_database);
    query.exec("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS dibujos (id INTEGER PRIMARY KEY AUTOINCREMENT, created_at TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS coordenadas (id INTEGER PRIMARY KEY AUTOINCREMENT, dibujo_id INTEGER NOT NULL, stroke_index INTEGER NOT NULL, point_index INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, pen_width INTEGER NOT NULL, color TEXT NOT NULL, FOREIGN KEY(dibujo_id) REFERENCES dibujos(id))");
}

void MainWindow::seedDefaultUsers()
{
    QSqlQuery query(m_database);
    if (!query.exec("SELECT COUNT(*) FROM users")) {
        return;
    }
    if (query.next() && query.value(0).toInt() == 0) {
        QSqlQuery insert(m_database);
        insert.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        insert.addBindValue("admin");
        insert.addBindValue("admin123");
        insert.exec();

        insert.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
        insert.addBindValue("usuario");
        insert.addBindValue("lusicic");
        insert.exec();
    }
}

bool MainWindow::validateUser(const QString &username, const QString &password)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);
    if (!query.exec() || !query.next()) {
        return false;
    }
    return query.value(0).toString() == password;
}

void MainWindow::logAccess(const QString &username, bool success)
{
    const QString logFile = QDir(QCoreApplication::applicationDirPath()).filePath("accesos.log");
    QFile file(logFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString(Qt::ISODate) << " - "
        << username << " - " << (success ? "Éxito" : "Fallo") << "\n";
}

void MainWindow::setupStyleSheet()
{
    setStyleSheet(
        "QMainWindow { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #09121f, stop:1 #1a2332); }"
        "QFrame { background: rgba(20, 28, 44, 220); border: 1px solid #29b6f6; border-radius: 14px; }"
        "QPushButton { color: #e3f2fd; background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1565c0, stop:1 #0288d1); border-radius: 12px; min-height: 36px; }"
        "QPushButton:hover { background: #00b0ff; }"
        "QLabel { color: #e0f7fa; }"
        "QLineEdit { background: rgba(255, 255, 255, 0.08); border: 1px solid #4dd0e1; color: #ffffff; border-radius: 10px; padding: 8px; }"
    );
}

void MainWindow::setupPaintingArea()
{
    m_pintura = new Pintura(ui->paintPlaceholder);
    m_pintura->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if (!ui->paintPlaceholder->layout()) {
        auto layout = new QVBoxLayout(ui->paintPlaceholder);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(m_pintura);
    } else {
        ui->paintPlaceholder->layout()->addWidget(m_pintura);
    }

    connect(m_pintura, &Pintura::paintingChanged, this, &MainWindow::updatePaintingStatus);
    connect(m_pintura, &Pintura::strokeSaved, this, &MainWindow::onCanvasStrokeSaved);
    connect(ui->btnClear, &QPushButton::clicked, m_pintura, &Pintura::clearCanvas);
    m_pintura->setFocus();
}

void MainWindow::loadLastDrawing()
{
    QSqlQuery query(m_database);
    if (!query.exec("SELECT id FROM dibujos ORDER BY id DESC LIMIT 1")) {
        return;
    }
    if (!query.next()) {
        return;
    }
    const int dibujoId = query.value(0).toInt();
    query.prepare("SELECT stroke_index, point_index, x, y, pen_width, color FROM coordenadas WHERE dibujo_id = ? ORDER BY stroke_index, point_index");
    query.addBindValue(dibujoId);
    if (!query.exec()) {
        return;
    }

    QVector<Pintura::Stroke> strokes;
    int currentStrokeIndex = -1;
    Pintura::Stroke currentStroke;
    while (query.next()) {
        const int strokeIndex = query.value(0).toInt();
        if (strokeIndex != currentStrokeIndex) {
            if (currentStrokeIndex >= 0) {
                strokes.append(currentStroke);
            }
            currentStrokeIndex = strokeIndex;
            currentStroke = Pintura::Stroke();
            currentStroke.width = query.value(4).toInt();
            currentStroke.color = QColor(query.value(5).toString());
            currentStroke.points.clear();
        }
        currentStroke.points.append(QPoint(query.value(2).toInt(), query.value(3).toInt()));
    }
    if (currentStroke.points.size() > 0) {
        strokes.append(currentStroke);
    }
    if (!strokes.isEmpty()) {
        m_pintura->setStrokes(strokes);
    }
}

void MainWindow::saveCurrentDrawing()
{
    const QVector<Pintura::Stroke> strokes = m_pintura->strokes();
    if (strokes.isEmpty()) {
        QMessageBox::information(this, tr("Guardar dibujo"), tr("No hay nada para guardar."));
        return;
    }
    QSqlQuery query(m_database);
    m_database.transaction();
    query.prepare("INSERT INTO dibujos (created_at) VALUES (?)");
    query.addBindValue(QDateTime::currentDateTime().toString(Qt::ISODate));
    if (!query.exec()) {
        m_database.rollback();
        QMessageBox::warning(this, tr("Guardar dibujo"), tr("Error al guardar el dibujo."));
        return;
    }
    const int dibujoId = query.lastInsertId().toInt();
    query.prepare("INSERT INTO coordenadas (dibujo_id, stroke_index, point_index, x, y, pen_width, color) VALUES (?, ?, ?, ?, ?, ?, ?)");
    for (int strokeIndex = 0; strokeIndex < strokes.size(); ++strokeIndex) {
        const Pintura::Stroke &stroke = strokes.at(strokeIndex);
        for (int pointIndex = 0; pointIndex < stroke.points.size(); ++pointIndex) {
            query.addBindValue(dibujoId);
            query.addBindValue(strokeIndex);
            query.addBindValue(pointIndex);
            query.addBindValue(stroke.points.at(pointIndex).x());
            query.addBindValue(stroke.points.at(pointIndex).y());
            query.addBindValue(stroke.width);
            query.addBindValue(stroke.color.name());
            if (!query.exec()) {
                m_database.rollback();
                QMessageBox::warning(this, tr("Guardar dibujo"), tr("Error al insertar coordenadas."));
                return;
            }
            query.finish();
        }
    }
    m_database.commit();
    QMessageBox::information(this, tr("Guardar dibujo"), tr("Dibujo guardado correctamente."));
}

void MainWindow::on_btnSave_clicked()
{
    saveCurrentDrawing();
}

void MainWindow::on_btnLoad_clicked()
{
    loadLastDrawing();
}

void MainWindow::on_btnClear_clicked()
{
    m_pintura->clearCanvas();
}

void MainWindow::updatePaintingStatus(int strokeCount, const QColor &currentColor, int currentWidth)
{
    ui->labelCount->setText(tr("Trazos: %1").arg(strokeCount));
    ui->labelColor->setText(tr("Color: %1").arg(currentColor.name().toUpper()));
    ui->labelWidth->setText(tr("Grosor: %1 px").arg(currentWidth));
}

void MainWindow::onCanvasStrokeSaved()
{
    ui->labelMessage->setText(tr("Pulse Guardar para persistir el trazo en SQLite."));
}
