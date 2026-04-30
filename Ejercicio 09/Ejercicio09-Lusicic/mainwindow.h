#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class Pintura;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool initializeDatabase();
    bool validateUser(const QString &username, const QString &password);
    void logAccess(const QString &username, bool success);

private slots:
    void on_btnSave_clicked();
    void on_btnLoad_clicked();
    void on_btnClear_clicked();
    void updatePaintingStatus(int strokeCount, const QColor &currentColor, int currentWidth);
    void onCanvasStrokeSaved();

private:
    void setupStyleSheet();
    void setupPaintingArea();
    bool openDatabase();
    void createTablesIfNeeded();
    void seedDefaultUsers();
    void loadLastDrawing();
    void saveCurrentDrawing();

    Ui::MainWindow *ui;
    Pintura *m_pintura;
    QSqlDatabase m_database;
};

#endif // MAINWINDOW_H
