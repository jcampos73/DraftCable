Attribute VB_Name = "Módulo1"
Public Sub eliminar_todos(text As String)

    Selection.Find.ClearFormatting
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = text
        .Replacement.text = ""
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

End Sub
Public Sub lee_cable(cable As String)
'open input data stream
    Const ForReading = 1, ForWriting = 2, ForAppending = 3
    Dim fs, f
    Dim s As String
    Set fs = CreateObject("Scripting.FileSystemObject")

    Set f = fs.OpenTextFile(cable, ForReading, TristateFalse)


'end of input block

    s = f.ReadLine
    Do While Len(s)
    
    'read pin de
    'length_s = Len(s)
    'space_s = InStr(s, ";")
    's2 = Left(s, space_s - 1)
    s2 = read_field(s, ";")
    
    'replace tag
    If StrComp(s2, "") Then
        Call replace_text("__pin_de", s2)
    End If
    
    'replace ref hilo
    s2 = read_field(s, ";")
    Call replace_text("__ref_hilo", s2)
    
    'replace funcion
    s2 = read_field(s, ";")
    Call replace_text("__funcion", s2)
    
    'replace pin a
    s2 = read_field(s, ";")
    Call replace_text("__pin_a", s2)
    
    s = f.ReadLine
    Loop

    'eliminates quation mark from here, MUST BE DONE NOW
    'eliminar_todos ("__pin_a") '(Chr(34))
    
    'get actual position
    Column0 = Selection.Information(wdStartOfRangeColumnNumber)
    Line0 = Selection.Information(wdFirstCharacterLineNumber)
    Call replace_text_all(Chr(34), "")
    
    'Selection.HomeKey Unit:=wdStory
    'go to previous position
    Line1 = Selection.Information(wdFirstCharacterLineNumber)
    LineOffset = Line0 - Line1 - 1
    Res = Selection.Move(wdRow, LineOffset)
    Column1 = Selection.Information(wdStartOfRangeColumnNumber)
    ColOffset = Column0 - Column1
    Res = Selection.Move(wdCell, ColOffset)
    Call replace_text_all("__pin_de", "")
    
    Selection.HomeKey Unit:=wdStory
    Call replace_text_all("__funcion", "")
    Selection.HomeKey Unit:=wdStory
    Call replace_text_all("__ref_hilo", "")
    Selection.HomeKey Unit:=wdStory
    Call replace_text_all("__pin_a", "")
    
    f.Close
End Sub
Public Sub replace_text(ByVal s_find As String, ByVal s_repl As String)

    Selection.Find.ClearFormatting
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = s_find
        .Replacement.text = s_repl
        .Forward = True
        .Wrap = wdFindContinue
        .Format = False
        .MatchCase = False
        .MatchWholeWord = False
        .MatchWildcards = False
        .MatchSoundsLike = False
        .MatchAllWordForms = False
    End With
    Selection.Find.Execute Replace:=wdReplaceOne

End Sub
Public Function replace_text1(ByVal s_find As String, ByVal s_repl As String)

    Selection.Find.ClearFormatting
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = s_find
        .Replacement.text = s_repl
        .Forward = True
        .Wrap = wdFindContinue
        .Format = False
        .MatchCase = False
        .MatchWholeWord = False
        .MatchWildcards = False
        .MatchSoundsLike = False
        .MatchAllWordForms = False
    End With
    replace_text1 = Selection.Find.Execute(Replace:=wdReplaceOne)

End Function
Public Sub replace_text_all(ByVal s_find As String, ByVal s_repl As String)

    Selection.Find.ClearFormatting
    Selection.Find.Replacement.ClearFormatting
    With Selection.Find
        .text = s_find
        .Replacement.text = s_repl
        .Forward = True
        .Wrap = wdFindStop
        .Format = False
        .MatchCase = False
        .MatchWholeWord = False
        .MatchWildcards = False
        .MatchSoundsLike = False
        .MatchAllWordForms = False
    End With
    Selection.Find.Execute
    
    Dim result As Boolean
    result = True
    Do While (result)
    With Selection
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseStart
        Else
            .Collapse Direction:=wdCollapseEnd
        End If
        .Find.Execute Replace:=wdReplaceOne
        result = Selection.Find.Forward
        If .Find.Forward = True Then
            .Collapse Direction:=wdCollapseEnd
        Else
            .Collapse Direction:=wdCollapseStart
        End If
        .Find.Execute
    End With
    result = Selection.Find.Found
    Loop

End Sub
Public Function read_field(s As String, ByVal s_sep As String)

    length_s = Len(s)

    space_s = InStr(s, ";")
    
    If space_s = 0 Then
    space_s = length_s
    s2 = Left(s, space_s)
    Else
    s2 = Left(s, space_s - 1)
    End If

    s = Right(s, length_s - space_s)
    
    read_field = s2

End Function
