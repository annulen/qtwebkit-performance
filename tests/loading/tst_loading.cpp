/*
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

#include "common_init.h"
#include "benchmark.h"

#include <qwebframe.h>
#include <qwebview.h>
#include <qpainter.h>

class tst_Loading : public QObject
{
    Q_OBJECT

public:
    ~tst_Loading();

public Q_SLOTS:
    void init();
    void cleanup();

private Q_SLOTS:
    void load();

private:
    QWebView* m_view;
    QWebPage* m_page;
};

tst_Loading::~tst_Loading()
{
    benchmarkOutput(*benchmark_parent);
}

void tst_Loading::init()
{
    QWebSettings::globalSettings()->setMaximumPagesInCache(0);
    QWebSettings::globalSettings()->setObjectCacheCapacities(0, 0, 0);
    m_view = new QWebView;
    m_page = m_view->page();

    QSize viewportSize(1024, 768);
    m_view->setFixedSize(viewportSize);
    m_page->setViewportSize(viewportSize);
}

void tst_Loading::cleanup()
{
    delete m_view;
}

void tst_Loading::load()
{
    const QList<QUrl> urls = test_urls();

    // warmup
    foreach(const QUrl& url, urls) {
        m_view->load(url);
        // really wait for loading..
        ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));
    }
    m_view->load(QUrl("about:blank"));
    QWebSettings::clearMemoryCaches();
    ::waitForSignal(m_view, SIGNAL(loadFinished(bool)), 1000);

    WEB_BENCHMARK("all_loads") {
        foreach(const QUrl& url, urls) {
            m_view->load(url);
            // really wait for loading..
            ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));
        }

        TIME_NOW
        m_view->load(QUrl("about:blank"));
        ::waitForSignal(m_view, SIGNAL(loadFinished(bool)), 1000);
        QWebSettings::clearMemoryCaches();
    }
}

QTEST_MAIN(tst_Loading)
#include "tst_loading.moc"
