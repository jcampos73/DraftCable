; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDialogPlacePart
LastTemplate=CView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DraftDraw.h"
LastPage=0

ClassCount=31
Class1=CDraftDrawApp
Class2=CDraftDrawDoc
Class3=CDraftDrawView
Class4=CMainFrame
Class7=CAboutDlg

ResourceCount=51
Resource1=IDR_CONTEXT (Spanish (Castilian))
Resource2=IDD_DEBUG_SHPINFO
Class5=CChildFrame
Class6=CLeftView
Class8=CShape
Resource3=IDR_PLACE1
Resource4=IDD_SAVEAS (Spanish (Castilian))
Resource5=IDD_ABOUTBOX (Spanish (Modern))
Resource6=IDR_PATCHPTYPE (Spanish (Modern))
Resource7=IDD_PLACEPART
Resource8=IDD_TEXT
Resource9=IDD_PAGESIZE
Resource10=IDR_PLACE1 (Spanish (Modern))
Class9=CNetAlias
Resource11=IDD_NETALIAS
Class10=CDialogPin
Resource12=IDD_PATCHPPROP
Class11=CDialogPlacePart
Class12=CWndSplitter
Resource13=IDR_PLACE (Spanish (Modern))
Resource14=IDD_CONFIG
Class13=CPartEdit
Resource15=IDD_PAGEGRID
Class14=CDialogPageProperties
Class15=CPageSize
Resource16=IDR_DRAWING (Spanish (Modern))
Class16=CDialogConfig
Resource17=IDD_SAVEAS
Class17=CDialogSaveas
Resource18=IDR_CONTEXT
Resource19=IDD_CABLEDIT (Spanish (Modern))
Class18=CPageGrid
Resource20=IDD_TBEDIT_FORM (Spanish (Modern))
Resource21=IDD_PIN
Resource22=IDD_PARTEDIT (Spanish (Modern))
Class19=CDialogPatchPProperties
Resource23=IDR_DRAFTDTYPE (Spanish (Modern))
Class20=CDialogText
Resource24=IDR_MAINFRAME (Spanish (Modern))
Resource25=IDR_DRAWING
Resource26=IDD_PARTEDIT
Resource27=IDR_EDITDROPDOWN (Spanish (Castilian))
Resource28=IDR_DRAFTDTYPE
Resource29=IDR_CABLEDIT
Resource30=IDD_PLACEPART (Spanish (Castilian))
Resource31=IDD_PAGEGRID (Spanish (Castilian))
Resource32=IDD_ABOUTBOX
Resource33=IDD_SAVEASDB
Resource34=IDD_PARTPROP
Resource35=IDD_PATCHPPROP (Spanish (Castilian))
Resource36=IDD_CABLEDIT
Resource37=IDD_NAME
Resource38=IDD_NETALIAS (Spanish (Castilian))
Resource39=IDD_NETLIST
Resource40=IDR_DEBUG_CONTEXT
Class21=CPartPView
Resource41=IDD_GRAPH_DEN
Resource42=IDD_PIN (Spanish (Castilian))
Class22=CDialogDebugShpInfo
Resource43=IDD_TBEDIT_FORM
Class23=CNameDlg
Resource44=IDR_PLACE
Class24=CDialogNetList
Class25=CButtonStates
Resource45=IDD_PLACEPART2
Class26=CPreviewFrame
Class27=CListCtrlCR
Resource46=IDR_PATCHPTYPE
Class28=CDialogSaveasDB
Resource47=IDD_PAGESIZE (Spanish (Castilian))
Class29=CDialogPartProp
Resource48=IDD_TEXT (Spanish (Castilian))
Resource49=IDD_CONFIG (Spanish (Castilian))
Resource50=IDR_MAINFRAME
Class30=CDialogGraphDen
Class31=CGraphDView
Resource51=IDR_MAINFRAME1

[CLS:CDraftDrawApp]
Type=0
HeaderFile=DraftDraw.h
ImplementationFile=DraftDraw.cpp
Filter=N
LastObject=CDraftDrawApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CDraftDrawDoc]
Type=0
HeaderFile=DraftDrawDoc.h
ImplementationFile=DraftDrawDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CDraftDrawDoc

[CLS:CDraftDrawView]
Type=0
HeaderFile=DraftDrawView.h
ImplementationFile=DraftDrawView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CDraftDrawView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=CMainFrame


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame

[CLS:CLeftView]
Type=0
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp
Filter=T
BaseClass=CTreeView
VirtualFilter=VWC
LastObject=CLeftView

[CLS:CAboutDlg]
Type=0
HeaderFile=DraftDraw.cpp
ImplementationFile=DraftDraw.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_WINDOW_SPLIT
Command9=ID_VIEW_LARGEICON
Command10=ID_VIEW_SMALLICON
Command11=ID_VIEW_LIST
Command12=ID_VIEW_DETAILS
Command13=ID_VIEW_BYNAME
Command14=ID_VIEW_AUTOARRANGE
Command15=ID_VIEW_LINEUP
Command16=ID_APP_ABOUT
CommandCount=16

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_REDO
Command10=ID_APP_ABOUT
CommandCount=10

[MNU:IDR_DRAFTDTYPE]
Type=1
Class=CDraftDrawView
Command1=ID_FILE_NEW
Command2=ID_FILE_NEW_SHEET
Command3=ID_FILE_NEW_RACK
Command4=ID_FILE_NEW_PATCHP
Command5=ID_FILE_NEW_COMPONENTE
Command6=ID_FILE_OPEN
Command7=ID_FILE_CLOSE
Command8=ID_FILE_SAVE
Command9=ID_FILE_SAVE_AS
Command10=ID_FILE_SAVE_SHEET_CURRENT
Command11=ID_FILE_PRINT
Command12=ID_FILE_PRINT_PREVIEW
Command13=ID_FILE_PRINT_SETUP
Command14=ID_FILE_MRU_FILE1
Command15=ID_APP_EXIT
Command16=ID_ARCHIVO_GENCABLE
Command17=ID_EDIT_UNDO
Command18=ID_EDIT_CUT
Command19=ID_EDIT_COPY
Command20=ID_EDIT_PASTE
Command21=ID_EDIT_PART
Command22=ID_VIEW_TOOLBAR
Command23=ID_VIEW_STATUS_BAR
Command24=ID_WINDOW_SPLIT
Command25=ID_VIEW_LARGEICON
Command26=ID_VIEW_SMALLICON
Command27=ID_VIEW_LIST
Command28=ID_VIEW_DETAILS
Command29=ID_VIEW_BYNAME
Command30=ID_VIEW_AUTOARRANGE
Command31=ID_VIEW_LINEUP
Command32=ID_VIEW_GRAPH_DEN
Command33=ID_PLACE_PART
Command34=ID_PLACE_WIRE
Command35=ID_PLACE_BUS
Command36=ID_PLACE_PORT
Command37=ID_PLACE_PORTRIGHT
Command38=ID_PLACE_BUSENTRY
Command39=ID_PLACE_NOCON
Command40=ID_PLACE_BUSJUNC
Command41=ID_BUTTON_RECT
Command42=ID_BUTTON_ELLIPSE
Command43=ID_BUTTON_LINE
Command44=ID_BUTTON_POLYLINE
Command45=ID_BUTTON_ARC
Command46=ID_IMG_POLYGONPOPUP
Command47=ID_PLACE_TEXT
Command48=ID_PLACE_PICTURE
Command49=ID_PLACE_SHEETS
Command50=ID_ROTATE
Command51=ID_IMG_FLIPHORZ
Command52=ID_IMG_FLIPVERT
Command53=ID_BUTTON_SELECT
Command54=ID_TOOL_SIC
Command55=ID_EDIT_RACK
Command56=ID_TOOL_OPTION_PAGE
Command57=ID_TOOL_CONFIG
Command58=ID_WINDOW_NEW
Command59=ID_WINDOW_CASCADE
Command60=ID_WINDOW_TILE_HORZ
Command61=ID_WINDOW_ARRANGE
Command62=ID_HELP_FINDER
Command63=ID_APP_ABOUT
CommandCount=63

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_ROTATE
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_CANCEL
Command10=ID_SUP
Command11=ID_EDIT_CUT
Command12=ID_ESCAPE
Command13=ID_HELP
Command14=ID_CONTEXT_HELP
Command15=ID_NEXT_PANE
Command16=ID_PREV_PANE
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_OK
Command20=ID_NEXT_FIELD
Command21=ID_EDIT_CUT
Command22=ID_EDIT_UNDO
CommandCount=22

[CLS:CShape]
Type=0
HeaderFile=Shape.h
ImplementationFile=Shape.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CShape

[TB:IDR_DRAWING]
Type=1
Class=?
Command1=ID_BUTTON_SELECT
Command2=ID_BUTTON_RULE
Command3=ID_SNAP_TOGRID
Command4=ID_SNAP_TOGRID2
Command5=ID_ZOOM_IN
Command6=ID_ZOOM_OUT
Command7=ID_ZOOM_AREA
Command8=ID_ZOOM_ALL
Command9=ID_ZOOM_CURSORPLUS
Command10=ID_ZOOM_CURSORMINUS
Command11=ID_BUTTON_RECT
Command12=ID_BUTTON_ELLIPSE
Command13=ID_BUTTON_ARC
Command14=ID_BUTTON_LINE
Command15=ID_BUTTON_POLYLINE
Command16=ID_BUTTON_CURVE
Command17=ID_PLACE_TEXT
Command18=ID_PLACE_PICTURE
Command19=ID_BUTTON_ROTATE
CommandCount=19

[TB:IDR_DRAWING (Spanish (Modern))]
Type=1
Class=?
Command1=ID_BUTTON_SELECT
Command2=ID_BUTTON_RULE
Command3=ID_SNAP_TOGRID
Command4=ID_SNAP_TOGRID2
Command5=ID_ZOOM_IN
Command6=ID_ZOOM_OUT
Command7=ID_ZOOM_AREA
Command8=ID_ZOOM_ALL
Command9=ID_ZOOM_CURSORPLUS
Command10=ID_ZOOM_CURSORMINUS
Command11=ID_BUTTON_RECT
Command12=ID_BUTTON_ELLIPSE
Command13=ID_BUTTON_LINE
Command14=ID_BUTTON_POLYLINE
Command15=ID_BUTTON_ARC
Command16=ID_PLACE_TEXT
CommandCount=16

[TB:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_VIEW_LARGEICON
Command9=ID_VIEW_SMALLICON
Command10=ID_VIEW_LIST
Command11=ID_VIEW_DETAILS
Command12=ID_APP_ABOUT
CommandCount=12

[MNU:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_WINDOW_SPLIT
Command9=ID_VIEW_LARGEICON
Command10=ID_VIEW_SMALLICON
Command11=ID_VIEW_LIST
Command12=ID_VIEW_DETAILS
Command13=ID_VIEW_BYNAME
Command14=ID_VIEW_AUTOARRANGE
Command15=ID_VIEW_LINEUP
Command16=ID_APP_ABOUT
CommandCount=16

[MNU:IDR_DRAFTDTYPE (Spanish (Modern))]
Type=1
Class=CDraftDrawView
Command1=ID_FILE_NEW
Command2=ID_FILE_NEW_COMPONENTE
Command3=ID_FILE_NEW_RACK
Command4=ID_FILE_NEW_PATCHP
Command5=ID_FILE_OPEN
Command6=ID_FILE_CLOSE
Command7=ID_FILE_SAVE
Command8=ID_FILE_SAVE_AS
Command9=ID_FILE_PRINT
Command10=ID_FILE_PRINT_PREVIEW
Command11=ID_FILE_PRINT_SETUP
Command12=ID_FILE_MRU_FILE1
Command13=ID_APP_EXIT
Command14=ID_ARCHIVO_GENCABLE
Command15=ID_EDIT_UNDO
Command16=ID_EDIT_CUT
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_EDIT_PART
Command20=ID_VIEW_TOOLBAR
Command21=ID_VIEW_STATUS_BAR
Command22=ID_WINDOW_SPLIT
Command23=ID_VIEW_LARGEICON
Command24=ID_VIEW_SMALLICON
Command25=ID_VIEW_LIST
Command26=ID_VIEW_DETAILS
Command27=ID_VIEW_BYNAME
Command28=ID_VIEW_AUTOARRANGE
Command29=ID_VIEW_LINEUP
Command30=ID_BUTTON_LINE
Command31=ID_BUTTON_SELECT
Command32=ID_TOOL_OPTION_PAGE
Command33=ID_TOOL_CONFIG
Command34=ID_WINDOW_NEW
Command35=ID_WINDOW_CASCADE
Command36=ID_WINDOW_TILE_HORZ
Command37=ID_WINDOW_ARRANGE
Command38=ID_APP_ABOUT
CommandCount=38

[ACL:IDR_MAINFRAME (Spanish (Modern))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_ROTATE
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_CANCEL
Command10=ID_SUP
Command11=ID_EDIT_CUT
Command12=ID_ESCAPE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_OK
Command18=ID_NEXT_FIELD
Command19=ID_EDIT_CUT
Command20=ID_EDIT_UNDO
CommandCount=20

[DLG:IDD_ABOUTBOX (Spanish (Modern))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_PLACE (Spanish (Modern))]
Type=1
Class=?
Command1=ID_PLACE_PART
Command2=ID_PLACE_CHASSIS
Command3=ID_PLACE_PIN
Command4=ID_PLACE_PORT
Command5=ID_PLACE_WIRE
Command6=ID_PLACE_BUSENTRY
Command7=ID_PLACE_BUS
Command8=ID_PLACE_NOCON
Command9=ID_EDIT_PART
Command10=ID_EDIT_RACK
CommandCount=10

[MNU:IDR_CONTEXT]
Type=1
Class=?
Command1=ID_EDIT_NETALIAS
Command2=ID_SELECT_NET
Command3=ID_ORDER_TOP
Command4=ID_ORDER_BOTTOM
CommandCount=4

[DLG:IDD_NETALIAS]
Type=1
Class=CNetAlias
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ALIAS,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_VERTICAL,button,1342242819
Control7=IDC_SIZE,combobox,1344340226
Control8=IDC_STATIC,static,1342308352

[CLS:CNetAlias]
Type=0
HeaderFile=NetAlias.h
ImplementationFile=NetAlias.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_PIN]
Type=1
Class=CDialogPin
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO_NUMBER,combobox,1344340226
Control4=IDC_COMBO_SHAPE,combobox,1344340227
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_JACK,button,1342242819
Control8=IDC_COMBO_TYPE,combobox,1344340226
Control9=IDC_STATIC,static,1342308352

[CLS:CDialogPin]
Type=0
HeaderFile=DialogPin.h
ImplementationFile=DialogPin.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogPin
VirtualFilter=dWC

[DLG:IDD_PLACEPART]
Type=1
Class=CDialogPlacePart
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LISTPART,SysListView32,1350631425

[CLS:CDialogPlacePart]
Type=0
HeaderFile=DialogPlacePart.h
ImplementationFile=DialogPlacePart.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LISTPART
VirtualFilter=dWC

[CLS:CWndSplitter]
Type=0
HeaderFile=WndSplitter.h
ImplementationFile=WndSplitter.cpp
BaseClass=CSplitterWnd
Filter=W
LastObject=CWndSplitter
VirtualFilter=pWC

[DLG:IDD_CABLEDIT (Spanish (Modern))]
Type=1
Class=?
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631937
Control4=IDC_FILE,combobox,1344340227
Control5=IDC_STATIC_TYPE,static,1342308352
Control6=IDC_STATIC_NUMBER,static,1342308352
Control7=IDC_NAME,edit,1350631552
Control8=IDC_BUTTON_SAVE,button,1476460544
Control9=IDC_BUTTON_SAVEAS,button,1342242816
Control10=IDC_BUTTON_NEW,button,1342242816
Control11=IDC_BUTTON_EDIT,button,1342242816

[DLG:IDD_PARTEDIT (Spanish (Modern))]
Type=1
Class=CPartEdit
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631937
Control4=IDC_PROPERTY,combobox,1344340227
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_NAME,edit,1350631552

[CLS:CPartEdit]
Type=0
HeaderFile=PartEdit.h
ImplementationFile=PartEdit.cpp
BaseClass=CDialog
Filter=D
LastObject=CPartEdit
VirtualFilter=dWC

[DLG:IDD_PAGESIZE]
Type=1
Class=CPageSize
ControlCount=19
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_RADIO_A4,button,1342177284
Control5=IDC_RADIO_A3,button,1342177284
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_RADIO_CUSTOM,button,1342177289
Control13=IDC_WIDTH,edit,1350631552
Control14=IDC_HEIGHT,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_RADIO_MILLIM,button,1342177284
Control19=IDC_RADIO_INCH,button,1342177284

[CLS:CDialogPageProperties]
Type=0
HeaderFile=DialogPageProperties.h
ImplementationFile=DialogPageProperties.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=CDialogPageProperties
VirtualFilter=hWC

[CLS:CPageSize]
Type=0
HeaderFile=PageSize.h
ImplementationFile=PageSize.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CPageSize
VirtualFilter=idWC

[DLG:IDD_CONFIG]
Type=1
Class=CDialogConfig
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=ID_HELP,button,1342242816
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EXAM,button,1342242816
Control6=IDC_STATIC,static,1342308352

[CLS:CDialogConfig]
Type=0
HeaderFile=DialogConfig.h
ImplementationFile=DialogConfig.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogConfig
VirtualFilter=dWC

[DLG:IDD_SAVEAS]
Type=1
Class=CDialogSaveas
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO1,combobox,1344340226

[CLS:CDialogSaveas]
Type=0
HeaderFile=DialogSaveas.h
ImplementationFile=DialogSaveas.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogSaveas
VirtualFilter=dWC

[TB:IDR_PLACE1 (Spanish (Modern))]
Type=1
Class=?
Command1=ID_PLACE_PART
Command2=ID_PLACE_CHASSIS
Command3=ID_PLACE_WIRE
Command4=ID_PLACE_BUSENTRY
Command5=ID_PLACE_NETALIAS
Command6=ID_PLACE_PIN
Command7=ID_ZOOM_IN
Command8=ID_ZOOM_OUT
Command9=ID_EDIT_PART
Command10=ID_EDIT_RACK
CommandCount=10

[DLG:IDD_PAGEGRID]
Type=1
Class=CPageGrid
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308354
Control3=IDC_WIDTH,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CPageGrid]
Type=0
HeaderFile=PageGrid.h
ImplementationFile=PageGrid.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CPageGrid
VirtualFilter=idWC

[DLG:IDD_TBEDIT_FORM (Spanish (Modern))]
Type=1
Class=?
ControlCount=0

[MNU:IDR_PATCHPTYPE (Spanish (Modern))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_NEW_COMPONENTE
Command3=ID_FILE_NEW_RACK
Command4=ID_FILE_OPEN
Command5=ID_FILE_CLOSE
Command6=ID_FILE_SAVE
Command7=ID_FILE_SAVE_AS
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_MRU_FILE1
Command12=ID_APP_EXIT
Command13=ID_ARCHIVO_GENCABLE
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_CUT
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_EDIT_PART
Command19=ID_VIEW_TOOLBAR
Command20=ID_VIEW_STATUS_BAR
Command21=ID_WINDOW_SPLIT
Command22=ID_VIEW_LARGEICON
Command23=ID_VIEW_SMALLICON
Command24=ID_VIEW_LIST
Command25=ID_VIEW_DETAILS
Command26=ID_VIEW_BYNAME
Command27=ID_VIEW_AUTOARRANGE
Command28=ID_VIEW_LINEUP
Command29=ID_BUTTON_LINE
Command30=ID_BUTTON_SELECT
Command31=ID_TOOL_OPTION_PAGE
Command32=ID_TOOL_CONFIG
Command33=ID_WINDOW_NEW
Command34=ID_WINDOW_CASCADE
Command35=ID_WINDOW_TILE_HORZ
Command36=ID_WINDOW_ARRANGE
Command37=ID_APP_ABOUT
CommandCount=37

[DLG:IDD_PATCHPPROP]
Type=1
Class=CDialogPatchPProperties
ControlCount=20
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631429
Control4=IDC_LIST2,SysListView32,1350631429
Control5=ID_EDITTB1,button,1342242816
Control6=ID_DELTB1,button,1342242816
Control7=ID_NEWTB1,button,1342242816
Control8=ID_NEWTB2,button,1342242816
Control9=ID_EDITTB2,button,1342242816
Control10=ID_DELTB2,button,1342242816
Control11=IDC_NAME1,combobox,1344340226
Control12=IDC_FILE1,combobox,1344340227
Control13=IDC_NAME2,combobox,1344340226
Control14=IDC_FILE2,combobox,1344340227
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352

[CLS:CDialogPatchPProperties]
Type=0
HeaderFile=DialogPatchPProperties.h
ImplementationFile=DialogPatchPProperties.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogPatchPProperties
VirtualFilter=dWC

[DLG:IDD_TEXT]
Type=1
Class=CDialogText
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ALIAS,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_VERTICAL,button,1342242819
Control7=IDC_STATIC,button,1342177287
Control8=IDC_FONT,edit,1342245060
Control9=ID_CHANGE_FONT,button,1342242816

[CLS:CDialogText]
Type=0
HeaderFile=DialogText.h
ImplementationFile=DialogText.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogText
VirtualFilter=dWC

[MNU:IDR_CONTEXT (Spanish (Castilian))]
Type=1
Class=?
Command1=ID_EDIT_NETALIAS
Command2=ID_SELECT_NET
Command3=ID_EDIT_TB
Command4=ID_ORDER_TOP
Command5=ID_ORDER_BOTTOM
Command6=ID_EDIT_PROPERTIES
CommandCount=6

[MNU:IDR_PATCHPTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_NEW_COMPONENTE
Command3=ID_FILE_NEW_RACK
Command4=ID_FILE_OPEN
Command5=ID_FILE_CLOSE
Command6=ID_FILE_SAVE
Command7=ID_FILE_SAVE_AS
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_MRU_FILE1
Command12=ID_APP_EXIT
Command13=ID_ARCHIVO_GENCABLE
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_CUT
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_EDIT_PART
Command19=ID_VIEW_TOOLBAR
Command20=ID_VIEW_STATUS_BAR
Command21=ID_WINDOW_SPLIT
Command22=ID_VIEW_LARGEICON
Command23=ID_VIEW_SMALLICON
Command24=ID_VIEW_LIST
Command25=ID_VIEW_DETAILS
Command26=ID_VIEW_BYNAME
Command27=ID_VIEW_AUTOARRANGE
Command28=ID_VIEW_LINEUP
Command29=ID_BUTTON_LINE
Command30=ID_BUTTON_SELECT
Command31=ID_TOOL_OPTION_PAGE
Command32=ID_TOOL_CONFIG
Command33=ID_WINDOW_NEW
Command34=ID_WINDOW_CASCADE
Command35=ID_WINDOW_TILE_HORZ
Command36=ID_WINDOW_ARRANGE
Command37=ID_APP_ABOUT
CommandCount=37

[DLG:IDD_NETALIAS (Spanish (Castilian))]
Type=1
Class=CNetAlias
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ALIAS,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_VERTICAL,button,1342242819
Control7=IDC_SIZE,combobox,1344340226
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_PIN (Spanish (Castilian))]
Type=1
Class=CDialogPin
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO_NUMBER,combobox,1344340226
Control4=IDC_COMBO_SHAPE,combobox,1344340227
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_JACK,button,1342242819
Control8=IDC_COMBO_TYPE,combobox,1344340226
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_PLACEPART (Spanish (Castilian))]
Type=1
Class=CDialogPlacePart
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LISTPART,SysListView32,1350631425

[DLG:IDD_PAGESIZE (Spanish (Castilian))]
Type=1
Class=CPageSize
ControlCount=19
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_RADIO_A4,button,1342177284
Control5=IDC_RADIO_A3,button,1342177284
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_RADIO_CUSTOM,button,1342177289
Control13=IDC_WIDTH,edit,1350631552
Control14=IDC_HEIGHT,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_RADIO_MILLIM,button,1342177284
Control19=IDC_RADIO_INCH,button,1342177284

[DLG:IDD_CONFIG (Spanish (Castilian))]
Type=1
Class=CDialogConfig
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=ID_HELP,button,1342242816
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EXAM,button,1342242816
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_SAVEAS (Spanish (Castilian))]
Type=1
Class=CDialogSaveas
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_COMBO1,combobox,1344340226

[DLG:IDD_PAGEGRID (Spanish (Castilian))]
Type=1
Class=CPageGrid
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308354
Control3=IDC_WIDTH,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_PATCHPPROP (Spanish (Castilian))]
Type=1
Class=CDialogPatchPProperties
ControlCount=20
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631429
Control4=IDC_LIST2,SysListView32,1350631429
Control5=ID_EDITTB1,button,1342242816
Control6=ID_DELTB1,button,1342242816
Control7=ID_NEWTB1,button,1342242816
Control8=ID_NEWTB2,button,1342242816
Control9=ID_EDITTB2,button,1342242816
Control10=ID_DELTB2,button,1342242816
Control11=IDC_NAME1,combobox,1344340226
Control12=IDC_FILE1,combobox,1344340227
Control13=IDC_NAME2,combobox,1344340226
Control14=IDC_FILE2,combobox,1344340227
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352

[DLG:IDD_TEXT (Spanish (Castilian))]
Type=1
Class=CDialogText
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ALIAS,edit,1350631556
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_VERTICAL,button,1342242819
Control7=IDC_STATIC,button,1342177287
Control8=IDC_FONT,edit,1342245060
Control9=ID_CHANGE_FONT,button,1342242816
Control10=IDC_NAME,edit,1350631552
Control11=IDC_STATIC,static,1342308352

[DLG:IDD_CABLEDIT]
Type=1
Class=?
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631937
Control4=IDC_FILE,combobox,1344340227
Control5=IDC_STATIC_TYPE,static,1342308352
Control6=IDC_STATIC_NUMBER,static,1342308352
Control7=IDC_NAME,edit,1350631552
Control8=IDC_BUTTON_SAVE,button,1476460544
Control9=IDC_BUTTON_SAVEAS,button,1342242816
Control10=IDC_BUTTON_NEW,button,1342242816
Control11=IDC_BUTTON_EDIT,button,1342242816

[DLG:IDD_PARTEDIT]
Type=1
Class=CPartEdit
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631937
Control4=IDC_PROPERTY,combobox,1344340227
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_NAME,edit,1350631552
Control8=IDC_BUTTON_EDITLABEL,button,1342242816

[DLG:IDD_TBEDIT_FORM]
Type=1
Class=?
ControlCount=0

[TB:IDR_PLACE]
Type=1
Class=?
Command1=ID_PLACE_PART
Command2=ID_PLACE_CHASSIS
Command3=ID_PLACE_PIN
Command4=ID_PLACE_PORT
Command5=ID_PLACE_PORTRIGHT
Command6=ID_PLACE_NOCON
Command7=ID_PLACE_WIRE
Command8=ID_PLACE_BUSENTRY
Command9=ID_PLACE_BUS
Command10=ID_PLACE_BUSJUNC
Command11=ID_EDIT_PART
Command12=ID_EDIT_RACK
Command13=ID_TOOL_SIC
Command14=ID_ARCHIVO_GENCABLE
CommandCount=14

[TB:IDR_PLACE1]
Type=1
Class=?
Command1=ID_PLACE_PART
Command2=ID_PLACE_CHASSIS
Command3=ID_PLACE_WIRE
Command4=ID_PLACE_BUSENTRY
Command5=ID_PLACE_NETALIAS
Command6=ID_PLACE_PIN
Command7=ID_ZOOM_IN
Command8=ID_ZOOM_OUT
Command9=ID_EDIT_PART
Command10=ID_EDIT_RACK
CommandCount=10

[DLG:IDD_PLACEPART2]
Type=1
Class=?
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_COMBO1,combobox,1344339970
Control5=IDC_LISTPART,SysListView32,1350631429
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_BUTTON_NEW,button,1073807360
Control9=IDC_BUTTON_DEL,button,1073807360
Control10=IDC_BUTTON_NEWLIB,button,1073807360
Control11=IDC_CHECK_ALL,button,1342252035

[CLS:CPartPView]
Type=0
HeaderFile=PartPView.h
ImplementationFile=PartPView.cpp
BaseClass=CView
Filter=C
LastObject=CPartPView

[MNU:IDR_DEBUG_CONTEXT]
Type=1
Class=?
Command1=ID_DEBUG_EDIT_SHPINFO
CommandCount=1

[DLG:IDD_DEBUG_SHPINFO]
Type=1
Class=CDialogDebugShpInfo
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TEXT1,listbox,1352728835
Control4=IDC_TEXT,edit,1350631556

[CLS:CDialogDebugShpInfo]
Type=0
HeaderFile=DialogDebugShpInfo.h
ImplementationFile=DialogDebugShpInfo.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialogDebugShpInfo

[DLG:IDD_NAME]
Type=1
Class=CNameDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_NAME,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:CNameDlg]
Type=0
HeaderFile=NameDlg.h
ImplementationFile=NameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNameDlg
VirtualFilter=dWC

[DLG:IDD_NETLIST]
Type=1
Class=CDialogNetList
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631424
Control4=IDC_STATIC,button,1342177287
Control5=IDC_BUTTON_ICONBIG,button,1342373963
Control6=IDC_BUTTON_LIST,button,1342242891
Control7=IDC_BUTTON_DETAILS,button,1342242891
Control8=IDC_RADIO_ALL,button,1342308361
Control9=IDC_RADIO_SHEETCUR,button,1342177289
Control10=IDC_RADIO_SHEET,button,1342177289
Control11=IDC_EDIT_SHEET,edit,1350631552

[CLS:CDialogNetList]
Type=0
HeaderFile=DialogNetList.h
ImplementationFile=DialogNetList.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogNetList
VirtualFilter=dWC

[CLS:CButtonStates]
Type=0
HeaderFile=ButtonStates.h
ImplementationFile=ButtonStates.cpp
BaseClass=CButton
Filter=W
LastObject=CButtonStates
VirtualFilter=BWC

[MNU:IDR_EDITDROPDOWN (Spanish (Castilian))]
Type=1
Class=?
Command1=ID_0_1
CommandCount=1

[CLS:CPreviewFrame]
Type=0
HeaderFile=PreviewFrame.h
ImplementationFile=PreviewFrame.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CPreviewFrame

[CLS:CListCtrlCR]
Type=0
HeaderFile=ListCtrlCR.h
ImplementationFile=ListCtrlCR.cpp
BaseClass=CListCtrl
Filter=W
LastObject=CListCtrlCR

[DLG:IDD_SAVEASDB]
Type=1
Class=CDialogSaveasDB
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_TREE1,SysTreeView32,1350631458
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO1,combobox,1344340226

[CLS:CDialogSaveasDB]
Type=0
HeaderFile=DialogSaveasDB.h
ImplementationFile=DialogSaveasDB.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogSaveasDB
VirtualFilter=dWC

[DLG:IDD_PARTPROP]
Type=1
Class=CDialogPartProp
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ADODC1,{67397AA3-7FB1-11D0-B148-00A0C922E820},1073741824
Control4=IDC_DATAGRID1,{CDE57A43-8B86-11D0-B3C6-00A0C90AEA82},1342242816

[CLS:CDialogPartProp]
Type=0
HeaderFile=DialogPartProp.h
ImplementationFile=DialogPartProp.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogPartProp
VirtualFilter=dWC

[ACL:IDR_CABLEDIT]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_CANCEL
Command9=ID_SUP
Command10=ID_EDIT_CUT
Command11=ID_ESCAPE
Command12=ID_HELP
Command13=ID_CONTEXT_HELP
Command14=ID_NEXT_PANE
Command15=ID_PREV_PANE
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_OK
Command19=ID_NEXT_FIELD
Command20=ID_EDIT_CUT
Command21=ID_EDIT_UNDO
CommandCount=21

[ACL:IDR_MAINFRAME1]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_ROTATE
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_CANCEL
Command10=ID_SUP
Command11=ID_EDIT_CUT
Command12=ID_ESCAPE
Command13=ID_HELP
Command14=ID_CONTEXT_HELP
Command15=ID_NEXT_PANE
Command16=ID_PREV_PANE
Command17=ID_EDIT_COPY
Command18=ID_EDIT_PASTE
Command19=ID_OK
Command20=ID_NEXT_FIELD
Command21=ID_EDIT_CUT
Command22=ID_EDIT_UNDO
CommandCount=22

[DLG:IDD_GRAPH_DEN]
Type=1
Class=CDialogGraphDen
ControlCount=0

[CLS:CDialogGraphDen]
Type=0
HeaderFile=DialogGraphDen.h
ImplementationFile=DialogGraphDen.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogGraphDen
VirtualFilter=dWC

[CLS:CGraphDView]
Type=0
HeaderFile=GraphDView.h
ImplementationFile=GraphDView.cpp
BaseClass=CView
Filter=C
LastObject=CGraphDView

