#ifndef CSV_H
#define CSV_H

#include <QStringList>

namespace CSV
{
    QList<QStringList> parseFromString(const QString &string, const QChar &delimiter = ',');
    QList<QStringList> parseFromFile(const QString &filename, const QString &codec = QString(), const QChar &delimiter = ',');
    bool write(const QList<QStringList> data, const QString &filename, const QString &codec = QString(), const QChar &delimiter = ',');
}

#endif // CSV_H
