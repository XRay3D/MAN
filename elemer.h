#ifndef ELEMER_H
#define ELEMER_H

#include <QObject>

class ELEMER : public QObject
{
    Q_OBJECT
public:
    explicit ELEMER(QObject *parent = 0);
/*
Option Explicit

Sub test()
    Const S As String = ":4;254;"
    Debug.Print S; ControlSum(S)
End Sub

Public Function ControlSum(ByVal S As String) As String
    Dim N, L As Integer
    Dim KS, NS As Long
    KS = 65535
    For N = 2 To Len(S)
        NS = Asc(Mid(S, N, 1))
        KS = KS Xor NS
        For L = 1 To 8
            If (KS \ 2) * 2 <> KS Then    ' "\" ýòî äåëåíèå áåç îñòàòêà è îêòóãëåíèÿ. (10 \ 3 = 3)
                KS = (KS \ 2) Xor 40961
            Else
                KS = KS \ 2
            End If
        Next
    Next
    ControlSum = Trim(Str(KS))
End Function

Function Check(ByVal S As String, ByRef Y() As String) As Long
    Dim X1() As String
    Dim Y1() As String
    Dim I As Integer
    If InStr(1, S, ";", vbTextCompare) > 0 And InStr(1, S, "!", vbTextCompare) > 0 Then
        X1 = Split(S, "!")
        Y1 = Split(X1(1), ";")
        S = "!"
        For I = 0 To UBound(Y1) - 1
            S = S & Y1(I) & ";"
        Next
        If Val(Y1(UBound(Y1))) = Val(ControlSum(S)) Then
            Check = UBound(Y1)
            Y = Y1
        Else: Check = -1
        End If
    Else
        Check = -1
    End If
End Function




*/
signals:

public slots:
};

#endif // ELEMER_H
