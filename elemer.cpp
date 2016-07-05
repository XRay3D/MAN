#include "elemer.h"

ELEMER::ELEMER()
{
}

QString ELEMER::ControlSum(const QString& S)
{
    uint16_t KS = 0xFFFF;
    for (int n = 1; n < S.length(); ++n) {
        KS ^= S.at(n).toLatin1();
        for (int i = 0; i < 8; ++i) {
            if (KS & 0x01)
                KS = (KS >> 1) ^ 40961;
            else
                KS >>= 1;
        }
    }
    return QString().setNum(KS);
}

int ELEMER::Check(QString Parcel, QStringList& Array)
{
    while (Parcel.size() && Parcel[0] != '!') {
        Parcel.remove(0, 1);
    }
    if (Parcel[0] == '!' && Parcel.count(';')) {
        Array = Parcel.split(';');
        Parcel.clear();
        for (int i = 0; i < Array.size() - 1; ++i) {
            Parcel += Array.at(i) + ";";
        }
        if (Array.last().count(ControlSum(Parcel))) {
            return Array.size();
        }
    }
    return 0;
}
