// 这是主 DLL 文件。

#include "stdafx.h"
#include "svGuiDisplay.h"
#include "svGuiCrossVar.h"

svGuiCrossVar::svGuiCrossVar()
{
	m_vGuiLineSegVer	= NULL;
	m_vGuiLineSegHor	= NULL;
	m_vGuiLineSegVerEx	= NULL;
	m_vGuiLineSegHorEx	= NULL;
	m_vGuiCoorCross		= NULL;
	m_pGuiTextShow		= NULL;
}

svGuiCrossVar::~svGuiCrossVar()
{
	if (m_vGuiLineSegVer!=NULL)
	{	
		delete m_vGuiLineSegVer;
		m_vGuiLineSegVer = NULL;
	}
	if (m_vGuiLineSegHor!=NULL)
	{
		delete m_vGuiLineSegHor;
		m_vGuiLineSegHor = NULL;
	}
	if (m_vGuiLineSegVerEx!=NULL)
	{
		delete m_vGuiLineSegVerEx;
		m_vGuiLineSegVerEx = NULL;
	}
	if (m_vGuiLineSegHorEx)
	{
		delete m_vGuiLineSegHorEx;
		m_vGuiLineSegHorEx = NULL;
	}
	if (m_vGuiCoorCross!=NULL)
	{
		delete m_vGuiCoorCross;
		m_vGuiCoorCross = NULL;
	}
	if (m_pGuiTextShow !=NULL)
	{
		delete m_pGuiTextShow->GetTextFont();
		delete m_pGuiTextShow;
		m_pGuiTextShow = NULL;
	}
}

//传递图像的指针显示
bool svGuiCrossVar::SetDisplay(scGuiDisplay *pGuiDisplay)
{
	m_vpGuiDisplay = pGuiDisplay;
	return true;
}

//设置文本，按照系统的字体大小显示
void svGuiCrossVar::SetText(double dX, double dY,double dRotation,LPCTSTR lpszText)
{
	m_vpGuiInteractiveContainer = m_vpGuiDisplay->GetInteractiveContainer();
	scGuiText* scText,pGuiText;
	scText = new scGuiText;
	scText->SetLabel("Text");
	scText->SetLabelVisible(FALSE);
	scText->SetLineColor(RGB(255, 0, 0));
	scText->SetXYRotationText(dX,dY,dRotation,lpszText);
	scText->SetVisible(TRUE);

	m_vpGuiInteractiveContainer->AddItem(scText,"Text");
}

//设置固定文字显示,可以修改字体大小及格式显示
void svGuiCrossVar::SetFixText(LPCTSTR lpszText)
{
	m_pGuiTextShow = new scGuiText;
	m_pGuiTextShow->SetXYRotation(sc2Vector(10,10),scRadian(0));
	m_pGuiTextShow->SetLineColor(RGB(255,0,0));
	m_pGuiTextShow->SetVisible(TRUE);
	LOGFONT* lf = new LOGFONT;
	lf->lfHeight         = 16 ;
	lf->lfWidth          = 16 ;
	lf->lfEscapement     = 0 ;
	lf->lfOrientation      = 0 ;
	lf->lfWeight         =  FW_SEMIBOLD;
	lf->lfItalic           = 0 ;
	lf->lfUnderline       = 0 ;
	lf->lfStrikeOut       = 0 ;
	lf->lfCharSet        = DEFAULT_CHARSET ;
	lf->lfOutPrecision    = 0 ;
	lf->lfClipPrecision    = 0 ;
	lf->lfQuality         = 0 ;
	lf->lfPitchAndFamily  = 0 ;
	lstrcpy(lf->lfFaceName, _T("仿宋"));
	m_pGuiTextShow->SetTextFont(lf);
	m_pGuiTextShow->SetText(lpszText);
}

//设置十字靶标的中心位置，大小
void svGuiCrossVar::SetCenterXYWidthHeight(double dCenterX, double dCenterY, double dWidth /* = 16 */, double dHeight /* = 16 */)
{
	CString strInfo;
	m_vCenter   = sc2Vector(dCenterX,dCenterY);
	m_vTop		= sc2Vector(dCenterX,dCenterY - dHeight);
	m_vBottom	= sc2Vector(dCenterX,dCenterY + dHeight);
	m_vLeft		= sc2Vector(dCenterX - dWidth,dCenterY);
	m_vRight	= sc2Vector(dCenterX + dWidth,dCenterY);
	m_bVisionVerExLine = FALSE;
	m_bVisionHorExLine = FALSE;

	m_vpGuiInteractiveContainer = m_vpGuiDisplay->GetInteractiveContainer();

	//设置竖直线
	m_vGuiLineSegVer = new scGuiLineSeg;
	m_vGuiLineSegVer->SetLineColor(RGB(0, 255, 0));
	m_vGuiLineSegVer->SetStartXYEndXY(m_vTop,m_vBottom);
	m_vGuiLineSegVer->SetLineSegArrowVisible(FALSE);
	m_vGuiLineSegVer->EnableGuiDOF(scGuiLineSeg::eGuiDOFAll);
	m_vGuiLineSegVer->SetMultiSelectable(TRUE);
	m_vGuiLineSegVer->SetLabel("LineSegVer");
	m_vGuiLineSegVer->SetLabelVisible(FALSE);
	m_vGuiLineSegVer->SetVisible(TRUE);

	m_vpGuiInteractiveContainer->AddItem(m_vGuiLineSegVer,"LineSegVer");

	//设置水平线
	m_vGuiLineSegHor = new scGuiLineSeg;
	m_vGuiLineSegHor->SetLineColor(RGB(0, 255, 0));
	m_vGuiLineSegHor->SetStartXYEndXY(m_vLeft,m_vRight);
	m_vGuiLineSegHor->SetLineSegArrowVisible(FALSE);
	m_vGuiLineSegHor->EnableGuiDOF(scGuiLineSeg::eGuiDOFAll);
	m_vGuiLineSegHor->SetMultiSelectable(TRUE);
	m_vGuiLineSegHor->SetLabel("LineSegHor");
	m_vGuiLineSegHor->SetLabelVisible(FALSE);
	m_vGuiLineSegHor->SetVisible(TRUE);
	

	m_vpGuiInteractiveContainer->AddItem(m_vGuiLineSegHor,"LineSegHor");

	//设置十字交点
	m_vGuiCoorCross = new scGuiCoordCross;
	m_vGuiCoorCross->SetLineColor(RGB(255, 0, 0));
	m_vGuiCoorCross->SetCenterRotationLengths(m_vCenter,scRadian(0),20,20);
	m_vGuiCoorCross->EnableGuiDOF(scGuiCoordCross::eGuiDOFPosition);
	m_vGuiCoorCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
	m_vGuiCoorCross->SetXLenEnable(true);
	m_vGuiCoorCross->SetYLenEnable(true);
	m_vGuiCoorCross->SetMultiSelected(TRUE);
	m_vGuiCoorCross->SetLabel("GuiCoordCross");
	m_vGuiCoorCross->SetLabelVisible(FALSE);
	m_vGuiCoorCross->SetVisible(TRUE);
	m_vGuiCoorCross->SetSelected(TRUE);

	m_vpGuiInteractiveContainer->AddItem(m_vGuiCoorCross,"GuiCoordCross");

	//设置竖直线的延长线
	m_vGuiLineSegVerEx = new scGuiLineSeg;
	m_vGuiLineSegVerEx->SetLineColor(RGB(0, 255, 0));
	m_vGuiLineSegVerEx->SetSelectedLineStyle(scGuiGraphic::dashLine);
	m_vGuiLineSegVerEx->SetLineSegArrowVisible(FALSE);
	m_vGuiLineSegVerEx->EnableGuiDOF(scGuiLineSeg::eGuiDOFNone);
	m_vGuiLineSegVerEx->SetMultiSelectable(TRUE);
	m_vGuiLineSegVerEx->SetLabel("LineSegVerEx");
	m_vGuiLineSegVerEx->SetLabelVisible(FALSE);
	m_vGuiLineSegVerEx->SetVisible(FALSE);

	m_vpGuiInteractiveContainer->AddItem(m_vGuiLineSegVerEx,"GuiLineSegVerEx");

	//设置水平线的延长线
	m_vGuiLineSegHorEx = new scGuiLineSeg;
	m_vGuiLineSegHorEx->SetLineColor(RGB(0, 255, 0));
	m_vGuiLineSegHorEx->SetSelectedLineStyle(scGuiGraphic::dashLine);
	m_vGuiLineSegHorEx->SetLineSegArrowVisible(FALSE);
	m_vGuiLineSegHorEx->EnableGuiDOF(scGuiLineSeg::eGuiDOFNone);
	m_vGuiLineSegHorEx->SetMultiSelectable(TRUE);
	m_vGuiLineSegHorEx->SetLabel("GuiLineSegHorEx");
	m_vGuiLineSegHorEx->SetLabelVisible(FALSE);
	m_vGuiLineSegHorEx->SetVisible(FALSE);

	m_vpGuiInteractiveContainer->AddItem(m_vGuiLineSegHorEx,"GuiLineSegHorEx");

	m_vpGuiDisplay->SetInteractiveContainer(m_vpGuiInteractiveContainer);
	m_vpGuiDisplay->MyInvalidate();
}

//将十字转换成两条直线的交点
void svGuiCrossVar::GuiCrossToLineseg()
{
	m_vLineSegVer = m_vGuiLineSegVer->GetLineSeg();
	m_vLineSegHor = m_vGuiLineSegHor->GetLineSeg();
	m_vCenter = m_vGuiCoorCross->GetCenter();
	double Width,Height,Angel;

	scDegree degLineAngle;
	degLineAngle = scDegree(m_vGuiCoorCross->GetRotation()).SignedNorm();

	if ((degLineAngle.ToDouble() <= 0 && degLineAngle.ToDouble() > -90)||(degLineAngle.ToDouble() < 180 && degLineAngle.ToDouble() > 90))
	{
		Width	= m_vGuiCoorCross->GetXLength()/2;
		Height	= m_vGuiCoorCross->GetYLength()/2;
		Angel	= scRadian(m_vGuiCoorCross->GetRotation().NormMod180()).ToDouble();
		CrossMoveSet(m_vCenter,Width,Height,Angel);
	}
	else
	{
		Width	= m_vGuiCoorCross->GetXLength()/2;
		Height	= m_vGuiCoorCross->GetYLength()/2;
		Angel	= scRadian(m_vGuiCoorCross->GetRotation().NormMod180()).ToDouble();
		CrossMoveSet90(m_vCenter,Width,Height,Angel);
	}

	m_vGuiLineSegVer->SetStartXYEndXY(m_vTop,m_vBottom);
	m_vGuiLineSegHor->SetStartXYEndXY(m_vLeft,m_vRight);
	m_vGuiLineSegVer->SetSelected(TRUE);
	m_vGuiLineSegHor->SetSelected(TRUE);
	m_vGuiLineSegVer->SetVisible(TRUE);
	m_vGuiLineSegHor->SetVisible(TRUE);
	m_vGuiCoorCross->SetVisible(FALSE);
}

//根据十字的拖动，完成直线的拖动
void svGuiCrossVar::Dragging()
{
	if (m_vGuiLineSegVer->IsSelected()||m_vGuiLineSegHor->IsSelected())
	{
		if(GetLineCross())
		{
			m_vGuiCoorCross->SetCenterRotationLengths(m_vCenter,scRadian(0),20,20);
			m_vGuiCoorCross->EnableGuiDOF(scGuiCoordCross::eGuiDOFPosition);
			m_vGuiCoorCross->SetXDirectionAdornment(scGuiCoordCross::eNone);
			m_vGuiCoorCross->SetXLenEnable(false);
			m_vGuiCoorCross->SetYLenEnable(false);
			m_vGuiCoorCross->SetLineColor(RGB(255, 0, 0));
			m_vGuiCoorCross->SetVisible(TRUE);

			m_vGuiLineSegVer->SetSelected(TRUE);
			m_vGuiLineSegHor->SetSelected(TRUE);

			m_vGuiLineSegVerEx->SetSelected(TRUE);
			m_vGuiLineSegVerEx->SetVisible(m_bVisionVerExLine);
			m_vGuiLineSegHorEx->SetSelected(TRUE);
			m_vGuiLineSegHorEx->SetVisible(m_bVisionHorExLine);
		}	
	}
	if (m_vGuiCoorCross->IsSelected())
	{
		sc2Vector svCenterCross = m_vGuiCoorCross->GetCenter();
		double dX = 0,dY = 0;
		if (svCenterCross.GetX() != m_vCenter.GetX() || svCenterCross.GetY() != m_vCenter.GetY())
		{
			dX = svCenterCross.GetX() - m_vCenter.GetX();
			dY = svCenterCross.GetY() - m_vCenter.GetY();

			m_vGuiLineSegVer->GetStartXYEndXY(m_vTop, m_vBottom);
			m_vGuiLineSegHor->GetStartXYEndXY(m_vLeft, m_vRight);

			m_vTop		= sc2Vector(m_vTop.GetX()+dX, m_vTop.GetY()+dY);
			m_vBottom	= sc2Vector(m_vBottom.GetX()+dX, m_vBottom.GetY()+dY);
			m_vLeft		= sc2Vector(m_vLeft.GetX()+dX, m_vLeft.GetY()+dY);
			m_vRight	= sc2Vector(m_vRight.GetX()+dX, m_vRight.GetY()+dY);
			m_vCenter	= m_vGuiCoorCross->GetCenter();

			m_vGuiLineSegVer->SetStartXYEndXY(m_vTop,m_vBottom);
			m_vGuiLineSegHor->SetStartXYEndXY(m_vLeft,m_vRight);
			m_vLineSegVer = m_vGuiLineSegVer->GetLineSeg();
			m_vLineSegHor = m_vGuiLineSegHor->GetLineSeg();

			sc2Vector VerStart = GetLinesegContinePos(m_vCenter,m_vLineSegVer);
			sc2Vector HorStart = GetLinesegContinePos(m_vCenter,m_vLineSegHor);

			m_vGuiLineSegVerEx->SetStartXYEndXY(VerStart, m_vCenter);
			m_vGuiLineSegHorEx->SetStartXYEndXY(HorStart, m_vCenter);
		}
		m_vGuiLineSegVer->SetSelected(TRUE);
		m_vGuiLineSegHor->SetSelected(TRUE);

		m_vGuiLineSegVerEx->SetSelected(TRUE);
		m_vGuiLineSegHorEx->SetSelected(TRUE);
	}
}

//得到直线的交点
bool svGuiCrossVar::GetLineCross()
{
	bool bVerHorInter = false,bVerHorExInter = false,bVerExHorInter = false,bVerExHorExInter = false;
	std::vector<sc2Vector> VerHorPointArray,VerHorExPointArray,VerExHorPointArray,VerExHorExPointArray; 
	sc2Vector VerStart,HorStart;

	m_vLineSegVer = m_vGuiLineSegVer->GetLineSeg();
	m_vLineSegHor = m_vGuiLineSegHor->GetLineSeg();

	//求两条线段是否有交点
	sfInterSectSegmentSegment(m_vLineSegVer,m_vLineSegHor,bVerHorInter,VerHorPointArray);
	if (!bVerHorInter)
	{
		//竖直直线段和水平线段延长线是否交点
		sfInterSectSegmentLine(m_vLineSegVer,m_vLineSegHor.GetLine(),bVerHorExInter,VerHorExPointArray);
		if (!bVerHorExInter)
		{
			//竖直直线段延长线和水平线段是否交点
			sfInterSectSegmentLine(m_vLineSegHor,m_vLineSegVer.GetLine(),bVerExHorInter,VerExHorPointArray);
			if (!bVerExHorInter)
			{
				//两条线延长线右交点
				sfInterSectLineLine(m_vLineSegVer.GetLine(),m_vLineSegHor.GetLine(),bVerExHorExInter,VerExHorExPointArray);
				if (!bVerExHorExInter)
				{
					return false;
				}
				m_vCenter.SetX(VerExHorExPointArray.at(0).GetX());
				m_vCenter.SetY(VerExHorExPointArray.at(0).GetY());

				VerStart = GetLinesegContinePos(m_vCenter,m_vLineSegVer);
				m_vGuiLineSegVerEx->SetStartXYEndXY(VerStart,m_vCenter);
				m_vGuiLineSegVerEx->SetVisible(TRUE);

				HorStart = GetLinesegContinePos(m_vCenter,m_vLineSegHor);
				m_vGuiLineSegHorEx->SetStartXYEndXY(HorStart,m_vCenter);
				m_vGuiLineSegHorEx->SetVisible(TRUE);
				m_bVisionVerExLine = TRUE;
				m_bVisionHorExLine = TRUE;
				return bVerExHorExInter;
			}
			m_vCenter.SetX(VerExHorPointArray.at(0).GetX());
			m_vCenter.SetY(VerExHorPointArray.at(0).GetY());

			VerStart = GetLinesegContinePos(m_vCenter,m_vLineSegVer);
			m_vGuiLineSegVerEx->SetStartXYEndXY(VerStart,m_vCenter);
			m_vGuiLineSegVerEx->SetVisible(TRUE);
			m_bVisionHorExLine = FALSE;
			m_bVisionVerExLine = TRUE;
			return bVerExHorInter;
		}
		m_vCenter.SetX(VerHorExPointArray.at(0).GetX());
		m_vCenter.SetY(VerHorExPointArray.at(0).GetY());

		HorStart = GetLinesegContinePos(m_vCenter,m_vLineSegHor);
		m_vGuiLineSegHorEx->SetStartXYEndXY(HorStart,m_vCenter);
		m_vGuiLineSegHorEx->SetVisible(TRUE);
		m_bVisionVerExLine = FALSE;
		m_bVisionHorExLine = TRUE;
		return bVerHorExInter;
	}
	m_vCenter.SetX(VerHorPointArray.at(0).GetX());
	m_vCenter.SetY(VerHorPointArray.at(0).GetY());
	m_bVisionVerExLine = FALSE;
	m_bVisionHorExLine = FALSE;
	return bVerHorInter;
}

//判断直线的延长线端点在直线上还是直线外
sc2Vector svGuiCrossVar::GetLinesegContinePos(sc2Vector scPoint,scLineSeg scCheckLine)
{
	sc2Vector P1,P2;
	P1 = scCheckLine.GetP1();
	P2 = scCheckLine.GetP2();
	double dP1Length = 0,dP2Length = 0;
	dP1Length = pow(P1.GetX()-scPoint.GetX(),2) + pow(P1.GetY()-scPoint.GetY(),2);
	dP2Length = pow(P2.GetX()-scPoint.GetX(),2) + pow(P2.GetY()-scPoint.GetY(),2);
	if (dP1Length - dP2Length > 0)
	{
		return P2;
	}
	else
		return P1;
}

//十字转换直线前没有完成旋转
void svGuiCrossVar::CrossMoveSet(sc2Vector scCenter, double dWidth, double dHeight,double dAngel)
{
	m_vCenter   = scCenter;
	m_vTop		= sc2Vector(scCenter.GetX() - fabs(dHeight*sin(dAngel)),scCenter.GetY() - fabs(dHeight*cos(dAngel)));
	m_vBottom	= sc2Vector(scCenter.GetX() + fabs(dHeight*sin(dAngel)),scCenter.GetY() + fabs(dHeight*cos(dAngel)));
	m_vLeft		= sc2Vector(scCenter.GetX() - fabs(dWidth*cos(dAngel)),scCenter.GetY() + fabs(dWidth*sin(dAngel)));
	m_vRight	= sc2Vector(scCenter.GetX() + fabs(dWidth*cos(dAngel)),scCenter.GetY() - fabs(dWidth*sin(dAngel)));
}

//十字转换直线前没有旋转90度
void svGuiCrossVar::CrossMoveSet90(sc2Vector scCenter, double dWidth, double dHeight,double dAngel)
{
	m_vCenter   = scCenter;
	m_vTop		= sc2Vector(scCenter.GetX() - fabs(dHeight*sin(dAngel)),scCenter.GetY() + fabs(dHeight*cos(dAngel)));
	m_vBottom	= sc2Vector(scCenter.GetX() + fabs(dHeight*sin(dAngel)),scCenter.GetY() - fabs(dHeight*cos(dAngel)));
	m_vLeft		= sc2Vector(scCenter.GetX() - fabs(dWidth*cos(dAngel)),scCenter.GetY() - fabs(dWidth*sin(dAngel)));
	m_vRight	= sc2Vector(scCenter.GetX() + fabs(dWidth*cos(dAngel)),scCenter.GetY() + fabs(dWidth*sin(dAngel)));
}

//得到十字的中心点X、Y的值
void svGuiCrossVar::GetCenterXY(double& dCenterX,double& dCenterY) const
{
	sc2Vector scCenter;
	scCenter = m_vGuiCoorCross->GetCenter();
	dCenterX = scCenter.GetX();
	dCenterY = scCenter.GetY();
}

//得到十字中心点的坐标值
sc2Vector svGuiCrossVar::GetCenter() const
{
	sc2Vector scCenter;
	scCenter = m_vGuiCoorCross->GetCenter();
	return scCenter;
}

//设置十字中心点坐标值
void svGuiCrossVar::SetCrossCenter(const sc2Vector& vCenter)
{
	double dX = 0,dY = 0;
	m_vGuiCoorCross->SetCenter(vCenter);

	dX = vCenter.GetX() - m_vCenter.GetX();
	dY = vCenter.GetY() - m_vCenter.GetY();

	m_vGuiLineSegVer->GetStartXYEndXY(m_vTop, m_vBottom);
	m_vGuiLineSegHor->GetStartXYEndXY(m_vLeft, m_vRight);

	m_vTop		= sc2Vector(m_vTop.GetX()+dX, m_vTop.GetY()+dY);
	m_vBottom	= sc2Vector(m_vBottom.GetX()+dX, m_vBottom.GetY()+dY);
	m_vLeft		= sc2Vector(m_vLeft.GetX()+dX, m_vLeft.GetY()+dY);
	m_vRight	= sc2Vector(m_vRight.GetX()+dX, m_vRight.GetY()+dY);
	m_vCenter   = vCenter;

	m_vGuiLineSegVer->SetStartXYEndXY(m_vTop,m_vBottom);
	m_vGuiLineSegHor->SetStartXYEndXY(m_vLeft,m_vRight);
	m_vLineSegVer = m_vGuiLineSegVer->GetLineSeg();
	m_vLineSegHor = m_vGuiLineSegHor->GetLineSeg();

	sc2Vector VerStart = GetLinesegContinePos(m_vCenter,m_vLineSegVer);
	sc2Vector HorStart = GetLinesegContinePos(m_vCenter,m_vLineSegHor);

	m_vGuiLineSegVerEx->SetStartXYEndXY(VerStart, m_vCenter);
	m_vGuiLineSegHorEx->SetStartXYEndXY(HorStart, m_vCenter);

	m_vGuiLineSegVer->SetSelected(TRUE);
	m_vGuiLineSegHor->SetSelected(TRUE);
	m_vGuiLineSegVerEx->SetSelected(TRUE);
	m_vGuiLineSegHorEx->SetSelected(TRUE);
}

