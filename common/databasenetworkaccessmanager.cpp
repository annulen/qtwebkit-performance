#include "databasenetworkaccessmanager.h"

#include "benchmark.h"
#include "common_init.h"
#include "databasenetworkreply.h"

#include <QApplication>

bool connectToDatabase(const QString& name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    const bool isValid = db.isValid() && db.open() && db.tables().size() > 0;
    if (!isValid)
        QSqlDatabase::removeDatabase(db.connectionName());
    return isValid;
}


QNetworkReply* DatabaseNetworkAccessManager::createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData)
{
    Q_UNUSED(outgoingData);
    DatabaseNetworkReply* reply = 0;
    if (op != GetOperation)
        reply = new DatabaseNetworkReply(QUrl());
    else
        reply = new DatabaseNetworkReply(req.url());

    reply->setOperation(op);
    reply->setRequest(req);
    return reply;
}
