
#include "stdafx.h"
#include "ShArray.h"
#include "DraftDrawDoc.h"

bool operator<(CPoint pt1,CPoint pt2){

	if(pt1.y<pt2.y){
		return true;
	}
	else if(pt1.y==pt2.y){
		if(pt1.x<pt2.x){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShArray
CShArray::CShArray(){

	m_pddDoc=NULL;
}

CShArray::CShArray(SIZE szTotal,SIZE szCell,LPVOID lpddDoc /*=NULL*/){

	m_pddDoc=NULL;
	Create(szTotal,szCell,lpddDoc);
}

BOOL CShArray::Create(SIZE szTotal,SIZE szCell,LPVOID lpddDoc /*=NULL*/){

	float x_total=szTotal.cx;
	float x_cell=szCell.cx;
	float y_total=szTotal.cy;
	float y_cell=szCell.cy;

	m_szArray=CSize(ceil(x_total/x_cell),ceil(y_total/y_cell));
	m_szCell=szCell;

	m_pddDoc=lpddDoc;

	m_vvRows.SetSize(m_szArray.cy);

	for(int j=0;j<m_szArray.cy;j++){

		m_vvRows[j].SetSize(m_szArray.cx);
	}

	return TRUE;
}

CShArray::~CShArray(){

}

/////////////////////////////////////////////////////////////////////////////
// CShArray implementation
SIZE CShArray::GetSize( ) const{

	return m_szArray;
}

void CShArray::AddShape(CShape *pSh){	//Add a shape to array

	int origen_x=pSh->m_Rect.TopLeft().x/m_szCell.cx;
	int origen_y=pSh->m_Rect.TopLeft().y/m_szCell.cy;
	int limit_x=pSh->m_Rect.BottomRight().x/m_szCell.cx;
	int limit_y=pSh->m_Rect.BottomRight().y/m_szCell.cy;

	for(int j=origen_y;j<=limit_y;j++){
	for(int i=origen_x;i<=limit_x;i++){

		m_vvRows[j][i].AddTail(pSh);
	}}
}

void CShArray::MoveShape(CShape *pSh,LPRECT lpRecLast,BOOL bReconnect /*=TRUE*/){			//Move shape in array

	CRect rectInter;
	if(1){
	//Rectangles does not overlap.
	//if(pSh->m_Rect.IntersectRect(pSh->m_Rect,lpRecLast)){

		int origen_x=CRect(lpRecLast).TopLeft().x/m_szCell.cx;
		int origen_y=CRect(lpRecLast).TopLeft().y/m_szCell.cy;
		int limit_x=CRect(lpRecLast).BottomRight().x/m_szCell.cx;
		int limit_y=CRect(lpRecLast).BottomRight().y/m_szCell.cy;

		int j;
		for(j=origen_y;j<=limit_y;j++){
		for(int i=origen_x;i<=limit_x;i++){

			POSITION pos=m_vvRows[j][i].Find(pSh);
			if(pos){

				m_vvRows[j][i].RemoveAt(pos);
				int nCount=m_vvRows[j][i].GetCount();
			}
		}}

		origen_x=pSh->m_Rect.TopLeft().x/m_szCell.cx;
		origen_y=pSh->m_Rect.TopLeft().y/m_szCell.cy;
		limit_x=pSh->m_Rect.BottomRight().x/m_szCell.cx;
		limit_y=pSh->m_Rect.BottomRight().y/m_szCell.cy;

		for(j=origen_y;j<=limit_y;j++){
		for(int i=origen_x;i<=limit_x;i++){

			m_vvRows[j][i].AddTail(pSh);
		}}
	}
	//Rectangles overlap
	else{

		CRect rectUnion;
		rectUnion.UnionRect(pSh->m_Rect,lpRecLast);
		CRect rectUpper,rectLower;

		if(pSh->m_Rect.TopLeft().y>CRect(lpRecLast).TopLeft().y){

		}
		else{

		}

		if(pSh->m_Rect.TopLeft().y>CRect(lpRecLast).TopLeft().y){

			int origen_x=CRect(lpRecLast).TopLeft().x/m_szCell.cx;
			int origen_y=CRect(lpRecLast).TopLeft().y/m_szCell.cy;
			int limit_x=CRect(lpRecLast).TopLeft().x/m_szCell.cx;
			int limit_y=rectInter.TopLeft().y/m_szCell.cy;

			for(int j=origen_y;j<=limit_y;j++){
			for(int i=origen_x;i<=limit_x;i++){

				m_vvRows[j][i].AddTail(pSh);
			}}
		}
		else{


		}
	}

	//Try to reconnect shapes
//#define DCABLE_SHARRAY_RECONNECT
#ifdef DCABLE_SHARRAY_RECONNECT
	if(bReconnect){
		if(pSh->IsKindOf(RUNTIME_CLASS(CShapeUnit))){

			CShapeUnit *pShUnit=(CShapeUnit *)pSh;

			//1.Prepare a map of points pointer to lists of objects.
			std::map<CPoint,CObject*> map;
			std::map<CPoint,int> map_index;

			//2. Iterate pins in shape
			for(int i=0;i<pShUnit->m_obarrShapearr.GetSize();i++){
				CShape *pSh=(CShape *)pShUnit->m_obarrShapearr[i];
				if(pSh->IsKindOf(RUNTIME_CLASS(CShapePin))){
					CShapePin *pShPin=(CShapePin *)pSh;
					CPoint pt=pShPin->m_rectPin.CenterPoint()+pShPin->m_Rect.TopLeft();
					pt=CPoint(pt.x/m_szCell.cx,pt.y/m_szCell.cy);
					std::map<CPoint,CObject*>::iterator it=map.find( pt);
					if(it==map.end()){
						CList<CObject*,CObject*> *cells=new CList<CObject*,CObject*>;
						map[pt]=cells;
						map_index[pt]=i;
						it=map.find(pt);
					}
					CList<CObject*,CObject*> *cells=(CList<CObject*,CObject*> *)it->second;
					if(!cells->Find(pShPin)){
						cells->AddTail(pShPin);
					}
				}
			}

			//3. Try to connect pins
			for(std::map<CPoint,CObject*>::iterator it=map.begin();it!=map.end();++it){
				CList<CObject*,CObject*> *cells=(CList<CObject*,CObject*> *)it->second;
				for(POSITION q=cells->GetHeadPosition();cells->GetNext(q);q!=cells->GetTailPosition( )){
					int i=it->first.x;
					int j=it->first.y;
					for(POSITION p=m_vvRows[i][j].GetHeadPosition();m_vvRows[i][j].GetNext(p);p!=m_vvRows[i][j].GetTailPosition( )){
						if(pSh->IsKindOf(RUNTIME_CLASS(CShapeWire))){
							CShape *pShconn=NULL;
							CShape *pShchild=NULL;
							CShapePin *pShPin=(CShapePin *)cells->GetAt(q);
							//check temp conection
							CPoint pt_conn=pShPin->m_rectPin.CenterPoint()+pShPin->GetParent()->m_Rect.TopLeft();
							//19/01/2005: new connecting mechanism
							BOOL bConnectionTmp=FALSE;//set when a connection is pending
							CShapeContainer *pShContConnect=NULL;
							bConnectionTmp=((CShapeContainer*)pSh)->PtInRect(&pt_conn,&pShContConnect);
							//Previous mechanism to 19/01/2005 is commented
							/*
							int mode_prev=pSh->m_Mode;
							pSh->OnLButtonDown(0,pt_conn);
							pSh->m_Mode=mode_prev;
							*/

							//19/01/2005: new connecting mechanism
							if(bConnectionTmp){

								CShapeContainer *pShCont1=(CShapeContainer *)pShUnit;
								CShape *pSh=(*pShCont1)[0];
								if(pSh->IsKindOf(RUNTIME_CLASS(CShapeContainer))){

									CShapeContainer *pShCont2=(CShapeContainer *)pSh;
									pShCont1->LinkShapes(pShContConnect,pShCont2);
								}

							}

							//Previous mechanism to 19/01/2005 is commented
							/*
							//After lbutton click detination container contains information about destination pin
							if(pSh->m_pshChildConn){

								pShconn=pSh;//pointer to destination container
								pShchild=pSh->m_pshChildConn;//pointer to pin of destination container
							}
							
							//Connect
							if(pShconn){
								//To connect put in source container information about destination pin (container,pin)
								//Call SetConnection(...) passing source index.
								pShUnit->m_pshConn=pShconn;
								pShUnit->m_pshChildConn=pShchild;
								int nIndex=map_index[it->first];
								pShchild=pShUnit->SetConnection(NULL,nIndex);

								//Call SetConnection(...) for destination container.
								//Destination container matches pin using destination->m_pshChildConn that was unaltered
								//since lbutton click.
								//Destination put information in pin using supplied parameters.
								pShconn->m_pshConn=pShUnit;
								pShconn->SetConnection(pShchild);
							}
							*/
						}
					}
				}
			}

			//4.Clean up of map
			for(it=map.begin();it!=map.end();++it){
				delete(it->second);
			}
		}//end of if is kind of CShapeUnit
	}//end of flag reconnect
#endif
}

void CShArray::DeleteShape(CShape *pSh){			//Delete shape

	int origen_x = max(pSh->m_Rect.TopLeft().x / m_szCell.cx, 0);
	int origen_y = max(pSh->m_Rect.TopLeft().y / m_szCell.cy, 0);
	int limit_x = pSh->m_Rect.BottomRight().x / m_szCell.cx;
	int limit_y = pSh->m_Rect.BottomRight().y / m_szCell.cy;

	for(int j=origen_y;j<=min(limit_y,m_szArray.cy-1);j++){
	for(int i=origen_x;i<=min(limit_x,m_szArray.cx-1);i++){

		POSITION pos=m_vvRows[j][i].Find(pSh);
		if(pos){

			m_vvRows[j][i].RemoveAt(pos);
		}
	}}
}

//Removes all the pointers from this array but does not actually delete the objects
void CShArray::RemoveAll(){

	int origen_x=0;
	int origen_y=0;
	int limit_x=m_szArray.cx;
	int limit_y=m_szArray.cy;

	for(int j=origen_y;j<limit_y;j++){
	for(int i=origen_x;i<limit_x;i++){

		m_vvRows[j][i].RemoveAll();

	}}
}

//Establishes the size of an empty or existing array
void CShArray::SetSize( SIZE szTotal,SIZE szCell, SIZE szGrowBy /*= CSize(-1,-1)*/){

	//Calcule new size
	float x_total=szTotal.cx;
	float x_cell=szCell.cx;
	float y_total=szTotal.cy;
	float y_cell=szCell.cy;

	SIZE szNewSize=CSize(ceil(x_total/x_cell),ceil(y_total/y_cell));

	//Get actual array size
	SIZE szArray=GetSize();

	//Vertical frame
	if(szArray.cx > szNewSize.cx){

		int limit_y=min(szArray.cy,szNewSize.cy);

		for(int j=0;j<limit_y;j++){
			for(int i=szNewSize.cx;i<szArray.cx;i++){

				RemoveAt(CSize(i,j));
			}
		}
	}

	//Horizontal frame
	if(szArray.cy > szNewSize.cy){

		int limit_x=max(szArray.cx,szNewSize.cx);

		for(int j=szNewSize.cy;j<szArray.cy;j++){
			for(int i=0;i<limit_x;i++){

				RemoveAt(CSize(i,j));
			}
		}
	}

	//Set size
	m_szArray=szNewSize;
	m_szCell=szCell;

	m_vvRows.SetSize(m_szArray.cy);

	for(int j=0;j<m_szArray.cy;j++){

		m_vvRows[j].SetSize(m_szArray.cx);
	}
}

//Removes one or more elements starting at a specified index
void CShArray::RemoveAt( SIZE szIndex, SIZE szCount /*= CSize(1,1)*/ ){

	for(int j=szIndex.cy;j<szIndex.cy+szCount.cy;j++){
		for(int i=szIndex.cx;i<szIndex.cx+szCount.cx;i++){
			if(m_pddDoc){
				POSITION pos;
				CDraftDrawDoc *pddDoc=(CDraftDrawDoc *)m_pddDoc;
				CObList *pObLst = &m_vvRows[i][j];
				TRY
				{
					if (pObLst!=NULL) for (pos = m_vvRows[i][j].GetHeadPosition(); pos != NULL;){
						CObject *pobj=m_vvRows[i][j].GetNext( pos ) ;
						CShape *psh=(CShape *)pobj;
						pddDoc->DeleteShape(psh->m_uiShapeId,FALSE);//Do not use brute force option
					}
				}
				CATCH_ALL(e)
				{

				}
				END_CATCH_ALL
			}

			m_vvRows[i][j].RemoveAll();
	}}
}