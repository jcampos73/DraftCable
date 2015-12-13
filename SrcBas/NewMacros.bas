Attribute VB_Name = "NewMacros"
Public strtit As String
Public straux As String

Sub Macro1()
Attribute Macro1.VB_Description = "Macro grabada el 25/09/02 por Jesus Campos"
Attribute Macro1.VB_ProcData.VB_Invoke_Func = "Project.NewMacros.Macro1"
'
' Macro1 Macro
' Macro grabada el 25/09/02 por Jesus Campos
'

strtit = "Título "
straux = strtit + str(3)



For I = 1 To 6

    Selection.HomeKey Unit:=wdStory
    Selection.Find.ClearFormatting

    If I = 1 Then
        Selection.Find.Style = ActiveDocument.Styles("Título 1")
    ElseIf I = 2 Then
        Selection.Find.Style = ActiveDocument.Styles("Título 2")
    ElseIf I = 3 Then
        Selection.Find.Style = ActiveDocument.Styles("Título 3")
    ElseIf I = 4 Then
        Selection.Find.Style = ActiveDocument.Styles("Título 4")
    ElseIf I = 5 Then
        Selection.Find.Style = ActiveDocument.Styles("Título 5")
    ElseIf I = 6 Then
        Selection.Find.Style = ActiveDocument.Styles("Título 6")
    End If
    

    
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = "[0123456789]."
        .Replacement.text = ""
        .Forward = True
        .Wrap = wdFindContinue
        .Format = True
        .MatchCase = False
        .MatchWholeWord = False
        .MatchWildcards = True
        .MatchSoundsLike = False
        .MatchAllWordForms = False
    End With
    Selection.Find.Execute
    Selection.Find.Execute Replace:=wdReplaceAll
    
Next I
    
End Sub
Public Sub generar_cableado()
Attribute generar_cableado.VB_Description = "Macro grabada el 28/11/02 por Jesus Campos"
Attribute generar_cableado.VB_ProcData.VB_Invoke_Func = "Normal.NewMacros.generar_cableado"
'
' generar_cableado Macro
' Macro grabada el 28/11/02 por Jesus Campos
'
    'replaces obra
    Dim obra As String
    obra = InputBox("Introduzca numero de obra para empezar")


    'ChangeFileOpenDirectory "C:\PruDBsystem\Doc\"
    ChangeFileOpenDirectory "C:\P_SOFTWARE\2002\DraftCable\Src"
    'Documents.Open FileName:="plantilla_cabl.doc", ConfirmConversions:=False, _
    '    ReadOnly:=False, AddToRecentFiles:=False, PasswordDocument:="", _
    '    PasswordTemplate:="", Revert:=False, WritePasswordDocument:="", _
    '    WritePasswordTemplate:="", Format:=wdOpenFormatAuto
        
    'replaces obra
    Call replace_text("__obra", obra)
    Selection.HomeKey Unit:=wdStory
        
'open input data stream
    Const ForReading = 1, ForWriting = 2, ForAppending = 3
    Dim fs, f
    Dim s As String
    Set fs = CreateObject("Scripting.FileSystemObject")

    Set f = fs.OpenTextFile("cableado.txt", ForReading, TristateFalse)

    s = f.ReadLine
    length_s = Len(s)
    'space_s = InStr(s, ";")
    's2 = Left(s, space_s - 1)


'end of input block
        
    Selection.WholeStory
    Selection.Copy
    
    Dim counter As Integer
    counter = 1
    
    Do While Len(s)
    
    Call replace_text("__h", Format(counter))
    counter = counter + 1
    
    'replace ref hilo
    s2 = read_field(s, ";")
    s2 = read_field(s, ";")
    Call replace_text("__cable_num", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__cable_tipo", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__pj_de", s2)
    Call replace_text("__de_conector", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__pj_a", s2)
    Call replace_text("__a_conector", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__de_equipo", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__a_equipo", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__de_situadoen", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__a_situadoen", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__de_conectortipo", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__a_conectortipo", s2)
    
    s2 = read_field(s, ";")
    Call replace_text("__clongi", s2)
    
    s2 = read_field(s, ";")
    'hilos
    Dim cable As String
    cable = ".\Cables\"
    cable = cable + s2
    cable = cable + ".txt"
    
    
    s2 = read_field(s, ";")
    Call replace_text("__cpar", s2)
    
    
    'replace all cable inputs
    'lee_cable (".\\Cables\\Cable516.txt")
    lee_cable (cable)
    
    s2 = read_field(s, ";")
    Selection.HomeKey Unit:=wdStory
    If IsNumeric(s2) Then
        While (replace_text1("__pin_de", s2))
            s2 = s2 + 1
        Wend
    Else
        Call replace_text("__pin_de", s2)
        Call replace_text_all("__pin_de", "")
    End If
    
    
    'Selection.EndKey Unit:=wdStory
    'Selection.HomeKey Unit:=wdStory
    'Selection.Find.ClearFormatting
    'Selection.Find.Replacement.ClearFormatting
    'With Selection.Find
    '    .text = "__cable_num"
    '    .Replacement.text = s2 '"W100"
    '    .Forward = True
    '    .Wrap = wdFindContinue
    '    .Format = False
    '    .MatchCase = False
    '    .MatchWholeWord = False
    '    .MatchWildcards = False
    '    .MatchSoundsLike = False
    '    .MatchAllWordForms = False
    'End With
    'Selection.Find.Execute
    'With Selection
    '    If .Find.Forward = True Then
    '        .Collapse Direction:=wdCollapseStart
    '    Else
    '        .Collapse Direction:=wdCollapseEnd
    '    End If
    '    .Find.Execute Replace:=wdReplaceOne
    '    If .Find.Forward = True Then
    '        .Collapse Direction:=wdCollapseEnd
    '    Else
    '        .Collapse Direction:=wdCollapseStart
    '    End If
    '    .Find.Execute
    'End With
    
'replace all cable inputs
'lee_cable (".\\Cables\\Cable516.txt")
   
    s = f.ReadLine
    If Len(s) Then
    Selection.EndKey Unit:=wdStory
    Selection.Paste
        
    'begin of just pasted table
    Selection.MoveUp Unit:=wdScreen, Count:=1
    'Selection.HomeKey Unit:=wdColumn
    End If
    
    Loop

    Selection.HomeKey Unit:=wdStory
    
    Selection.Find.ClearFormatting
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = "__t"
        .Replacement.text = Format(counter - 1)
        .Forward = True
        .Wrap = wdFindStop
        .Format = False
        .MatchCase = False
        .MatchWholeWord = False
        .MatchWildcards = False
        .MatchSoundsLike = False
        .MatchAllWordForms = False
    End With
    Selection.Find.Execute Replace:=wdReplaceAll

    'deletes not used tags, obsolete
    'eliminar_todos ("__pin_de")
    
    f.Close
End Sub
Sub pru_repag_inicio()
Attribute pru_repag_inicio.VB_Description = "Macro grabada el 29/11/02 por Jesus Campos"
Attribute pru_repag_inicio.VB_ProcData.VB_Invoke_Func = "Normal.NewMacros.pru_repag_inicio"
'
' pru_repag_inicio Macro
' Macro grabada el 29/11/02 por Jesus Campos
'
    Selection.MoveUp Unit:=wdScreen, Count:=3
    Selection.HomeKey Unit:=wdColumn
End Sub
Sub pru_find_repl()
Attribute pru_find_repl.VB_Description = "Macro grabada el 29/11/02 por Jesus Campos"
Attribute pru_find_repl.VB_ProcData.VB_Invoke_Func = "Normal.NewMacros.pru_find_repl"
'
' pru_find_repl Macro
' Macro grabada el 29/11/02 por Jesus Campos
'
    Selection.Find.ClearFormatting
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = "__pin_de"
        .Replacement.text = ""
        .Forward = True
        .Wrap = wdFindContinue
        .Format = False
        .MatchCase = False
        .MatchWholeWord = False
        .MatchWildcards = False
        .MatchSoundsLike = False
        .MatchAllWordForms = False
    End With
    Selection.Find.Execute
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
End Sub
