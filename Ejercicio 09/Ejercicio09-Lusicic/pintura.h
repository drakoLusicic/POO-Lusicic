#ifndef PINTURA_H
#define PINTURA_H

#include <QColor>
#include <QPoint>
#include <QVector>
#include <QWidget>

class Pintura : public QWidget
{
    Q_OBJECT
public:
    struct Stroke {
        QColor color;
        int width;
        QVector<QPoint> points;
    };

    explicit Pintura(QWidget *parent = nullptr);
    void setStrokes(const QVector<Stroke> &strokes);
    QVector<Stroke> strokes() const;

signals:
    void paintingChanged(int strokeCount, const QColor &currentColor, int currentWidth);
    void strokeSaved();

public slots:
    void undoLastAction();
    void clearCanvas();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void addCurrentStroke();
    void updateStatus();

    QVector<Stroke> m_strokes;
    Stroke m_currentStroke;
    bool m_isDrawing;
    int m_undosRemaining;
    QColor m_penColor;
    int m_penWidth;
};

#endif // PINTURA_H
