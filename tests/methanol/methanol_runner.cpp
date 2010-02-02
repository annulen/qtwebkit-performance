/*
 * Copyright (C) 2009 Holger Hans Peter Freyther
 * Copyright (C) 2010 University of Szeged
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <QtCore>


#include <qapplication.h>
#include <qwebview.h>
#include <qwebframe.h>
#include <qwebsettings.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWebSettings::globalSettings()->setMaximumPagesInCache(0);
    QWebSettings::globalSettings()->setObjectCacheCapacities(0, 0, 0);

    QWebView view;
    QWebPage& page = *view.page();

    QSize viewportSize(1024, 768);
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    // screensize is a more typical rendering unit for embedded
    page.setPreferredContentsSize(viewportSize);
    const QSize screenSize = QApplication::desktop()->geometry().size();
    view.setFixedSize(screenSize);
    page.setViewportSize(screenSize);
#else
    view.setFixedSize(viewportSize);
    page.setViewportSize(viewportSize);
#endif

    QString firePage = QString("file://%1").arg(QDir().absolutePath() + QDir::separator() + "fire.html");
    view.load(QUrl(firePage));

#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    view.showFullScreen();
#else
    view.show();
#endif

    return app.exec();
}
