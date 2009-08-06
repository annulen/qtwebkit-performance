/*
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Holger Hans Peter Freyther
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QApplication>

#include <QWebPage>
#include <QWebView>
#include <QWebSettings>

#include <QDebug>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkReply>

#include <QSqlDatabase>
#include <QSqlQuery>

/**
 * simple observer to get the metadata and content
 */
class NetworkReplyObserver : public QObject {
    Q_OBJECT
public:
    NetworkReplyObserver(QNetworkReply* reply)
        : QObject(reply)
        , m_reply(reply)
    {
        connect(m_reply, SIGNAL(readyRead()), SLOT(slotRead()));
        connect(m_reply, SIGNAL(finished()), SLOT(slotFinished()));
    }

private Q_SLOTS:
    void slotRead();
    void slotFinished();

private:
    QNetworkReply* m_reply;
    QByteArray m_internalData;
};

void NetworkReplyObserver::slotRead()
{
    QByteArray data = m_reply->peek(m_reply->bytesAvailable());
    m_internalData += data;
}

void NetworkReplyObserver::slotFinished()
{
    QByteArray httpHeader;
    QList<QByteArray> headers = m_reply->rawHeaderList();
    foreach(QByteArray header, headers)
        httpHeader += header + ": " + m_reply->rawHeader(header) + "\r\n";

    QSqlQuery query("INSERT INTO responses(operation, response, url, data, header) VALUES(:op, :response, :url, :data, :header)");
    query.bindValue(":op", m_reply->operation());
    query.bindValue(":response", m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute));
    query.bindValue(":url", m_reply->url());
    query.bindValue(":data", m_internalData);
    query.bindValue(":header", httpHeader);
    query.exec();

    // verify that the old and new data re the same...
    if (!query.exec()) {
        QSqlQuery select("SELECT data, header FROM responses WHERE url = :url");
        select.bindValue(":url", m_reply->url());
        if (!select.exec()) {
            qWarning() << "Unknown issue for storing: " << m_reply->url();
            return;
        }

        select.next();
        if (httpHeader != select.value(1))
            qWarning() << "Headers are different, not storing them for: " << m_reply->url();
        if (m_internalData != select.value(0))
            qWarning() << "Data is different, not storing it for: " << m_reply->url();
    }
}

class NetworkAccessManagerProxy : public QNetworkAccessManager {
    Q_OBJECT
public:
    virtual QNetworkReply* createRequest(Operation op, const QNetworkRequest& request,
                                         QIODevice *outgoingData)
    {
        QNetworkReply* reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
        new NetworkReplyObserver(reply);
        return reply;
    }
};


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, false);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("crawl_db.db");
    if (!db.open())
        qCritical() << "Failed to open the crawl db";

    /*
     * iniialize tables... for the poor
     */
    QSqlQuery query("CREATE TABLE IF NOT EXISTS responses(operation int NOT NULL,"
                                                          "response int, "
                                                          "url blob NOT NULL UNIQUE,"
                                                          "data blob, header blob)");

    QStringList args = app.arguments();
    QUrl url("http://www.google.com/news");
    if (args.count() > 1)
        url = args.at(1);

    QWebView* view = new QWebView;
    QWebPage* page = new QWebPage(view);
    QObject::connect(page, SIGNAL(loadFinished(bool)), &app, SLOT(quit()), Qt::QueuedConnection);

    view->setPage(page);
    view->page()->setNetworkAccessManager(new NetworkAccessManagerProxy);
    view->load(url);


    return app.exec();
}

#include "main.moc"
