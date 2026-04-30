#include "pintura.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>
#include <QWheelEvent>

Pintura::Pintura(QWidget *parent)
    : QWidget(parent),
      m_isDrawing(false),
      m_undosRemaining(10),
      m_penColor(Qt::cyan),
      m_penWidth(6)
{
    setAttribute(Qt::WA_StaticContents);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);
    setPalette(QPalette(Qt::white));

    QShortcut *undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(undoShortcut, &QShortcut::activated, this, &Pintura::undoLastAction);
}

void Pintura::setStrokes(const QVector<Stroke> &strokes)
{
    m_strokes = strokes;
    m_undosRemaining = 10;
    update();
    updateStatus();
}

QVector<Pintura::Stroke> Pintura::strokes() const
{
    return m_strokes;
}

void Pintura::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    for (const auto &stroke : m_strokes) {
        if (stroke.points.isEmpty()) {
            continue;
        }
        QPen pen(stroke.color, stroke.width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        for (int i = 1; i < stroke.points.size(); ++i) {
            painter.drawLine(stroke.points.at(i - 1), stroke.points.at(i));
        }
    }

    if (m_isDrawing && m_currentStroke.points.size() > 1) {
        QPen pen(m_currentStroke.color, m_currentStroke.width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        for (int i = 1; i < m_currentStroke.points.size(); ++i) {
            painter.drawLine(m_currentStroke.points.at(i - 1), m_currentStroke.points.at(i));
        }
    }
}

void Pintura::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    m_currentStroke.points.clear();
    m_currentStroke.color = m_penColor;
    m_currentStroke.width = m_penWidth;
    m_currentStroke.points.append(event->pos());
    m_isDrawing = true;
    update();
}

void Pintura::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_isDrawing || !(event->buttons() & Qt::LeftButton)) {
        return;
    }
    m_currentStroke.points.append(event->pos());
    update();
}

void Pintura::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_isDrawing || event->button() != Qt::LeftButton) {
        return;
    }
    if (!m_currentStroke.points.isEmpty()) {
        addCurrentStroke();
    }
    m_isDrawing = false;
    update();
}

void Pintura::wheelEvent(QWheelEvent *event)
{
    const int delta = event->angleDelta().y() / 120;
    if (delta == 0) {
        event->ignore();
        return;
    }
    m_penWidth = qBound(1, m_penWidth + delta, 40);
    updateStatus();
    event->accept();
}

void Pintura::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_R:
        m_penColor = Qt::red;
        updateStatus();
        break;
    case Qt::Key_G:
        m_penColor = Qt::green;
        updateStatus();
        break;
    case Qt::Key_B:
        m_penColor = Qt::blue;
        updateStatus();
        break;
    case Qt::Key_Escape:
        clearCanvas();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    event->accept();
}

void Pintura::undoLastAction()
{
    if (m_undosRemaining <= 0 || m_strokes.isEmpty()) {
        return;
    }
    m_strokes.removeLast();
    --m_undosRemaining;
    update();
    updateStatus();
}

void Pintura::clearCanvas()
{
    m_strokes.clear();
    m_currentStroke.points.clear();
    m_isDrawing = false;
    m_undosRemaining = 10;
    update();
    updateStatus();
}

void Pintura::addCurrentStroke()
{
    if (!m_currentStroke.points.isEmpty()) {
        m_strokes.append(m_currentStroke);
        m_currentStroke.points.clear();
        m_undosRemaining = 10;
        emit strokeSaved();
        updateStatus();
    }
}

void Pintura::updateStatus()
{
    emit paintingChanged(m_strokes.count(), m_penColor, m_penWidth);
}
