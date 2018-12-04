Public Class Form2
    Private Sub Form2_FormClosed(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosedEventArgs) Handles Me.FormClosed
        Form1.ProgressBar1.Value = 0
    End Sub
End Class