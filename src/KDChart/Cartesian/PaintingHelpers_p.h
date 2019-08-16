/****************************************************************************
** Copyright (C) 2001-2019 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Chart library.
**
** Licensees holding valid commercial KD Chart licenses may use this file in
** accordance with the KD Chart Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.GPL.txt included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.com if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef PAINTINGHELPERS_P_H
#define PAINTINGHELPERS_P_H

#include "KDChartAbstractDiagram_p.h"
#include <KDABLibFakes>

#include <QPointF>
#include <QVector>

class QBrush;
class QModelIndex;
class QPen;
class QPolygonF;

namespace KDChart {

class LineAttributesInfo;
typedef QVector<LineAttributesInfo> LineAttributesInfoList;
class ThreeDLineAttributes;
class ValueTrackerAttributes;

namespace PaintingHelpers {

inline bool isFinite(const QPointF &point)
{
    return !ISINF(point.x()) && !ISNAN(point.x()) && !ISINF(point.y()) && !ISNAN(point.y());
}

const QPointF project( const QPointF& point, const ThreeDLineAttributes& tdAttributes );
void paintPolyline( PaintContext* ctx, const QBrush& brush, const QPen& pen, const QPolygonF& points );
void paintThreeDLines( PaintContext* ctx, AbstractDiagram *diagram, const QModelIndex& index,
                       const QPointF& from, const QPointF& to, const ThreeDLineAttributes& tdAttributes,
                       ReverseMapper* reverseMapper );
void paintValueTracker( PaintContext* ctx, const ValueTrackerAttributes& vt, const QPointF& at );
void paintElements( AbstractDiagram::Private *diagramPrivate, PaintContext* ctx,
                    const LabelPaintCache& lpc, const LineAttributesInfoList& lineList );
void paintAreas( AbstractDiagram::Private* diagramPrivate, PaintContext* ctx, const QModelIndex& index,
                 const QList< QPolygonF >& areas, uint opacity );

void paintSpline( PaintContext* ctx, const QBrush& brush, const QPen& pen, const QPolygonF& points );
void paintAreas( AbstractDiagram::Private* diagramPrivate, PaintContext* ctx, const QModelIndex& index,
                 const QList< QPainterPath >& areas, uint opacity );

}

inline qreal euclideanLength( const QPointF &p )
{
    return sqrt( p.x() * p.x() + p.y() * p.y() );
}

enum class SplineNodePosition { Left, Right };

inline QPointF splineNode( QPointF before, QPointF current, QPointF after, SplineNodePosition position )
{
    if ( ISNAN( before.y() ) || ISNAN ( after.y() ) )
        return current;

    const auto diff = after - before;
    const auto scale = 0.25 * diff.x() / euclideanLength( diff );
    return current + diff * scale * ( position == SplineNodePosition::Left ? 1 : -1 );
}

enum class SplineDirection { Normal, Reverse };

inline QPair<QPointF, QPointF> splineChunk( QPointF before, QPointF a, QPointF b, QPointF after,
                                            SplineDirection direction = SplineDirection::Normal )
{
    QPointF nodeLeft = a;
    QPointF nodeRight = b;

    if ( !ISNAN( before.y() ) ) {
        nodeLeft = splineNode( before, a, b,
                direction == SplineDirection::Normal ? SplineNodePosition::Left : SplineNodePosition::Right );
    }

    if ( !ISNAN( after.y() ) ) {
        nodeRight = splineNode( a, b, after,
                direction != SplineDirection::Normal ? SplineNodePosition::Left : SplineNodePosition::Right );
    }

    return { nodeLeft, nodeRight };
}

inline void addSplineChunkTo( QPainterPath& path, QPointF before, QPointF a, QPointF b, QPointF after,
                              SplineDirection direction = SplineDirection::Normal )
{
    const auto chunk = splineChunk( before, a, b, after, direction );
    path.cubicTo( chunk.first, chunk.second, b );
}

}

#endif
