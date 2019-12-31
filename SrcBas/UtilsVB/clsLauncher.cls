VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsLauncher"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Function Launcher(PathWord As String, PathOutput As String)

Dim oWordApp As Word.Application

'Start a new instance of Microsoft Word
Set oWordApp = New Word.Application



With oWordApp

    .ScreenUpdating = Flase

   'Create a new document
   .Documents.Open (PathWord) '("F:\Documents and Settings\Administrador\Mis documentos\temp\draft\DraftCable\SrcBas\LaunchWord\plantilla_cabl1.doc")

   'Add text to the document
   '.Selection.TypeText Text:="one"
   '.Selection.TypeParagraph
   '.Selection.TypeText Text:="two"
   '.Selection.TypeParagraph
   '.Selection.TypeText Text:="three"
   
   'Save the document
   '"F:\Documents and Settings\Administrador\Mis documentos\temp\draft\DraftCable\SrcBas\LaunchWord\cableado.doc"
   .ActiveDocument.SaveAs FileName:=PathOutput, _
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
'End
End Function


