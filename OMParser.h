#if !defined(__DRAFTCABLE_OM_PARSER)
#define __DRAFTCABLE_OM_PARSER

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OMParser.h : header file
//

//Only add members to this enumeration,
//becose serialization of previus version will not behave properly (undo/redo commands)
//after deserializarion.
enum{ddcShape,ddcShapeEllipse,ddcShapeRectangle,ddcShapeLine,
ddcShapePargram,ddcShapeTrapezium,ddcShapeRhomb,
ddcShapeTriangle,ddcShapePentagon,ddcShapeHexagon,ddcShapeOctogon,
ddcShapeCross,ddcShapeNGear,ddcShapeNStar,ddcShapeNPolygon,
ddcShapeT,ddcShapeL,ddcShapeStraight,ddcShapeNCross,
ddcShapeUnit,ddcShapeWire,ddcShapeBus,
ddcShapePolyline,ddcShapeCurve,
ddcShapeLabel,ddcShapePicture,ddcShapeArc,
ddcShapePolyarc
};

//Constants
enum{ddcFalse=0,ddcTrue};
enum{ddcScaleFromTopLeft=0,ddcScaleFromMiddle,ddcScaleFromBottomRight};

//The number after Object stands for level:
//1 1st level objects just under Application Object
//2 2nd level objects under 1st level objects
//...
enum{ddcObjectNull=-1,ddcParam,ddcBlockBegin=1,ddcBlockEnd,ddcVar,ddcNew,
ddcObject1ActiveSheet,ddcObject1ActiveDocument,
ddcObject4Shapes,ddcObject4Selection,
ddcObject5AddShape, ddcObject5SelectShape, ddcObject5DeleteShape, ddcObject5ShapeRange, ddcObject5CountShapes,
ddcObject6IncrementLeft,ddcObject6IncrementTop,ddcObject6ScaleWidth,ddcObject6ScaleHeight};

//Structures
typedef struct omobjecttag{
	int iObject;
	TCHAR sLabel[MAX_PATH];
	int iLevel;
	LPVOID lpParam;
}omobject;

#define DDC_OM_ADDSHAPE			"ActiveSheet.Shapes.AddShape(%s,%i,%i,%i,%i,%i).Select"//shape type,left,top,right,bottom,id
//This macro implies adding to the project tree a node. In future this functionality should be implemeted
//with a kind of event listener that adds the node to the tree when a new component is added.
#define DDC_OM_DELETESHAPE		"Select.Delete"

//Internal macros used by core
#define DDC_OM_ADDPTREENODE		"ActiveDocument.InternalObjects.ProjectTree.AddNode(%s,%i,%i).Select"//name,id,icon

//Parse flags
#define OMPF_JUSTSINTACTICAL	0x00000001

//Commands deque
class cmddeque{

public:
	//Two deque's are necessary:
	//1.When user hits redo button m_dqCommands is advanced (back to front).
	//2.When user hits undo button m_dqCommandsUndo is retroceded (front to back).
	//Deque to store visual basic commands
	std::deque<std::string> m_dqCommands;
	//Deque to store visual basic undo commands
	std::deque<std::string> m_dqCommandsUndo;
	//index to both queues
	int m_nIndexCommQue;
	//direction flag
	bool m_bBackwards;
	//queue maximum size
	int m_nSize;

	//construction
	cmddeque(int nSize = 32);
	//operations
	void push_cmd(std::string strComm,std::string strCommUndo,bool bModified =false);
	void clear();
	void check_size();
	bool is_modified();
	void reset_modifiedflag();//set to unmodified
protected:
	//This important variable represents the point where document has been modified.
	//The '0' im the name stands for 'reference point'.
	int m_nIndexModified0;
};

class OMParser{

public:
	//Return codes (ms-dos convention):
	//0	OK
	//1 Error
	OMParser ();
	int OMParse(std::string sCommand, int flags = 0, TCHAR* output = NULL, int count = 0);

//Implementation
protected:
	int m_nBlockDeep;
	std::map<std::string,omobject*> m_sblTable;
	BOOL IdFromName(std::string sName,omobject *lpOb);
};


/////////////////////////////////////////////////////////////////////////////

#endif // !define __DRAFTCABLE_OM_PARSER)
