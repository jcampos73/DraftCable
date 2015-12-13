Attribute VB_Name = "Module1"
Sub Main()
Dim strCmd As String
strCmd = StrConv(Command, 1)
'MsgBox (strCmd)

Dim xmlDoc As New MSXML2.DOMDocument
Dim root As IXMLDOMElement
Dim currNode, currNodeTemp As IXMLDOMNode
Dim currNode0 As IXMLDOMNode
Dim MyNewNode As IXMLDOMNode

Dim MyNode As IXMLDOMNode
Dim namedNodeMap As IXMLDOMNamedNodeMap


xmlDoc.async = False
'xmlDoc.resolveExternals = False

'Used to try to add line feeds after each node when adding nodes to document
'xmlDoc.preserveWhiteSpace = True

xmlDoc.Load (g_sBasePath & "wiring.xml")
Set root = xmlDoc.documentElement


'Open input data stream
'***************************************************************************
Const ForReading = 1, ForWriting = 2, ForAppending = 3
Dim fso, f
Dim s As String
Dim ssub1 As String
Set fso = CreateObject("Scripting.FileSystemObject")
If StrComp(strCmd, "-BRIDGES") <> 0 Then
    Set f = fso.OpenTextFile(g_sBasePath & "cableado.txt", ForReading, TristateFalse)
Else
    Set f = fso.OpenTextFile(g_sBasePath & "puentes.txt", ForReading, TristateFalse)
End If
s = f.ReadLine
length_s = Len(s)
'***************************************************************************

    
Dim counter As Integer
counter = 1
Set currNode = root.childNodes.Item(0)
Set currNode0 = currNode.cloneNode(True)

Do While Len(s) '& counter < 4)
    

    
    'Replace tag
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
    End If
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("cable_num")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("cable_tipo")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    s2 = read_field(s, ";")
    str_pj_de = s2
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("pj_de")
        currNodeTemp.nodeTypedValue = s2
        If StrComp(strCmd, "-BRIDGES") <> 0 Then
            Set currNodeTemp = currNode.selectSingleNode("de_conector")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    s2 = read_field(s, ";")
    str_pj_a = s2
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("pj_a")
        currNodeTemp.nodeTypedValue = s2
        If StrComp(strCmd, "-BRIDGES") <> 0 Then
            Set currNodeTemp = currNode.selectSingleNode("a_conector")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("de_equipo")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("a_equipo")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("de_situadoen")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("a_situadoen")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("de_conectortipo")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("a_conectortipo")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = currNode.selectSingleNode("clongi")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    '*********************************************************************************
    s2 = read_field(s, ";")
    'hilos
    Dim cable As String
    cable = ".\Cables\"
    cable = cable + s2
    cable = cable + ".txt"
    
    Set fsub1 = fso.OpenTextFile(cable, ForReading, TristateFalse)
    ssub1 = fsub1.ReadLine
    
    Set currNodeSub1 = currNode.selectSingleNode("wire") 'xmlDoc.documentElement.childNodes.Item(0).selectSingleNode("wire")

    Dim nCounter As Integer
    nCounter = 0
    Do While Len(ssub1)
        Set currNodeTemp = currNodeSub1.selectSingleNode("pj_de")
        If nCounter > 0 Then
            currNodeTemp.nodeTypedValue = Chr(34)
        Else
            currNodeTemp.nodeTypedValue = str_pj_de
        End If
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("pin_de")
        currNodeTemp.nodeTypedValue = s2sub1
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("ref_hilo")
        currNodeTemp.nodeTypedValue = s2sub1
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("funcion")
        currNodeTemp.nodeTypedValue = s2sub1
        Set currNodeTemp = currNodeSub1.selectSingleNode("pj_a")
        If nCounter > 0 Then
            currNodeTemp.nodeTypedValue = Chr(34)
        Else
            currNodeTemp.nodeTypedValue = str_pj_a
        End If
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("pin_a")
        currNodeTemp.nodeTypedValue = s2sub1
        
        ssub1 = fsub1.ReadLine
        
        If Len(ssub1) > 0 Then
            Set MyNewNode = currNodeSub1.cloneNode(True)
            currNode.appendChild MyNewNode
            Set currNodeSub1 = MyNewNode
        End If
        
        nCounter = nCounter + 1
    Loop
    fsub1.Close
    '*********************************************************************************
    
    If StrComp(strCmd, "-BRIDGES") <> 0 Then
        s2 = read_field(s, ";")
        If StrComp(s2, "") Then
            Set currNodeTemp = xmlDoc.documentElement.childNodes.Item(0).selectSingleNode("cpar")
            currNodeTemp.nodeTypedValue = s2
        End If
    End If
    
    'Sample code to create add an attribute to child node 0
    'Set MyNode = xmlDoc.createNode(NODE_ATTRIBUTE, "Sci-Fi", "")
    'Set namedNodeMap = xmlDoc.documentElement.childNodes.Item(0).Attributes
    'namedNodeMap.setNamedItem MyNode
    
    'Sample node to replace nodes
    'Set currNode = xmlDoc.documentElement.childNodes.Item(0).childNodes.Item(1)
    'Set currNode = xmlDoc.documentElement.childNodes.Item(0).selectSingleNode("cable_tipo")
    'MsgBox currNode.nodeTypedValue
    'currNode.nodeTypedValue = "SSIF1"
    'Set currNode = xmlDoc.documentElement.childNodes.Item(1).childNodes.Item(1)
    'MsgBox currNode.nodeTypedValue
    'currNode.nodeTypedValue = "SSIF2"
    
    
    
    
    'Sample code to add nodes to document
    'How do you insert line feeds?
    'Set MyNode = xmlDoc.createNode(NODE_ELEMENT, "Wire1", "")
    'Set currNode = root.insertBefore(MyNode, Null) 'root.childNodes.Item(0))
    'Set MyNode = xmlDoc.createNode(NODE_ELEMENT, "Property1", "")
    'Set currNode = currNode.insertBefore(MyNode, Null) 'root.childNodes.Item(0))
    'Set MyNode = xmlDoc.createNode(NODE_ELEMENT, "Wire2", "")
    'Set currNode = root.insertBefore(MyNode, Null) 'root.childNodes.Item(0))
    'Set MyNode = xmlDoc.createNode(NODE_ELEMENT, "Property1", "")
    'Set currNode = currNode.insertBefore(MyNode, Null) 'root.childNodes.Item(0))
    
    
    counter = counter + 1
    s = f.ReadLine
    
    If Len(s) > 0 Then
        'Sample code code to clone child node 1
        'Set currNode = root.childNodes.Item(0)
        'Set MyNewNode = currNode.cloneNode(True)
        Set MyNewNode = currNode0.cloneNode(True) 'root.childNodes.Item(0).cloneNode(True)
        root.appendChild MyNewNode
        Set currNode = MyNewNode
        'MsgBox xmlDoc.xml
    End If
    

Loop
'Loop end


'Create output file
'Dim fso
Dim txtfile
'Set fso = CreateObject("Scripting.FileSystemObject")
If StrComp(strCmd, "-BRIDGES") <> 0 Then
    Set txtfile = fso.CreateTextFile(g_sBasePath & "testfile.xml", True)
Else
    Set txtfile = fso.CreateTextFile(g_sBasePath & "bridges.xml", True)
End If
'txtfile.Write ("This is a test. ") ' Write a line.
' Write a line with a newline character.
'txtfile.WriteLine ("Testing 1, 2, 3.")
' Write three newline characters to the file.
'txtfile.WriteBlankLines (3)





txtfile.Write xmlDoc.xml
txtfile.Close



'XSL transform
Dim xslt As New MSXML2.XSLTemplate
Dim xslDoc As New MSXML2.FreeThreadedDOMDocument
'Dim xmlDoc As New Msxml2.DOMDocument
Dim xslProc As IXSLProcessor
xslDoc.async = False
If StrComp(strCmd, "-BRIDGES") <> 0 Then
    xslDoc.Load "wiring.xsl"
Else
    xslDoc.Load "bridges.xsl"
End If
Set xslt.stylesheet = xslDoc
xmlDoc.async = False
If StrComp(strCmd, "-BRIDGES") <> 0 Then
    xmlDoc.Load "testfile.xml"
Else
    xmlDoc.Load "bridges.xml"
End If
Set xslProc = xslt.createProcessor()
xslProc.input = xmlDoc
xslProc.Transform
'MsgBox xslProc.output


Set fso = CreateObject("Scripting.FileSystemObject")
If StrComp(strCmd, "-BRIDGES") <> 0 Then
    Set txtfile = fso.CreateTextFile(g_sBasePath & "testfile.htm", True)
Else
    Set txtfile = fso.CreateTextFile(g_sBasePath & "bridges.htm", True)
End If

'Close files
txtfile.Write xslProc.output
txtfile.Close
f.Close
End
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
