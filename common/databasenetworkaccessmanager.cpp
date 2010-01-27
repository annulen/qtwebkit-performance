#include "databasenetworkaccessmanager.h"

#include "benchmark.h"
#include "common_init.h"
#include "databasenetworkreply.h"
#include "urlfilereader.h"

#include <QApplication>
#include <QFile>

bool connectToDatabase(const QString& name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    return db.open() && db.tables().size() > 0;
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

static bool initUrlFromFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QList<UrlInfo> urlList = readUrlList(&file);
    foreach (const UrlInfo &urlInfo, urlList) {
        add_url_to_test_urls(urlInfo.first);
    }
    return true;
}

static void initUrlFromDatabase()
{
    QSqlQuery knownUrlSelect;
    knownUrlSelect.prepare("SELECT url FROM mirrored_urls");
    if (knownUrlSelect.exec()) {
        while (knownUrlSelect.next())
            add_url_to_test_urls(knownUrlSelect.value(0).toUrl());
    }
}

QStringList mainImpl()
{
    const QStringList arguments = QCoreApplication::arguments();
    QStringList ouputArguments;
    ouputArguments.append(arguments[0]);

    const int argumentsCount = arguments.size();
    bool loadedUrlFromFile = false;
    bool loadUrlFromDatabase = false;
    for (int i = 1; i < argumentsCount; ++i) {
        const QString argument = arguments.at(i);
        if (argument == QLatin1String("-urlfile")) {
            ++i;
            if (!initUrlFromFile(arguments.at(i)))
                qWarning() << "Error while including urlfile " << arguments.at(i);
            else
                loadedUrlFromFile = true;
            continue;
        } else if (argument == QLatin1String("-database")) {
            ++i;
            if (i < argumentsCount) {
                if (!connectToDatabase(arguments.at(i))) {
                    qWarning() << "the database " << arguments.at(i) << " cannot be opened.";
                } else
                    loadUrlFromDatabase = true;
            } else {
                qFatal("-database is defined but no database file specified");
            }
            continue;
        }
        ouputArguments.append(arguments.at(i));
    }
    // url file has precedence over the database
    // so a url file can be used to reduce the scope of a database
    if (!loadedUrlFromFile && loadUrlFromDatabase)
        initUrlFromDatabase();

    return ouputArguments;
}
