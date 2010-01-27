#ifndef DATABASENETWORKACCESSMANAGER_H
#define DATABASENETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QStringList>
#include <QSqlDatabase>

class QIODevice;
class QString;
class QNetworkReply;
class QNetworkRequest;

bool connectToDatabase(const QString& name);

class DatabaseNetworkAccessManager : public QNetworkAccessManager
{
protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData = 0);
};

QStringList mainImpl();

#define DBWEBTEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    QApplication app(argc, argv); \
    TestObject tc; \
    return QTest::qExec(&tc, mainImpl()); \
}

#endif // DATABASENETWORKACCESSMANAGER_H
