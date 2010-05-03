/*
 * Copyright (C) 2010 Nokia
 * Copyright (C) 2009 Holger Hans Peter Freyther
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

#include <QtTest/QtTest>

#include "benchmark.h"
#include "common_init.h"
#include "databasenetworkaccessmanager.h"
#include "databasetests.h"
#include "webpage.h"

#include <qdesktopwidget.h>
#include <qwebframe.h>
#include <qwebview.h>
#include <qpainter.h>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QIODevice>
#include <QList>

class tst_Cycler : public QObject
{
    Q_OBJECT

public:
    ~tst_Cycler();

public Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private Q_SLOTS:
    void load_data();
    void load();

private:
    QWebView* m_view;
    WebPage* m_page;
};

class PipeliningNetworkAccessManager : public QNetworkAccessManager {
    Q_OBJECT

public:
    PipeliningNetworkAccessManager();
    QNetworkReply* createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData);
protected:
};

PipeliningNetworkAccessManager::PipeliningNetworkAccessManager() : QNetworkAccessManager()
{
}

QNetworkReply* PipeliningNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
    QNetworkRequest request = req;
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true); // FIXME
    QNetworkReply *r =  QNetworkAccessManager::createRequest(op, request, outgoingData);
    return r;
}

tst_Cycler::~tst_Cycler()
{
    benchmarkOutput();
}

void tst_Cycler::initTestCase()
{
    QWebSettings::globalSettings()->setMaximumPagesInCache(0);
    QWebSettings::globalSettings()->setObjectCacheCapacities(0, 0, 0);

    m_view = new QWebView;
    m_page = new WebPage(m_view);
    m_view->setPage(m_page);

    QSize viewportSize(1024, 768);
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    m_page->setPreferredContentsSize(viewportSize);
    const QSize screenSize = QApplication::desktop()->geometry().size();
    m_view->setFixedSize(screenSize);
    m_page->setViewportSize(screenSize);
#else
    m_view->setFixedSize(viewportSize);
    m_page->setViewportSize(viewportSize);
#endif

    // FIXME pipelining for DatabaseNetworkAccessManager
    if (QSqlDatabase::database().isValid())
        m_page->setNetworkAccessManager(new DatabaseNetworkAccessManager);
    else
        m_page->setNetworkAccessManager(new PipeliningNetworkAccessManager);


    // this makes us different to the loading test...
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    m_view->showFullScreen();
#else
    m_view->show();
#endif
    QTest::qWaitForWindowShown(m_view);
}

void tst_Cycler::cleanupTestCase()
{
    delete m_view;
}

void tst_Cycler::load_data()
{
    add_test_urls();
}

void tst_Cycler::load()
{
    QFETCH(QUrl, url);

    WEB_BENCHMARK("cycler_one_qnetworkaccessmanager_with_pipelining::load", url.toString()) {
        m_view->load(url);
        // really wait for loading..
        ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));
        TIME_NOW;
        m_view->load(QUrl("about:blank"));
        QWebSettings::clearMemoryCaches();
    }
}

DBWEBTEST_MAIN(tst_Cycler)
#include "tst_cycler_one_qnetworkaccessmanager_with_pipelining.moc"
