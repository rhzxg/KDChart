/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDAB-KDChart OR LicenseRef-KDAB-KDChart-US
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
****************************************************************************/

#ifndef PLOTTERDIAGRAMCOMPRESSOR_H
#define PLOTTERDIAGRAMCOMPRESSOR_H

#include <QtCore/QObject>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QPointer>
#include <QtCore/QVector>
#include <QtCore/QDateTime>

#include <cmath>
#include <limits>

namespace KDChart {

class PlotterDiagramCompressor : public QObject
{
    Q_OBJECT
    Q_ENUMS(CompressionMode)
public:
    enum CompressionMode {
        SLOPE = 0,
        DISTANCE,
        BOTH
    };
    class DataPoint
    {
    public:
        DataPoint()
            : key(std::numeric_limits<qreal>::quiet_NaN())
            , value(std::numeric_limits<qreal>::quiet_NaN())
            , hidden(false)
        {
        }
        inline qreal distance(const DataPoint &other)
        {
            const qreal dx = key - other.key;
            const qreal dy = value - other.value;
            return std::sqrt(dx * dx + dy * dy);
        }

        inline bool operator==(const DataPoint &other)
        {
            return key == other.key && value == other.value;
        }

        inline bool operator!=(const DataPoint &other)
        {
            return !(*this == other);
        }

        qreal key;
        qreal value;
        bool hidden;
        QModelIndex index;
    };

    class Iterator
    {
        friend class PlotterDiagramCompressor;

    public:
        Iterator(int dataSet, PlotterDiagramCompressor *parent);
        ~Iterator();
        bool isValid() const;
        Iterator &operator++();
        Iterator operator++(int);
        Iterator &operator+=(int value);
        Iterator &operator--();
        Iterator operator--(int);
        Iterator &operator-=(int value);
        DataPoint operator*();
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        void invalidate();

    protected:
        Iterator(int dataSet, PlotterDiagramCompressor *parent, QVector<DataPoint> buffer);

    private:
        void handleSlopeForward(const DataPoint &dp);
        QPointer<PlotterDiagramCompressor> m_parent;
        QVector<DataPoint> m_buffer;
        int m_index;
        int m_dataset;
        int m_bufferIndex;
        int m_rebuffer;
        QDateTime m_timeOfCreation;
    };

    typedef QVector<DataPoint> DataPointVector;
    class CachePosition
    {
    public:
        CachePosition()
            : first(-1)
            , second(-1)
        {
        }
        CachePosition(int first, int second)
            : first(first)
            , second(second)
        {
        }
        int first;
        int second;

        bool operator==(const CachePosition &rhs) const
        {
            return first == rhs.first && second == rhs.second;
        }
    };
    explicit PlotterDiagramCompressor(QObject *parent = nullptr);
    ~PlotterDiagramCompressor() override;
    Iterator begin(int dataSet);
    Iterator end(int dataSet);
    void setMergeRadius(qreal radius);
    void setMergeRadiusPercentage(qreal radius);
    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model() const;
    DataPoint data(const CachePosition &pos) const;
    int rowCount() const;
    int datasetCount() const;
    void setCompressionModel(CompressionMode value);
    void setMaxSlopeChange(qreal value);
    qreal maxSlopeChange() const;
    void cleanCache();
    QPair<QPointF, QPointF> dataBoundaries() const;
    void setForcedDataBoundaries(const QPair<qreal, qreal> &bounds, Qt::Orientation direction);
Q_SIGNALS:
    void boundariesChanged();
    void rowCountChanged();

private:
    class Private;
    Private *d;
};

}

#endif // PLOTTERDIAGRAMCOMPRESSOR_H
