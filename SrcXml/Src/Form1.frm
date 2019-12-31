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
   Visible         =   0   'False
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   615
      Left            =   360
      TabIndex        =   0
      Top             =   240
      Width           =   1335
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Const g_sBasePath As String = ".\"


Private Sub Form_Load()
Dim xmlDoc As New Msxml2.DOMDocument
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
Set f = fso.OpenTextFile("cableado.txt", ForReading, TristateFalse)
s = f.ReadLine
length_s = Len(s)
'***************************************************************************

    
Dim counter As Integer
counter = 1
Set currNode = root.childNodes.Item(0)
Set currNode0 = currNode.cloneNode(True)

Do While Len(s) '& counter < 4)
    

    
    'Replace tag
    s2 = read_field(s, ";")
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("cable_num")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("cable_tipo")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("pj_de")
        currNodeTemp.nodeTypedValue = s2
        Set currNodeTemp = currNode.selectSingleNode("de_conector")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("pj_a")
        currNodeTemp.nodeTypedValue = s2
        Set currNodeTemp = currNode.selectSingleNode("a_conector")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("de_equipo")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("a_equipo")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("de_situadoen")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("a_situadoen")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("de_conectortipo")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("a_conectortipo")
        currNodeTemp.nodeTypedValue = s2
    End If
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = currNode.selectSingleNode("clongi")
        currNodeTemp.nodeTypedValue = s2
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

    Do While Len(ssub1)
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("pin_de")
        currNodeTemp.nodeTypedValue = s2sub1
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("ref_hilo")
        currNodeTemp.nodeTypedValue = s2sub1
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("funcion")
        currNodeTemp.nodeTypedValue = s2sub1
        s2sub1 = read_field(ssub1, ";")
        Set currNodeTemp = currNodeSub1.selectSingleNode("pin_a")
        currNodeTemp.nodeTypedValue = s2sub1
        
        ssub1 = fsub1.ReadLine
        
        If Len(ssub1) > 0 Then
            Set MyNewNode = currNodeSub1.cloneNode(True)
            currNode.appendChild MyNewNode
            Set currNodeSub1 = MyNewNode
        End If
        
    Loop
    fsub1.Close
    '*********************************************************************************
    
    s2 = read_field(s, ";")
    If StrComp(s2, "") Then
        Set currNodeTemp = xmlDoc.documentElement.childNodes.Item(0).selectSingleNode("cpar")
        currNodeTemp.nodeTypedValue = s2
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
Set txtfile = fso.CreateTextFile(g_sBasePath & "testfile.xml", True)
'txtfile.Write ("This is a test. ") ' Write a line.
' Write a line with a newline character.
'txtfile.WriteLine ("Testing 1, 2, 3.")
' Write three newline characters to the file.
'txtfile.WriteBlankLines (3)





txtfile.Write xmlDoc.xml
txtfile.Close



'XSL transform
Dim xslt As New Msxml2.XSLTemplate
Dim xslDoc As New Msxml2.FreeThreadedDOMDocument
'Dim xmlDoc As New Msxml2.DOMDocument
Dim xslProc As IXSLProcessor
xslDoc.async = False
xslDoc.Load "wiring.xsl"
Set xslt.stylesheet = xslDoc
xmlDoc.async = False
xmlDoc.Load "testfile.xml"
Set xslProc = xslt.createProcessor()
xslProc.input = xmlDoc
xslProc.Transform
'MsgBox xslProc.output


Set fso = CreateObject("Scripting.FileSystemObject")
Set txtfile = fso.CreateTextFile(g_sBasePath & "testfile.htm", True)

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
