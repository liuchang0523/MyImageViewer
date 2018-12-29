#include "QImageViewer.h"
#include <QSize>
#include <QResizeEvent>
#include <QPainter>
#include <QDebug>

QImageViewer::QImageViewer(QWidget*parent /*= 0*/)
    :QWidget(parent)
{
    m_scale = 1.0;
    m_rotation = 0.0;
    m_scaleBasePt = QPointF(0, 0);
    m_buttonLeftPressed = false;
    m_buttonMiddlePressed = false;
    setMouseTracking(true);
}

QImageViewer::~QImageViewer(void)
{
}

void QImageViewer::setImage(QImage &image)
{
    m_sourceImage = image;
    //zoom();
}

QImage QImageViewer::getSelectionImage()
{
    if (m_sourceImage.isNull())
    {
        return QImage();
    }

    QImage img;
    if (m_rotation != 0)
    {
        QTransform t;
        t.rotate(m_rotation);
        img = m_sourceImage.transformed(t);

        QPointF c1(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
        QPointF c2(img.width() / qreal(2), img.height() / qreal(2));

        QRect r = m_selection.translated((c2 - c1).toPoint());
        img = img.copy(r);
    }
    else
    {
        img = m_sourceImage.copy(m_selection);
    }

    return img;
}

void QImageViewer::zoom()
{
    if (!m_sourceImage.isNull())
    {
        double sx = m_sourceImage.width() * 1.01 / this->width();
        double sy = m_sourceImage.height() * 1.01 / this->height();
        m_scale = 1. / qMax(sx, sy);
        qDebug() << QStringLiteral("放大倍数：%1").arg(m_scale);
        qreal x = this->width() - m_sourceImage.width() * m_scale;
        qreal y = this->height() - m_sourceImage.height() * m_scale;
        m_scaleBasePt = QPointF(x / 2, y / 2);
        repaint();
    }
}

QPointF QImageViewer::screenToWorld(QPointF &pt, bool includeRotation/* = true*/)
{
    QTransform t = getTransform(includeRotation);
    QTransform t1 = t.inverted();
    return t1.map(pt);
}

QPointF QImageViewer::worldToScreen(QPointF &pt, bool includeRotation/* = true*/)
{
    QTransform t = getTransform(includeRotation);
    return t.map(pt);
}


void QImageViewer::SetRefinedRect(int x, int y, int width, int height)
{
    m_refined = QRect(x, y, width, height);
    repaint();
}


QVector<QLineF> QImageViewer::getPixelBoundary(QRect &rect)
{
    QVector<QLineF> lines;
    QPointF left, right, top, buttom;
    for (int y = rect.y(); y < rect.y() + rect.height(); ++y)
    {
        left = worldToScreen(QPointF(rect.x() - 1, y));
        right = worldToScreen(QPointF(rect.x() + rect.width() + 1, y));
        lines.append(QLineF(left, right));
    }
    for (int x = rect.x(); x < rect.x() + rect.width(); ++x)
    {
        top = worldToScreen(QPointF(x, rect.y() - 1));
        buttom = worldToScreen(QPointF(x, rect.y() + rect.height() + 1));
        lines.append(QLineF(top, buttom));
    }
    return lines;
}

void QImageViewer::paintEvent(QPaintEvent * event)
{
    QPainter p(this);
    p.fillRect(this->rect(), Qt::gray);

    p.save();
    QTransform transform = getTransform();
    p.setWorldTransform(transform);
    p.drawImage(QPoint(0, 0), m_sourceImage);
    p.restore();

    if (!m_selection.isNull())
    {
        p.save();
        QTransform transform = getTransform(false);
        p.setWorldTransform(transform);
        QVector<QLineF> lines;
        qreal x1 = m_selection.left();
        qreal y1 = m_selection.top();
        qreal x2 = m_selection.right();
        qreal y2 = m_selection.bottom();

        lines.append(QLineF(x1, y1, x2, y1));
        lines.append(QLineF(x2, y1, x2, y2));
        lines.append(QLineF(x2, y2, x1, y2));
        lines.append(QLineF(x1, y2, x1, y1));

        QPen pen(Qt::red);
        pen.setStyle(Qt::PenStyle::DashDotLine);
        QTransform t1 = transform.inverted();
        QPointF pt = t1.map(QPointF(0, 0));
        QPointF pt1 = t1.map(QPointF(1, 0));
        pen.setWidthF(abs(pt1.x() - pt.x()));
        p.setPen(pen);
        p.drawLines(lines);
        p.restore();
    }

    if (!m_refined.isNull())
    {
        p.save();
        QTransform transform = getTransform(false);
        p.setWorldTransform(transform);
        QVector<QLineF> lines;
        qreal x1 = m_refined.left();
        qreal y1 = m_refined.top();
        qreal x2 = m_refined.right();
        qreal y2 = m_refined.bottom();

        lines.append(QLineF(x1, y1, x2, y1));
        lines.append(QLineF(x2, y1, x2, y2));
        lines.append(QLineF(x2, y2, x1, y2));
        lines.append(QLineF(x1, y2, x1, y1));

        QPen pen(Qt::green);
        //pen.setStyle(Qt::PenStyle::DashDotLine);
        QTransform t1 = transform.inverted();
        QPointF pt = t1.map(QPointF(0, 0));
        QPointF pt1 = t1.map(QPointF(1, 0));
        pen.setWidthF(abs(pt1.x() - pt.x()));
        p.setPen(pen);
        p.drawLines(lines);
        p.restore();
    }

    if (m_scale > 30)
    {
        //1.获得视场内点坐标
        QPointF tl = screenToWorld(QPointF(this->x(), this->y()));
        QPointF br = screenToWorld(QPointF(this->size().width(), this->size().height()));
        QRect rectInView(tl.toPoint(), br.toPoint());

        QPen pen(Qt::gray);
        p.setPen(pen);
        QVector<QLineF> lines = getPixelBoundary(rectInView);
        p.drawLines(lines);
        //p.restore();
    }

    if (m_scale > 48)
    {
        //qDebug() << "paint value!";
        //1.获得视场内点坐标
        QPointF tl = screenToWorld(QPointF(this->x(), this->y()));
        QPointF br = screenToWorld(QPointF(this->size().width(), this->size().height()));

        //TODO：获取每个点的像素值
        QRect rectInView(tl.toPoint(), br.toPoint());
        QVector<QPointF> pointsOnScreen;
        QVector<QString> valueString;

        for (int y = rectInView.y(); y < rectInView.y() + rectInView.height(); ++y)
        {
            for (int x = rectInView.x(); x < rectInView.x() + rectInView.width(); ++x)
            {
                QRgb pixValue = m_sourceImage.pixel(x, y);
                QPointF pointOnScreen = worldToScreen(QPointF(x + 0.5, y + 0.5));
                pointsOnScreen.push_back(pointOnScreen);
                valueString.push_back(QString::number(qGray(pixValue)));
            }

        }

        //画出像素值
        QPen pen(Qt::gray);
        p.setPen(pen);
        QFont font;
        font.setFamily("Times");
        font.setPointSize(8);
        p.setFont(font);
        for (int i = 0; i < pointsOnScreen.size(); ++i)
        {
            int strWidth = p.fontMetrics().width(valueString[i]);
            int strHeight = p.fontMetrics().height();
            QPointF fontPoint =
                QPointF(pointsOnScreen[i]
                    - QPointF(strWidth / 2.0, -strHeight / 4.0));
            QRect fontRect(fontPoint.x() - 1,
                fontPoint.y() - strHeight + strHeight / 8,
                strWidth + 2, strHeight + +strHeight / 8);
            p.setPen(Qt::gray);
            p.setBrush(QBrush(Qt::gray));
            p.drawRect(fontRect);
            p.setPen(Qt::black);
            p.drawText(fontPoint, valueString[i]);
        }
    }
}

void QImageViewer::mouseMoveEvent(QMouseEvent * event)
{
    if (m_buttonLeftPressed)
    {
        QPointF curPt = event->pos();
        QPointF pt1 = screenToWorld(m_lastPressedMousePt, false);
        QPointF pt2 = screenToWorld(curPt, false);

        m_selection = QRect(QPoint(qMin(pt1.x(), pt2.x()), qMin(pt1.y(), pt2.y())), QPoint(qMax(pt1.x(), pt2.x()), qMax(pt1.y(), pt2.y())));
        repaint();
    }
    else if (m_buttonMiddlePressed)
    {
        QPointF mv = event->pos() - m_lastPressedMousePt;
        m_scaleBasePt += mv;
        m_lastPressedMousePt = event->pos();
        repaint();
    }
    if (!m_sourceImage.isNull())
    {
        QPointF curPt = event->pos();
        QPointF pt2 = screenToWorld(curPt, false);
        //qDebug() << pt2.toPoint();
        int x = pt2.x();
        int y = pt2.y();
        QRgb pixValue = m_sourceImage.pixel(x, y);
        QString str = "[" + QString::number(x) + "," +
            QString::number(y) + "]---" +
            QString::number(qGray(pixValue));
        emit Coordinate(str);
    }
}

void QImageViewer::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
        m_buttonMiddlePressed = true;
    //m_buttonLeftPressed = true;
    else if (event->button() == Qt::MidButton)
        m_buttonMiddlePressed = true;
    else if (event->button() == Qt::RightButton)
    {
    }
    m_lastPressedMousePt = event->pos();
}

void QImageViewer::mouseReleaseEvent(QMouseEvent * event)
{
    m_buttonLeftPressed = false;
    m_buttonMiddlePressed = false;
    if (event->button() == Qt::LeftButton)
    {
        emit SelectedRect(m_selection);
        //qDebug() << m_selection;
    }
    if (event->button() == Qt::RightButton)
    {
        m_selection = QRect();
        m_refined = QRect();
        emit SelectedRect(m_selection);
        repaint();
    }
}

void QImageViewer::keyPressEvent(QKeyEvent * event)
{

}

void QImageViewer::wheelEvent(QWheelEvent *event)
{
    if (!m_sourceImage.isNull())
    {
        QPoint angle = event->angleDelta();

        if (event->modifiers() == Qt::ShiftModifier)
        {
            m_rotation += angle.y() / 8;
        }
        else
        {
            QPointF pt1 = event->posF();
            QPointF p = worldToScreen(QPointF(0, 0));

            QPointF pt2;
            if (angle.y() > 0)
            {
                pt2 = pt1 + (p - pt1)*0.618;
                m_scale *= 0.618;
            }
            else
            {
                pt2 = pt1 + (p - pt1) / 0.618;
                m_scale /= 0.618;
            }

            QTransform rotationTransform;
            QPointF c(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
            rotationTransform.translate(c.x(), c.y());
            rotationTransform.rotate(m_rotation);
            rotationTransform.translate(-c.x(), -c.y());

            QTransform scalingTransform;
            scalingTransform.scale(m_scale, m_scale);

            QPointF a3 = (scalingTransform.map(rotationTransform.map(QPointF(0, 0)))); //ok

            QPointF pt3 = pt2 - a3;
            m_scaleBasePt = pt3;
        }

        qDebug() << QStringLiteral("放大倍数：%1").arg(m_scale);
        repaint();
    }
}

void QImageViewer::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)
    {
        zoom();
    }
}

QTransform QImageViewer::getTransform(bool includeRotation/* = true*/)
{
    QTransform transform;
    if (!m_sourceImage.isNull())
    {
        transform.translate(m_scaleBasePt.x(), m_scaleBasePt.y());

        transform.scale(m_scale, m_scale);
        if (includeRotation)
        {
            QPointF c(m_sourceImage.width() / qreal(2), m_sourceImage.height() / qreal(2));
            transform.translate(c.x(), c.y());
            transform.rotate(m_rotation);
            transform.translate(-c.x(), -c.y());
        }

    }
    return transform;
}
