#ifndef MAN_H
#define MAN_H

#include <QObject>
#include <QtSerialPort>

#include "elemer.h"

#include "qt_windows.h"

class MAN : public QSerialPort, public ELEMER {
    Q_OBJECT
public:
    explicit MAN(QObject* parent = 0)
        : QSerialPort(parent)
    {
        setFlowControl(QSerialPort::NoFlowControl);
        setParity(QSerialPort::NoParity);
        setBaudRate(19200);
        connect(this, &MAN::PingSignal, this, &MAN::PingSlot);
        ::QueryPerformanceFrequency((LARGE_INTEGER*)&T3);
        T1 = T3;
    }

    double T1;
    long long T2;
    long long T3;

    bool Ping(const QString& portName)
    {
        mutex.lock();
        emit PingSignal(portName);
        if (mutex.tryLock(1000)) {
            mutex.unlock();
            return true;
        }
        mutex.unlock();
        return false;
    }

    void start()
    {
        ::QueryPerformanceCounter((LARGE_INTEGER*)&T2);
    }
    void stop()
    {
        ::QueryPerformanceCounter((LARGE_INTEGER*)&T3);
        qDebug() << ((T3 - T2) / T1);
    }

    void PingSlot(const QString& portName)
    {
        QString Parcel;
        QStringList Array;
        int counter = 5;
        start();
        setPortName(portName);
        if (open(QSerialPort::ReadWrite)) {
            qDebug() << "open";
            for (int i = 0; i < 4; ++i) {
                Parcel = QString(":%1;254;").arg(i + 1);
                Parcel = Parcel + ControlSum(Parcel) + '\r';
                write(Parcel.toLocal8Bit());
                waitForReadyRead(100);
                Parcel = readAll();
                while (!Parcel.count('\r') && counter--) {
                    waitForReadyRead(10);
                    Parcel.append(readAll());
                }
                if (!Check(Parcel, Array)) {
                    close();
                    return;
                }
                waitForReadyRead(50);
            }
            close();
            mutex.unlock();
        }
        stop();
    }

    //    Private Function WriteRead(ByVal H As Long, ByRef S As String, Optional ByVal T As Single = 1) As String
    //        Dim Data() As Byte
    //        Dim Flag As Integer
    //        If Len(S) > 1 Then Debug.Print Left(S, Len(S) - 1)
    //        If H > 0 Then
    //            Call StrArr(S, Data)
    //            T = T + timer
    //            Flag = 1
    //            Call WriteP2(H, Data, Flag)
    //            Do While Flag And T > timer                   'Îæèäàíèå îòâåòà
    //                If Not PO Then Exit Do
    //                Call Sleep(1)
    //                DoEvents
    //            Loop
    //            Call Delay(0.05)
    //            If ReadP(H, Data) > 0 Then Call ArrStr(Data, S): WriteRead = S Else S = "": WriteRead = ""
    //        End If
    //        MAN.Label1.Caption = S
    //        If Len(S) > 1 Then Debug.Print Left(S, Len(S) - 1)
    //    End Function

    //    typedef struct PAR {
    //        float InVoltage;
    //        float LoVoltage;
    //        float HiVoltage;
    //        float LoadCurrent;
    //        float IdleCurrent;
    //    } PAR;
    //    bool flUst;
    //    PAR Lo[4];
    //    PAR Hi[4];
    //    QVector<int> Channel;

    //    void Start()
    //    {
    //        const int Nsu = 20;

    //        bool flUst2;
    //        int I, J, L;
    //        QString Str;
    //        QVector<QString> Ar;

    //        double D[4] = { 0.0, 0.0, 0.0, 0.0 };
    //        const double filt = 0.2;
    //        //==============================
    //        //Call cbxTrans_Change
    //        //        MAN.U1.ForeColor = &H808080: MAN.U1.Text = "0,000"
    //        //        MAN.U2.ForeColor = &H808080: MAN.U2.Text = "0,000"
    //        //        MAN.U3.ForeColor = &H808080: MAN.U3.Text = "0,000"
    //        //        MAN.U4.ForeColor = &H808080: MAN.U4.Text = "0,000"

    //        //==============================
    //        if (MAN.CheckBox1.Value) {
    //            Setup();
    //        };
    //        while (PO) {
    //            if (flUst != flUst2) {
    //                Ustavka(flUst);
    //                flUst2 = flUst;
    //            } //==============================
    //            for (I = 0; i < Channel.size(); ++I) {
    //                Str = WriteRead2(":" & Channel(I) & ";1;0;");
    //                L = Check(Str, Ar);
    //                if (L > 0) {
    //                    //==============================
    //                    D(I) = (1 - filt) * D(I) + filt * Value(Ar(1)); // óñðåäíåíèå
    //                    VoltVal(I + 1, D(I));
    //                }
    //                //                DoEvents
    //            }
    //            //        If MAN.CheckBox1.Value Then
    //            //            MAN.MeasCurrent.Text = Format(GetIAC * 1000#, "0.000")
    //            //        End If
    //            //==============================
    //            //            DoEvents
    //            //        Sleep(1);
    //        }
    //    }

    //    void Ustavka(bool flNagr)
    //    {
    //        QString Ust;
    //        int I = 0;
    //        foreach (int var, Channel) {
    //            Ust = QByteArray().fromRawData((char*)&Lo(I).LoadCurrent, 4);
    //            if (flNagr) {
    //                Ust = ":" + Channel(I) + ";73;01" + Ust + ";";
    //            }
    //            else {
    //                Ust = ":" + I + ";73;00" + Ust + ";";
    //            }
    //            Ust = WriteRead2(Ust);
    //        }
    //    }

    //    float Value(QString Str)
    //    {
    //        Str = Str.replace(",", ".");
    //        return Str.toFloat();
    //    }

    //    QList<float> Value2(QString Str)
    //    {
    //        QList<QString> Ar;
    //        QList<float> S;

    //        Str = Str.replace(",", ".");
    //        if (Str.count('±')) {
    //            Ar = Str.split('±');
    //            S << Ar.at(0).toFloat() - Ar.at(1).toFloat();
    //            S << Ar.at(0).toFloat() + Ar.at(1).toFloat();
    //        }
    //        if (Str.count('>')) {
    //            Ar = Str.split('>');
    //            S << Ar.at(0).toFloat();
    //            S << Ar.at(1).toFloat() * 1.2;
    //        }
    //        if (Str.count('<')) {
    //            Ar = Str.split('<');
    //            S << Ar.at(1).toFloat() * 0.8;
    //            S << Ar.at(1).toFloat();
    //        }
    //        return S;
    //    }

    //float VoltVal(int N, float Str e = -999.0){
    //    Dim TextBox As Object;

    //    switch (N ) {
    //        case 1:
    //            break;
    //        default:
    //            break;
    //    }
    //    If N = 1 Then Set TextBox = MAN.U1;
    //    If N = 2 Then Set TextBox = MAN.U2;
    //    If N = 3 Then Set TextBox = MAN.U3;
    //    If N = 4 Then Set TextBox = MAN.U4;

    //        VoltVal = Value(.Text);
    //        If Str <> -999# Then;
    //            .Text = Format(Str, "0.000");
    //            if (flUst == true) {
    //                 Lo(N - 1).HiVoltage > Str And Str > Lo(N - 1).LoVoltage ? .ForeColor = RGB(0, 255, 0) : .ForeColor = RGB(255, 0, 0);
    //            }Else
    //                 Hi(N - 1).HiVoltage > Str And Str > Hi(N - 1).LoVoltage ? .ForeColor = RGB(0, 255, 0) : .ForeColor = RGB(255, 0, 0);
    //         }
    //    }
    //return Text.
    //}
    /*
Public Function CurrVal(ByVal N As Long, Optional ByVal Str As Single = -999#) As Single
    Dim TextBox As Object

    If N = 1 Then Set TextBox = MAN.I1
    If N = 2 Then Set TextBox = MAN.I2
    If N = 3 Then Set TextBox = MAN.I3
    If N = 4 Then Set TextBox = MAN.I4

    CurrVal = Value(TextBox.Text)
    If Str <> -999# Then TextBox.Text = Format(Str, "0.000")
End Function

Public Function Find(ByVal S As String, Optional ByVal Sh As String) As Range
    On Error GoTo ERR
    If Sh <> "" Then
        Set Find = Sheets(Sh).Cells.Find(What:=S, LookIn:=xlFormulas, LookAt:=xlPart, SearchOrder:=xlByRows, SearchDirection:=xlNext, MatchCase:=False, SearchFormat:=False)
    Else
        Set Find = ActiveSheet.Cells.Find(What:=S, LookIn:=xlFormulas, LookAt:=xlPart, SearchOrder:=xlByRows, SearchDirection:=xlNext, MatchCase:=False, SearchFormat:=False)
    End If
    If InStr(1, Find, S, vbTextCompare) = 0 Then GoTo ERR
    Exit Function
ERR:
    Call MsgBox("Íå íàéäåíà îïîðíàÿ ÿ÷åéêà:" & vbCr & S, vbCritical, "Îøèáêà âàïîëíåíèÿ")
    End
End Function

*/

signals:
    void PingSignal(const QString& portName);

public slots:

private:
    QMutex mutex;
};

#endif // MAN_H
