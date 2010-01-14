#ifndef DATABASENETWORKACCESSMANAGER_H
#define DATABASENETWORKACCESSMANAGER_H

#include "databasenetworkreply.h"

#include <QNetworkAccessManager>
#include <QSqlDatabase>

class DatabaseNetworkAccessManager : public QNetworkAccessManager
{
protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0) {
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
};

bool connectToDatabase(const QString& name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    return db.open() && db.tables().size() > 0;
}

#define DBWEBTEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    QApplication app(argc, argv); \
    const QStringList arguments = QCoreApplication::arguments(); \
    QStringList ouputArguments; \
    ouputArguments.append(arguments[0]); \
 \
    const int argumentsCount = arguments.size(); \
    for (int i = 1; i < argumentsCount; ++i) { \
        if (arguments.at(i) == QLatin1String("-database")) { \
            ++i; \
            if (i < argumentsCount) { \
                if (!connectToDatabase(arguments.at(i))) {\
                    qWarning() << "the database " << arguments.at(i) << " cannot be opened."; \
                    return -1; \
                } \
            } else { \
                qFatal("-database is defined but no database file specified"); \
            } \
            continue; \
        } \
        ouputArguments.append(arguments.at(i)); \
    } \
    TestObject tc; \
    return QTest::qExec(&tc, ouputArguments); \
}

#endif // DATABASENETWORKACCESSMANAGER_H
