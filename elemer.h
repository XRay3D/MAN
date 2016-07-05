#ifndef ELEMER_H
#define ELEMER_H

#include <QDebug>
#include <QtGui>
#include <stdint.h>

class ELEMER {
public:
    explicit ELEMER();

    QString ControlSum(const QString& S);
    int Check(QString Parcel, QStringList& Array);
};

#endif // ELEMER_H
