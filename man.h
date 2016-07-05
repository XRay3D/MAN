#ifndef MAN_H
#define MAN_H

#include <QObject>

class MAN : public QObject
{
    Q_OBJECT
public:
    explicit MAN(QObject *parent = 0);


    /*
Option Explicit

Public Type PAR
    InVoltage As Single
    LoVoltage As Single
    HiVoltage As Single
    LoadCurrent As Single
    IdleCurrent As Single
End Type

Private Declare Function Beep Lib "kernel32.dll" (ByVal dwFreq As Long, ByVal dwDuration As Long) As Long

Public flUst As Boolean

Public Lo(3) As PAR
Public Hi(3) As PAR
Public Channel() As Long

Public Sub Start()
    Const Nsu As Long = 20

    Dim flUst2 As Boolean
    Dim I As Integer, J As Integer
    Dim L As Long
    Dim Str As String
    Dim Ar() As String

    Dim D(3) As Double: D(0) = 0#: D(1) = 0#: D(2) = 0#: D(3) = 0#
    Const filt As Double = 0.2
    '==============================
    'Call cbxTrans_Change
    MAN.U1.ForeColor = &H808080: MAN.U1.Text = "0,000"
    MAN.U2.ForeColor = &H808080: MAN.U2.Text = "0,000"
    MAN.U3.ForeColor = &H808080: MAN.U3.Text = "0,000"
    MAN.U4.ForeColor = &H808080: MAN.U4.Text = "0,000"

    '==============================
    If MAN.CheckBox1.Value Then Call Setup
    Do While PO
        If flUst <> flUst2 Then
            Call Ustavka(flUst)
            flUst2 = flUst
        End If
        '==============================
        For I = LBound(Channel) To UBound(Channel)
            Str = WriteRead2(":" & Channel(I) & ";1;0;")
            L = Check(Str, Ar)
            If L > 0 Then
                '==============================
                D(I) = (1 - filt) * D(I) + filt * Value(Ar(1))    ' óñðåäíåíèå
                Call VoltVal(I + 1, D(I))
            End If
            DoEvents
        Next
        '        If MAN.CheckBox1.Value Then
        '            MAN.MeasCurrent.Text = Format(GetIAC * 1000#, "0.000")
        '        End If
        '==============================
        DoEvents
        Call Sleep(1)
    Loop
End Sub

Sub Ustavka(ByVal flNagr As Boolean)
    Dim Ust As String
    Dim I As Integer
    For I = LBound(Channel) To UBound(Channel)
        Ust = IEEE754ToHex(Lo(I).LoadCurrent)
        If flNagr Then Ust = ":" & Channel(I) & ";73;01" & Ust & ";" Else Ust = ":" & I & ";73;00" & Ust & ";"
        Ust = WriteRead2(Ust)
        Call Delay(0.1)
    Next
End Sub

Public Function Value(ByVal Str As String) As Single
    Str = Replace(Str, ",", ".", 1, -1, vbTextCompare)
    Value = Val(Str)
End Function

Public Function Value2(ByVal Str As String) As Single()
    Dim Ar() As String
    Dim S(1) As Single
    Str = Replace(Str, ",", ".", 1, -1, vbTextCompare)
    If InStr(1, Str, "±", vbTextCompare) Then
        Ar = Split(Str, "±", -1, vbTextCompare)
        S(0) = Val(Ar(0)) - Val(Ar(1))
        S(1) = Val(Ar(0)) + Val(Ar(1))
    End If
    If InStr(1, Str, ">", vbTextCompare) Then
        Ar = Split(Str, ">", -1, vbTextCompare)
        S(0) = Val(Ar(1))
        S(1) = Val(Ar(1)) * 1.2
    End If
    If InStr(1, Str, "<", vbTextCompare) Then
        Ar = Split(Str, "<", -1, vbTextCompare)
        S(0) = Val(Ar(1)) * 0.8
        S(1) = Val(Ar(1))
    End If
    Value2 = S
End Function

Public Function VoltVal(ByVal N As Long, Optional ByVal Str As Single = -999#) As Single
    Dim TextBox As Object

    If N = 1 Then Set TextBox = MAN.U1
    If N = 2 Then Set TextBox = MAN.U2
    If N = 3 Then Set TextBox = MAN.U3
    If N = 4 Then Set TextBox = MAN.U4

    With TextBox
        VoltVal = Value(.Text)
        If Str <> -999# Then
            .Text = Format(Str, "0.000")
            If flUst = True Then
                If Lo(N - 1).HiVoltage > Str And Str > Lo(N - 1).LoVoltage Then .ForeColor = RGB(0, 255, 0) Else .ForeColor = RGB(255, 0, 0)
            Else
                If Hi(N - 1).HiVoltage > Str And Str > Hi(N - 1).LoVoltage Then .ForeColor = RGB(0, 255, 0) Else .ForeColor = RGB(255, 0, 0)
            End If
        End If
    End With
End Function

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

public slots:
};

#endif // MAN_H
