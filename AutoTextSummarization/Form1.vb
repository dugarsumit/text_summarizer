Imports System.Runtime.InteropServices

Public Class Form1

    <DllImport("user32.dll")> _
    Public Shared Function GetAsyncKeyState(ByVal vKey As Int32) As Short

    End Function

    Dim sourcepath As String = ""
    Dim destinationpath As String = ""
    Dim sourcefile As String = ""
    Dim default_destination As String = ""
    Dim outputfile_empty As Integer = 1

    Dim filewriter As System.IO.StreamWriter 'object to write text into a file
    Dim filereader As String 'object to read text from a file
    Dim logreader As String = ""
    Dim logwriter As System.IO.StreamWriter
    Dim summary_amt As Integer = 5
    Dim article_type As String = "general"
    'Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

    Private Sub Source_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Source.Click
        sourcepath = ""
        Sourcetext.Text = ""
        default_destination = ""
        If OpenFileDialog1.ShowDialog() = DialogResult.OK Then
            sourcepath = OpenFileDialog1.FileName
            Sourcetext.Text = sourcepath
            Dim strarray() As String
            strarray = sourcepath.Split("\")
            Dim upper_bound As Integer = UBound(strarray)
            For f = 0 To upper_bound - 1
                default_destination = default_destination + strarray(f) + "\"
            Next
            sourcefile = strarray(upper_bound)
            strarray = sourcefile.Split(".")
            sourcefile = strarray(0)
        End If
    End Sub

    Private Sub Destination_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Destination.Click
        destinationpath = ""
        Destinationtext.Text = ""
        If FolderBrowserDialog1.ShowDialog() = DialogResult.OK Then
            destinationpath = FolderBrowserDialog1.SelectedPath
            Destinationtext.Text = destinationpath
        End If
    End Sub

    Private Sub Generatesummary_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Generatesummary.Click
        If check_noempty() Then
            'if maintext is not empty then the contents of maintext will be written in a file called articel.txt stored in summary folder
            'if destination is not specified then article_summary will be created at D:\summary\
            If maintext.Text <> "" Then
                sourcepath = ".\summary\article.txt"
                destinationpath = ".\summary\article_summary.txt"
                Call writearticlefile()
            Else   'when maintext is empty
                If Destinationtext.Text = "" Then
                    destinationpath = default_destination + sourcefile + "_summary.txt"
                Else
                    destinationpath = destinationpath + sourcefile + "_summary.txt"
                End If
                'SleepFor(2) ' Pause for 1½ seconds.
            End If
            Call runmainprog()
            SleepFor(1)
            Timer1.Enabled = True
        End If
    End Sub

    Private Sub runmainprog()
        'run the .exe by passing the source and destination files as arguments
        Dim pid As Integer
        'MsgBox(".\summer_f8.exe" + " " + sourcepath + " " + destinationpath)
        pid = Shell(".\summer_f12.exe" & " " & Chr(34) & sourcepath & Chr(34) & " " & Chr(34) & destinationpath & Chr(34) & " " & Chr(34) & summary_amt & Chr(34) & " " & Chr(34) & article_type & Chr(34), 1)
        'pid = Shell(".\summer_f8.exe" + " " + sourcepath + " " + destinationpath, 1)
    End Sub

    Private Function check_noempty() As Boolean
        'provides the initial check for empty text and source 
        If maintext.Text = "" And Sourcetext.Text = "" Then
            MsgBox("No Source File Found")
            Return False
        End If
        Return True
    End Function

    Private Sub readsummaryfile()
        'reads text from the summary file into the summary window
        filereader = My.Computer.FileSystem.ReadAllText(destinationpath) 'to check the working make it sourcepath
        If filereader <> "" Then
            outputfile_empty = 0
            Form2.summarytext.Text = filereader
        End If
    End Sub

    Private Sub writearticlefile()
        'writes the text from the maintext into an aritcle.txt file and saves it at sourcepath or at summary folder
        filewriter = My.Computer.FileSystem.OpenTextFileWriter(sourcepath, 0)
        'filewriter.WriteLine(maintext.Text)
        filewriter.Write(maintext.Text)
        filewriter.Close()
    End Sub

    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        ProgressBar1.Increment(1)
        Timer1.Interval = 10
        logreader = My.Computer.FileSystem.ReadAllText(".\log.txt")
        If logreader = "1" Then
            ProgressBar1.Value = 1000
            Call readsummaryfile()
            If outputfile_empty Then
                ProgressBar1.Value = 0
            Else
                Form2.Show()
            End If
            Try
                logwriter = My.Computer.FileSystem.OpenTextFileWriter(".\log.txt", 0)
                logwriter.Write(0)
                logwriter.Close()
            Catch
            End Try
            Timer1.Enabled = False
        End If
        If logreader = "2" Then
            logwriter = My.Computer.FileSystem.OpenTextFileWriter(".\log.txt", 0)
            logwriter.Write(0)
            logwriter.Close()
            maintext.Text = "Source file is empty!"
            Timer1.Enabled = False
        End If
    End Sub

    Private Sub maintext_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles maintext.DoubleClick
        maintext.Text = ""
    End Sub

    Public Sub SleepFor(ByVal Seconds As Double)
        ' "Sleep" for the specified number of seconds.
        Dim EndTime As Date
        EndTime = DateAdd("s", Seconds, Now)
        Do
        Loop Until Now >= EndTime
    End Sub

    Private Sub maintext_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles maintext.TextChanged
        If maintext.Text <> "" Then
            Source.Enabled = False
            Destination.Enabled = False
            Sourcetext.Enabled = False
            Destinationtext.Enabled = False
        Else
            Source.Enabled = True
            Destination.Enabled = True
            Sourcetext.Enabled = True
            Destinationtext.Enabled = True
        End If
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Timer2.Enabled = True
        Timer2.Interval = 1
        Dim pid As Integer = Shell("notepad readme", 1)
    End Sub

    Private Sub Timer2_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer2.Tick
        Dim f10 As Integer = 0
        f10 = GetAsyncKeyState(Keys.PageUp)
        If f10 = -32768 Then
            Dim copiedtext As String
            Timer1.Enabled = False
            'SendKeys.Send("^C")
            copiedtext = Clipboard.GetText()
            maintext.Text = copiedtext
            'MsgBox("hello")


            If check_noempty() Then
                'if maintext is not empty then the contents of maintext will be written in a file called articel.txt stored in summary folder
                'if destination is not specified then article_summary will be created at D:\summary\
                If maintext.Text <> "" Then
                    sourcepath = ".\summary\article.txt"
                    destinationpath = ".\summary\article_summary.txt"
                    Call writearticlefile()
                Else   'when maintext is empty
                    If Destinationtext.Text = "" Then
                        destinationpath = default_destination + sourcefile + "_summary.txt"
                    Else
                        destinationpath = destinationpath + sourcefile + "_summary.txt"
                    End If
                    'SleepFor(2) ' Pause for 1½ seconds.
                End If
                Call runmainprog()
                SleepFor(1)
                Timer1.Enabled = True
            End If



            Timer2.Enabled = True
        End If
    End Sub

    Private Sub Timer3_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer3.Tick
        Timer2.Enabled = True
        Timer3.Enabled = False
    End Sub

    Private Sub TrackBar1_Scroll(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TrackBar1.Scroll
        GroupBox3.Text = "Shrink to " & TrackBar1.Value * 10 & "%" & " Summary"
        summary_amt = TrackBar1.Value
    End Sub

    Private Sub ComboBox1_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ComboBox1.SelectedIndexChanged
        If ComboBox1.SelectedItem = "general" Then
            article_type = "general"
        ElseIf ComboBox1.SelectedItem = "sports" Then
            article_type = "sports"
        ElseIf ComboBox1.SelectedItem = "business" Then
            article_type = "business"
        ElseIf ComboBox1.SelectedItem = "science" Then
            article_type = "science"
        ElseIf ComboBox1.SelectedItem = "political" Then
            article_type = "political"
        End If
    End Sub
End Class
