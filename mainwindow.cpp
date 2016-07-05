#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
 Dim I As Byte
    Dim S As String
    Dim PortList() As String

    ThisWorkbook.Save
    If Mcp.EnumP(PortList) > 0 Then
        For I = LBound(PortList) To UBound(PortList)
            Call Me.cbxPort.AddItem(PortList(I), I)
            Call Me.cbxPortA.AddItem(PortList(I), I)
        Next
    End If

    Me.I1.Text = "000.000"
    Me.I2.Text = "000.000"
    Me.I3.Text = "000.000"
    Me.I4.Text = "000.000"

    Me.btnSetLoHiCurrent.Enabled = False
    Me.btnWrite.Enabled = False
    Me.MeasCurrent.Enabled = False

    For I = 0 To 50
        S = Sheets("BAZA").Cells(I + 4, 2)
        If S <> "" Then
            Call Me.cbxTrans.AddItem(Sheets("BAZA").Cells(I + 4, 2), I)
        Else
            Exit For
        End If
    Next

    If Me.cbxPort.ListCount > Sheets("INI").Cells(1, 1) Then Me.cbxPort.ListIndex = Sheets("INI").Cells(1, 1) Else Me.cbxPort.ListIndex = 0
    If Me.cbxPortA.ListCount > Sheets("INI").Cells(1, 2) Then Me.cbxPortA.ListIndex = Sheets("INI").Cells(1, 2) Else Me.cbxPortA.ListIndex = 0
    If Me.cbxTrans.ListCount > Sheets("INI").Cells(1, 3) Then Me.cbxTrans.ListIndex = Sheets("INI").Cells(1, 3) Else Me.cbxTrans.ListIndex = 0
    Me.CheckBox1.Value = Sheets("INI").Cells(1, 4)
*/
}

MainWindow::~MainWindow()
{
    /*
    If Me.cbxPort.Enabled = False Then Call Mcp.ClosePort
    Sheets("INI").Cells(1, 1) = Me.cbxPort.ListIndex
    Sheets("INI").Cells(1, 2) = Me.cbxPortA.ListIndex
    Sheets("INI").Cells(1, 3) = Me.cbxTrans.ListIndex
    Sheets("INI").Cells(1, 4) = Me.CheckBox1.Value
*/
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    /*

 Dim I As Integer
    Dim S As String

    If Me.cbxPort.Enabled = True Then
        If Mcp.OpenPort = True Then
            For I = 1 To 4
                S = ":" & I & ";254;"
                S = WriteRead2(S)
                If InStr(1, S, "Ver 5.19", vbTextCompare) = 0 Then
                    Call Mcp.ClosePort
                    Call MsgBox("Ìàí ¹" & I & "íå îòâå÷àåò", vbCritical)
                    Exit Sub
                End If
            Next
            Me.btnSetLoHiCurrent.Enabled = True
            Me.btnWrite.Enabled = True
            Me.MeasCurrent.Enabled = True
            Me.cbxTrans.Enabled = False
            Me.cbxPort.Enabled = False
            Me.cbxPortA.Enabled = False
            Me.CheckBox1.Enabled = False
            Call cbxTrans_Change
            Me.btnPort.Caption = "Çàêðûòü ïîðò"
            Call MAIN.Start
        End If
    Else
        Me.btnSetLoHiCurrent.Enabled = False
        Me.btnWrite.Enabled = False
        Me.MeasCurrent.Enabled = False
        Me.cbxTrans.Enabled = True

        Call Mcp.ClosePort
        Me.cbxPort.Enabled = True
        Call CheckBox1_Click
        Me.CheckBox1.Enabled = True
        Me.btnPort.Caption = "Óñòàíîâèòü ñâÿçü"
    End If
*/
}

void MainWindow::on_pushButton_clicked()
{
    /*
    If MAIN.flUst Then
        Me.btnSetLoHiCurrent.Caption = "Âêëþ÷èòü íàãðóçêó"
        Me.btnSetLoHiCurrent.BackColor = RGB(128, 255, 128)
        MAIN.flUst = False
        Me.Frame1.Caption = "Íàïðÿæåíèå " & "(" & Hi(0).InVoltage & ")"
        Me.LabelU1.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 5)
        Me.LabelU2.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 6)
        Me.LabelU3.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 7)
        Me.LabelU4.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 8)
    Else
        Me.btnSetLoHiCurrent.Caption = "Âûêëþ÷èòü íàãðóçêó"
        Me.btnSetLoHiCurrent.BackColor = RGB(255, 128, 128)
        MAIN.flUst = True
        Me.Frame1.Caption = "Íàïðÿæåíèå " & "(" & Lo(0).InVoltage & ")"
        Me.LabelU1.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 14)
        Me.LabelU2.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 15)
        Me.LabelU3.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 16)
        Me.LabelU4.Caption = Find(Me.cbxTrans.Text, "BAZA").offset(0, 17)
    End If
*/
}

void MainWindow::on_pushButton_3_clicked()
{
    /*
    Dim I As Long
    Dim R As Long
    Dim C As Long
    For R = 2 To 1000
        If MAIN.flUst Then
            If Sheets("11").Cells(R, 7) = "" Then
                For C = 1 To UBound(MAIN.Channel) + 1
                    Sheets("11").Cells(R, C + 6) = VoltVal(C)
                Next
                GoTo A
            End If
        Else
            If Sheets("11").Cells(R, 2) = "" Then

                For C = 1 To UBound(MAIN.Channel) + 1
                    Sheets("11").Cells(R, C + 1) = VoltVal(C)
                Next
                GoTo A
            End If
        End If
    Next
A:
    Sheets("11").Cells(R, 11) = Me.cbxTrans.Text
    Sheets("11").Cells(R, 12) = Time
    Sheets("11").Cells(R, 13) = Date
    Sheets("11").Cells(R, 13) = "îò" & Format(Date, "d mmmm yyyy") & "ã."

    Dim S As Single
    If MAN.CheckBox1.Value Then
        S = GetIAC * 1000#
        MAN.MeasCurrent.Text = Format(S, "0.000")
        If Not MAIN.flUst Then Sheets("11").Cells(R, 1) = S
    End If
*/
}

void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    /*
    Dim I As Integer
    Dim Ar() As String

    For I = LBound(Lo) To UBound(Lo)
        MAIN.Lo(I).InVoltage = Find(Me.cbxTrans.Text, "BAZA").offset(0, 9)
        MAIN.Lo(I).LoVoltage = Value2(Find(Me.cbxTrans.Text, "BAZA").offset(0, 14 + I))(0)
        MAIN.Lo(I).HiVoltage = Value2(Find(Me.cbxTrans.Text, "BAZA").offset(0, 14 + I))(1)
        MAIN.Lo(I).LoadCurrent = Value(Find(Me.cbxTrans.Text, "BAZA").offset(0, 10 + I))
        MAIN.Lo(I).IdleCurrent = 0#
        Call CurrVal(I + 1, MAIN.Lo(I).LoadCurrent)
    Next
    For I = LBound(Hi) To UBound(Hi)
        MAIN.Hi(I).InVoltage = Find(Me.cbxTrans.Text, "BAZA").offset(0, 4)
        MAIN.Hi(I).LoVoltage = Value2(Find(Me.cbxTrans.Text, "BAZA").offset(0, 5 + I))(0)
        MAIN.Hi(I).HiVoltage = Value2(Find(Me.cbxTrans.Text, "BAZA").offset(0, 5 + I))(1)
        MAIN.Hi(I).LoadCurrent = 0#
        MAIN.Hi(I).IdleCurrent = Value(Find(Me.cbxTrans.Text, "BAZA").offset(0, 3))
    Next

    Ar = Split(Find(Me.cbxTrans.Text, "BAZA").offset(0, 2), ",", -1, vbTextCompare)
    ReDim MAIN.Channel(UBound(Ar))

    For I = LBound(Ar) To UBound(Ar)
        MAIN.Channel(I) = Val(Ar(I))
    Next

    MAIN.flUst = True
    Call btnSetLoHiCurrent_Click
*/
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    /*
    If Me.CheckBox1.Value Then Me.cbxPortA.Enabled = True Else Me.cbxPortA.Enabled = False

*/
}
