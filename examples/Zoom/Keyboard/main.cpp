/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
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

#include <QApplication>
#include <KDChartWidget>

#include <QDebug>
#include <QVector>

#include "mainwindow.h"

using namespace KDChart;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow window;

    window.resize(800, 600);

    QVector<qreal> vec0;
    vec0.append(-5);
    vec0.append(-4);
    vec0.append(-3);
    vec0.append(-2);
    vec0.append(-1);
    vec0.append(0);
    vec0.append(1);
    vec0.append(2);
    vec0.append(3);
    vec0.append(4);
    vec0.append(5);
    window.widget->setDataset(0, vec0);

    QVector<qreal> vec1;
    vec1.append(25);
    vec1.append(16);
    vec1.append(9);
    vec1.append(4);
    vec1.append(1);
    vec1.append(0);
    vec1.append(1);
    vec1.append(4);
    vec1.append(9);
    vec1.append(16);
    vec1.append(25);
    window.widget->setDataset(1, vec1);

    QVector<qreal> vec2;
    vec2.append(-125);
    vec2.append(-64);
    vec2.append(-27);
    vec2.append(-8);
    vec2.append(-1);
    vec2.append(0);
    vec2.append(1);
    vec2.append(8);
    vec2.append(27);
    vec2.append(64);
    vec2.append(125);
    window.widget->setDataset(2, vec2);

    window.widget->addHeaderFooter("Zoom Example",
                                   HeaderFooter::Header, Position::North);
    window.widget->addHeaderFooter("zoom in/out: PageDn / PageUp",
                                   HeaderFooter::Footer, Position::North);
    window.widget->addHeaderFooter("pan around: Left / Right / Up / Down",
                                   HeaderFooter::Footer, Position::South);

    window.widget->addLegend(Position::East);

    //window.widget->setType( Widget::Polar );

    window.show();

    return app.exec();
}
