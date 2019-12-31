VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRegistro"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Dim hKey As Long, MainKeyHandle As Long
Dim rtn As Long, lBuffer As Long, sBuffer As String
Dim lBufferSize As Long
Dim lDataSize As Long
Dim ByteArray() As Byte

Function SetDWORDValue(SubKey As String, Entry As String, Value As Long)

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, SubKey, 0, KEY_WRITE, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key was open successfully then
      rtn = RegSetValueExA(hKey, Entry, 0, REG_DWORD, Value, 4) 'write the value
      If Not rtn = ERROR_SUCCESS Then   'if there was an error writting the value
         If DisplayErrorMsg = True Then 'if the user want errors displayed
            MsgBox ErrorMsg(rtn)        'display the error
         End If
      End If
      rtn = RegCloseKey(hKey) 'close the key
   Else 'if there was an error opening the key
      If DisplayErrorMsg = True Then 'if the user want errors displayed
         MsgBox ErrorMsg(rtn) 'display the error
      End If
   End If
End If

End Function
Function GetDWORDValue(SubKey As String, Entry As String)

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, SubKey, 0, KEY_READ, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key could be opened then
      rtn = RegQueryValueExA(hKey, Entry, 0, REG_DWORD, lBuffer, 4) 'get the value from the registry
      If rtn = ERROR_SUCCESS Then 'if the value could be retreived then
         rtn = RegCloseKey(hKey)  'close the key
         GetDWORDValue = lBuffer  'return the value
      Else                        'otherwise, if the value couldnt be retreived
         GetDWORDValue = "Error"  'return Error to the user
         If DisplayErrorMsg = True Then 'if the user wants errors displayed
            MsgBox ErrorMsg(rtn)        'tell the user what was wrong
         End If
      End If
   Else 'otherwise, if the key couldnt be opened
      GetDWORDValue = "Error"        'return Error to the user
      If DisplayErrorMsg = True Then 'if the user wants errors displayed
         MsgBox ErrorMsg(rtn)        'tell the user what was wrong
      End If
   End If
End If

End Function
Function SetBinaryValue(SubKey As String, Entry As String, Value As String)
Dim i As Long

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, SubKey, 0, KEY_WRITE, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key was open successfully then
      lDataSize = Len(Value)
      ReDim ByteArray(lDataSize)
      For i = 1 To lDataSize
      ByteArray(i) = Asc(Mid$(Value, i, 1))
      Next
      rtn = RegSetValueExB(hKey, Entry, 0, REG_BINARY, ByteArray(1), lDataSize) 'write the value
      If Not rtn = ERROR_SUCCESS Then   'if the was an error writting the value
         If DisplayErrorMsg = True Then 'if the user want errors displayed
            MsgBox ErrorMsg(rtn)        'display the error
         End If
      End If
      rtn = RegCloseKey(hKey) 'close the key
   Else 'if there was an error opening the key
      If DisplayErrorMsg = True Then 'if the user wants errors displayed
         MsgBox ErrorMsg(rtn) 'display the error
      End If
   End If
End If

End Function


Function GetBinaryValue(SubKey As String, Entry As String)

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, SubKey, 0, KEY_READ, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key could be opened
      lBufferSize = 1
      rtn = RegQueryValueEx(hKey, Entry, 0, REG_BINARY, 0, lBufferSize) 'get the value from the registry
      sBuffer = Space(lBufferSize)
      rtn = RegQueryValueEx(hKey, Entry, 0, REG_BINARY, sBuffer, lBufferSize) 'get the value from the registry
      If rtn = ERROR_SUCCESS Then 'if the value could be retreived then
         rtn = RegCloseKey(hKey)  'close the key
         GetBinaryValue = sBuffer 'return the value to the user
      Else                        'otherwise, if the value couldnt be retreived
         GetBinaryValue = "Error" 'return Error to the user
         If DisplayErrorMsg = True Then 'if the user wants to errors displayed
            MsgBox ErrorMsg(rtn)  'display the error to the user
         End If
      End If
   Else 'otherwise, if the key couldnt be opened
      GetBinaryValue = "Error" 'return Error to the user
      If DisplayErrorMsg = True Then 'if the user wants to errors displayed
         MsgBox ErrorMsg(rtn)  'display the error to the user
      End If
   End If
End If

End Function
Sub DeleteKey(Keyname As String)

Call ParseKey(Keyname, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, Keyname, 0, KEY_WRITE, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key could be opened then
      rtn = RegDeleteKey(hKey, Keyname) 'delete the key
      rtn = RegCloseKey(hKey)  'close the key
   End If
End If

End Sub

Function ErrorMsg(lErrorCode As Long) As String
    
'If an error does accurr, and the user wants error messages displayed, then
'display one of the following error messages

Select Case lErrorCode
       Case 1009, 1015
            ErrorMsg = "The Registry Database is corrupt!"
       Case 2, 1010
            ErrorMsg = "Bad Key Name"
       Case 1011
            ErrorMsg = "Can't Open Key"
       Case 4, 1012
            ErrorMsg = "Can't Read Key"
       Case 5
            ErrorMsg = "Access to this key is denied"
       Case 1013
            ErrorMsg = "Can't Write Key"
       Case 8, 14
            ErrorMsg = "Out of memory"
       Case 87
            ErrorMsg = "Invalid Parameter"
       Case 234
            ErrorMsg = "There is more data than the buffer has been allocated to hold."
       Case Else
            ErrorMsg = "Undefined Error Code:  " & Str$(lErrorCode)
End Select

End Function
Function CreateKey(SubKey As String)

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegCreateKey(MainKeyHandle, SubKey, hKey) 'create the key
   If rtn = ERROR_SUCCESS Then 'if the key was created then
      rtn = RegCloseKey(hKey)  'close the key
   End If
End If

End Function
Function SetStringValue(SubKey As String, Entry As String, Value As String)

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, SubKey, 0, KEY_WRITE, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key was open successfully then
      rtn = RegSetValueEx(hKey, Entry, 0, REG_SZ, ByVal Value, Len(Value)) 'write the value
      If Not rtn = ERROR_SUCCESS Then   'if there was an error writting the value
         If DisplayErrorMsg = True Then 'if the user wants errors displayed
            MsgBox ErrorMsg(rtn)        'display the error
         End If
      End If
      rtn = RegCloseKey(hKey) 'close the key
   Else 'if there was an error opening the key
      If DisplayErrorMsg = True Then 'if the user wants errors displayed
         MsgBox ErrorMsg(rtn)        'display the error
      End If
   End If
End If

End Function

Function GetMainKeyHandle(MainKeyName As String) As Long

Const HKEY_CLASSES_ROOT = &H80000000
Const HKEY_CURRENT_USER = &H80000001
Const HKEY_LOCAL_MACHINE = &H80000002
Const HKEY_USERS = &H80000003
Const HKEY_PERFORMANCE_DATA = &H80000004
Const HKEY_CURRENT_CONFIG = &H80000005
Const HKEY_DYN_DATA = &H80000006
   
Select Case MainKeyName
       Case "HKEY_CLASSES_ROOT"
            GetMainKeyHandle = HKEY_CLASSES_ROOT
       Case "HKEY_CURRENT_USER"
            GetMainKeyHandle = HKEY_CURRENT_USER
       Case "HKEY_LOCAL_MACHINE"
            GetMainKeyHandle = HKEY_LOCAL_MACHINE
       Case "HKEY_USERS"
            GetMainKeyHandle = HKEY_USERS
       Case "HKEY_PERFORMANCE_DATA"
            GetMainKeyHandle = HKEY_PERFORMANCE_DATA
       Case "HKEY_CURRENT_CONFIG"
            GetMainKeyHandle = HKEY_CURRENT_CONFIG
       Case "HKEY_DYN_DATA"
            GetMainKeyHandle = HKEY_DYN_DATA
End Select

End Function
Function GetStringValue(SubKey As String, Entry As String)

Call ParseKey(SubKey, MainKeyHandle)

If MainKeyHandle Then
   rtn = RegOpenKeyEx(MainKeyHandle, SubKey, 0, KEY_READ, hKey) 'open the key
   If rtn = ERROR_SUCCESS Then 'if the key could be opened then
      sBuffer = Space(255)     'make a buffer
      lBufferSize = Len(sBuffer)
      rtn = RegQueryValueEx(hKey, Entry, 0, REG_SZ, sBuffer, lBufferSize) 'get the value from the registry
      If rtn = ERROR_SUCCESS Then 'if the value could be retreived then
         rtn = RegCloseKey(hKey)  'close the key
         sBuffer = Trim(sBuffer)
         GetStringValue = Mid$(sBuffer, 1, InStr(1, sBuffer, Chr$(0)) - 1)
         'GetStringValue = Left(sBuffer, Len(sBuffer) - 1) 'return the value to the user
      Else                        'otherwise, if the value couldnt be retreived
         GetStringValue = "Error" 'return Error to the user
      End If
   Else 'otherwise, if the key couldnt be opened
      GetStringValue = ""       'return Error to the user
   End If
End If

End Function

Private Sub ParseKey(Keyname As String, Keyhandle As Long)
    
rtn = InStr(Keyname, "\") 'return if "\" is contained in the Keyname

If Left(Keyname, 5) <> "HKEY_" Or Right(Keyname, 1) = "\" Then 'if the is a "\" at the end of the Keyname then
   MsgBox "Incorrect Format:" + Chr(10) + Chr(10) + Keyname 'display error to the user
   Exit Sub 'exit the procedure
ElseIf rtn = 0 Then 'if the Keyname contains no "\"
   Keyhandle = GetMainKeyHandle(Keyname)
   Keyname = "" 'leave Keyname blank
Else 'otherwise, Keyname contains "\"
   Keyhandle = GetMainKeyHandle(Left(Keyname, rtn - 1)) 'seperate the Keyname
   Keyname = Right(Keyname, Len(Keyname) - rtn)
End If

End Sub

