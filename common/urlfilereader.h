#ifndef URLFILEREADER_H
#define URLFILEREADER_H

#include <qiodevice.h>
#include <qlist.h>
#include <qpair.h>
#include <qurl.h>

typedef QPair<QUrl, QString> UrlInfo;

QList<UrlInfo> readUrlList(QIODevice* device)
{
    QList<UrlInfo> result;
    if (!device->isOpen())
        if (!device->open(QIODevice::ReadOnly))
            return result;

    while (!device->atEnd()) {
        QByteArray line = device->readLine();
        line = line.trimmed();

        // skip comment and empty lines
        if (line.startsWith('#'))
            continue;
        if (line.isEmpty())
            continue;

        // load the urls
        const int firstSeparator = qMin(line.indexOf('\t'), line.indexOf(' '));
        if (firstSeparator > 0) {
            QByteArray urlPart = line.left(firstSeparator);
            QByteArray descriptionPart = line.mid(firstSeparator).trimmed();
            result.append(UrlInfo(QUrl(QString::fromUtf8(urlPart)),
                                  QString::fromUtf8(descriptionPart)));
        } else {
            result.append(UrlInfo(QUrl(QString::fromUtf8(line)),
                                  QString()));
        }
    }
    return result;
}

#endif // URLFILEREADER_H
