#ifndef AGILENT_H
#define AGILENT_H

#include <QObject>

class AGILENT : public QObject
{
    Q_OBJECT
public:
    explicit AGILENT(QObject *parent = 0);
/*
Private TXT As String

Public Sub Setup()
    Call WriteReadA("*RST")
    Call WriteReadA("*CLS")
    Call WriteReadA("Syst:Rem")
    Call WriteReadA("MEASure:Current:AC?", 20)
End Sub

Public Function GetIAC() As Single
    GetIAC = Val(WriteReadA("MEASure:Current:AC?", 20))
End Function

Public Function GetV() As Single
    TXT = "MEASure:Voltage:DC?" & Chr(10)
    GetV = Val(COM_PORT.CP_WRITE(TXT, Len(TXT)))
End Function
Public Function GetI() As Single
    TXT = "MEASure:CURRent:DC?" & Chr(10)
    GetI = Val(COM_PORT.CP_WRITE(TXT, Len(TXT)))
End Function
Public Function GetR2() As Single
    TXT = "MEASure:RESistance?" & Chr(10)
    GetR2 = Val(COM_PORT.CP_WRITE(TXT, Len(TXT)))
End Function
Public Function GetR4() As Single
    TXT = "MEASure:FRESistance?" & Chr(10)
    GetR4 = Val(COM_PORT.CP_WRITE(TXT, Len(TXT)))
End Function


*/
signals:

public slots:
};

#endif // AGILENT_H
