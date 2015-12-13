VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()

Dim oWordApp As Word.Application

'Start a new instance of Microsoft Word
Set oWordApp = New Word.Application



With oWordApp

    .ScreenUpdating = Flase

   'Create a new document
   .Documents.Open ("F:\Documents and Settings\Administrador\Mis documentos\temp\draft\DraftCable\SrcBas\LaunchWord\plantilla_cabl5.doc")

   'Add text to the document
   '.Selection.TypeText Text:="one"
   '.Selection.TypeParagraph
   '.Selection.TypeText Text:="two"
   '.Selection.TypeParagraph
   '.Selection.TypeText Text:="three"
   
   'Save the document
   .ActiveDocument.SaveAs FileName:="F:\Documents and Settings\Administrador\Mis documentos\temp\draft\DraftCable\SrcBas\LaunchWord\cableado6.doc", _
       FileFormat:=wdFormatDocument, LockComments:=False, _
       Password:="", AddToRecentFiles:=True, WritePassword _
       :="", ReadOnlyRecommended:=False, EmbedTrueTypeFonts:=False, _
       SaveNativePictureFormat:=False, SaveFormsData:=False, _
       SaveAsAOCELetter:=False

    .ScreenUpdating = True
    
    .Visible = True
    
    .ScreenRefresh
    
    
End With

'Quit Word
'oWordApp.Quit
End
End Sub
