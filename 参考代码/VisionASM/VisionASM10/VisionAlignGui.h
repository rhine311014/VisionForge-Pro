#pragma once

#define MAX_GUI_COUNT_PER_POS 15			// 每个位置显示的最大GUI数量
#define MAX_GUI_COMBINE_COUNT_PER_POS	2	// 每个位置组合显示最大GUI数量
#define MAX_GUI_AREA_COUNT_PER_POS  25      // 每个位置分区域补偿最大数量

class vcBaseVisionAlign;
class CVisionAlignPosGui;
class CVisionAlignMarkPosGui;
class CVisionAlignPosCombineGui;
class CVisionAlignPosOffsetAeraGui;

// 所有位置GUI
class CVisionAlignGui
{
public:
	CVisionAlignGui();
	virtual ~CVisionAlignGui();
	
	//void SetGuiDisplay(int nPosNum, std::vector<scGuiDisplay*> vpGuiDisplay);
	void SetVisionRef(vcBaseVisionAlign* pVisionASM);
	void SetVisionDisplay();

	// 设置GUI隐藏 vnPosIndex.size() == 0 隐藏所有位置
	void SetAllPosGuiInvisible(int eSearchMode);
	void SetPosGuiInvisible(std::vector<int>& vnPosIndex, int eSearchMode);
	void PostMessagePosGuiInvisible(std::vector<int>& vnPosIndex, int eSearchMode);
	void UpdatePosGuiInvisible(int nPosIndex, int eSearchMode);

	// 设置GUI显示
	void SetAllPosSearchResult(int eSearchMode);  // 设置所有位置结果
	void SetPosSearchResult(std::vector<int> vnPosIndex,  int eSearchMode);	 // 根据位置设置结果, 调用搜索成功模板结果
	void SetPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex, int eSearchMode); // 根据位置, 同时定位执行的候补模板(检测候补模板同时定位);
	
	void SetPosSearchResult_TX(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex, int eSearchMode); // 腾盛定制修改候补模板显示方式);
	
	void PostMessagePosSearchResult(std::vector<int> vnPosIndex, int eSearchMode);

	void UpdatePosSearchResult(int nPosIndex, int eSearchMode);

	void SetAllPosObjectToTargetResultLine();													// 设置所有位置目标连线结果
	void SetAllPosText(std::vector<BOOL>  bResult,std::vector<CString> vSearchResult);		// 设置所有位置文字显示结果
	void SetCurPosText(std::vector<int> vnPosIndex,std::vector<BOOL>  bResult,std::vector<CString> vSearchResult);		// 设置所有位置文字显示结果


	void SetPosObjectToTargetResultLine(std::vector<int> vnPosIndex);							// 设置对象到目标连线结果
	void PostMessagePosObjectToTargetResultLine(std::vector<int> vnPosIndex);					// 投递消息更新显示对象到目标的连线
	void UpdatePosObjectToTargetResultLine(int nPosIndex);										// 更新显示对象到目标的连线
	void SetAllPosObjectToTargetResultLineInvisible();											// 设置所有位置目标对象连线隐藏
	void SetPosObjectToTargetResultLineInvisible(std::vector<int> vnPosIndex);					// 设置对象到目标连线隐藏
	void PostMessagePosObjectToTargetResultLineGuiInvisible(std::vector<int> vnPosIndex);		// 投递消息
	void UpdatePosObjectToTargetLineGuiInvisible(int nPosIndex);								// 更新隐藏

	//设置按区域显示
	void SetAllPosAlignOffsetByArea(sc2Vector vPoint);							// 设置按区域补偿显示, 参数代表点在区域内会高亮
	void SetAllPosAlignOffsetByArea(std::vector<sc2Vector>& vvPoint);							// 设置按区域补偿显示, 参数代表点在区域内会高亮
	void PostMessageAlignOffsetByAreaShow(std::vector<int> vnPosIndex);													// 投递消息显示
	void UpdateAllPosAlignOffsetByAreaShow(int nPosIndex);
	void SetAllPosAlignOffsetByAreaInvisible();
	void PostMessageAlignOffsetByAreaInvisible(std::vector<int> vnPosIndex);
	void UpdateAllPosAlignOffsetByAreaInvisible(int nPosIndex);
	int  GetAlignOffsetByArea(sc2Vector vPoint);


	void SetConstTextShow(std::vector<int> vnPosIndex, const CString& strInfo, int eType);

	void Changelanguage();


	CVisionAlignMarkPosGui* GetVisionAlignConstGui(int nPosIndex);

protected:
	void SetObjectPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);
	void SetVirtualObjectPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);
	void SetTargetPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);
	void SetCalibPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);
	void SetInspectPosSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);
	void SetCalibTargetSearchResult(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);
	void SetInspectPosSearchResult_TX(std::vector<int> vnPosIndex,  std::vector<int> vnPattenIndex);//腾盛用

private:
// 	int m_nPosNum;
// 	std::vector<scGuiDisplay*> m_vpGuiDisplay;
	vcBaseVisionAlign* m_pVisionASM;

	std::vector<bool> m_vbIsShareGuiShow; // 判断是否是共享显示
	std::vector<CVisionAlignMarkPosGui*> m_vpAlignPosTextGui;	   // 显示固定大小文本信息定位结果GUI


	std::vector<CVisionAlignPosGui*> m_vpObjectAlignGui;	   // 对象拍照
	std::vector<CVisionAlignPosGui*> m_vpTargetAlignGui;	   // 目标拍照
	std::vector<CVisionAlignPosCombineGui*> m_vpObjectToTargetAlignGui;	// 对象到目标连线
	std::vector<CVisionAlignPosOffsetAeraGui*> m_vpAlignOffsetAreaGui; // 分区域补偿显示矩形
// 	std::vector<CVisionAlignPosGui*> m_vpInspectAlignGui;      // 检测相关GUI
// 	std::vector<CVisionAlignPosGui*> m_vpClibrateAlignGui;     // 标定相关GUI;
};


// 单个位置图形GUI
class CVisionAlignPosGui
{
public:
	CVisionAlignPosGui();
	CVisionAlignPosGui(const CVisionAlignPosGui& rhs);


	virtual ~CVisionAlignPosGui();

	void InitGui();
	void FreeGui();

	void SetGuiDisplay(scGuiDisplay* pGuiDisplay);	// 设置显示窗口

	void SetGuiProperty(SearchResultCrossSize guiProperty);

	//void SetShowAlnMarkTextGuiPos(sc2Vector vPos); // 设置显示图像坐标文本GUI位置
	void SetGuiData(std::vector<CSearchResult>& vSearchResult, bool bResult);
	void SetCoordPosLineStyle(int nPosIndex, int eStyle);

	void SetGuiInvisible();
	void UpdateGuiInvisible();		// 设置GUI隐藏
	void SetGuiResultText(bool bOk);
	void UpdateGuiShow();

	void ClearAlnGuiTextShow();

public:
	scGuiDisplay* m_pGuiDisplay;

protected:
 	void AddGuiToGuiDisplay();
 	void RemoveGuiFromGuiDisplay();

private: 
	// 显示图像坐标文本GUI
// 	scGuiText* m_pGuiShowImagePosText;
// 	CString    m_strGuiShowImagePosText;

	// 所有GUI公共属性
	SearchResultCrossSize m_GuiProperty;
	COLORREF		m_NGColor;


	// 显示定位点
	scGuiCoordCross m_vcGuiCoordCross[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiCoordCross[MAX_GUI_COUNT_PER_POS];
	sc2Vector m_vvlnGuiCoordCross[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiCoordCross[MAX_GUI_COUNT_PER_POS];
	scGuiGraphic::StyleFlags  m_vGuiCoordCrossLineStyle[MAX_GUI_COUNT_PER_POS]; // 实线还是虚线, 主要是为了应用投影显示


	// 显示其他文本数据
	scGuiText m_vcAlnGuiText[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiText[MAX_GUI_COUNT_PER_POS];
	CString m_vstrAlnGuiText[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiText[MAX_GUI_COUNT_PER_POS];
	// 显示线
	scGuiLine m_vcGuiLine[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiLine[MAX_GUI_COUNT_PER_POS];
	scLine m_vcAlnGuiLine[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiLine[MAX_GUI_COUNT_PER_POS];
	// 显示线段
	scGuiLineSeg m_vcGuiLineSeg[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiLineSeg[MAX_GUI_COUNT_PER_POS];
	scLineSeg m_vcAlnGuiLineSeg[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiLineSeg[MAX_GUI_COUNT_PER_POS];
	// 显示圆
	scGuiCircle m_vcGuiCircle[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiCircle[MAX_GUI_COUNT_PER_POS];
	scCircle m_vcAlnGuiCircle[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiCircle[MAX_GUI_COUNT_PER_POS];
	// 显示长圆
	scGuiObround m_vcGuiObround[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiObround[MAX_GUI_COUNT_PER_POS];
	scObround m_vcAlnGuiObround[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiObround[MAX_GUI_COUNT_PER_POS];
	// 显示搜索矩形框
	scGuiAffineRect m_vcSearchGuiRect[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnSearchGuiRect[MAX_GUI_COUNT_PER_POS];
	scAffineRect m_vcAlnSearchGuiRect[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnSearchGuiRect[MAX_GUI_COUNT_PER_POS];
	// 显示搜索结果框
	scGuiAffineRect m_vcGuiSearchResultAffineRect[MAX_GUI_COUNT_PER_POS];
	bool m_vbVisibleAlnSearchResultAffineRect[MAX_GUI_COUNT_PER_POS];
	scAffineRect m_vcAlnSearchResultAffineRect[MAX_GUI_COUNT_PER_POS];
	COLORREF m_vColorAlnSearchResultAffineRect[MAX_GUI_COUNT_PER_POS];

	// 显示搜索圆角矩形
	scGuiGenRect	m_vcGuiGenRect[MAX_GUI_COUNT_PER_POS];
	bool			m_vbVisibleAlnGuiGenRect[MAX_GUI_COUNT_PER_POS];
	scGenRect		m_vcAlnGenRect[MAX_GUI_COUNT_PER_POS];
	COLORREF		m_vColorAlnGuiGenRect[MAX_GUI_COUNT_PER_POS];
};


// 单个位置显示固定文本GUI
class CVisionAlignMarkPosGui
{
public:
	CVisionAlignMarkPosGui();
	CVisionAlignMarkPosGui(const CVisionAlignMarkPosGui& rhs);
	virtual ~CVisionAlignMarkPosGui();

	void InitObjectGui(const CString& strShowInfo);
	void InitTargetGui(const CString& strShowInfo);
	void FreeGui();

	void SetGuiDisplay(scGuiDisplay* pGuiDisplay);	// 设置显示窗口
	void SetGuiTextPos();


	void SetObjectMarkPos(const CString& strPos);
	void SetTargetMarkPos(const CString& strPos);

	void SetObjectSearchToolResult(const CString& strResult, COLORREF color);
	void SetTargetSearchToolResult(const CString& strResult, COLORREF color);


	void UpdateGuiShow();
	void UpdateAllGuiVisible(bool bVisible);

public:
	scGuiDisplay* m_pGuiDisplay;

protected:
	void AddGuiToGuiDisplay();
	void RemoveGuiFromGuiDisplay();

private: 

	bool m_bGuiUpdating;		//GUI是否正在更新中

	// 显示图像坐标文本GUI
	CString   m_strObjectText;
	char m_szObjectText[512];
	scGuiText m_scObjectGuiText;   // 对象

	CString   m_strTargetText;
	char m_szTargetText[512];
	scGuiText m_scTargetGuiText;   // 目标

	CString   m_strObjectSearchToolResult;	  // 对象定位工具搜索结果
	char m_szObjectSearchToolResult[512];
	scGuiText m_scObjectSearchToolResult;
	COLORREF  m_colorObjectSearchToolResult;

	CString   m_strTargetSearchToolResult;	  // 目标定位工具搜索结果
	char m_szTargetSearchToolResult[512];
	scGuiText m_scTargetSearchToolResult;
	COLORREF  m_colorTargetSearchToolResult;

};

class CVisionAlignPosCombineGui
{
public:
	CVisionAlignPosCombineGui();
	CVisionAlignPosCombineGui(const CVisionAlignPosCombineGui& rhs);
	virtual ~CVisionAlignPosCombineGui();
	void InitGui();
	void FreeGui();
	void SetGuiDisplay(scGuiDisplay* pGuiDisplay);	// 设置显示窗口
	void SetGuiProperty(SearchResultCrossSize guiProperty);
	void SetGuiData(std::vector<sc2Vector> vObj, std::vector<sc2Vector> vTar, bool bResult);  // 目标与对象点连线
	void SetGuiInvisible();
	void UpdateGuiInvisible();		// 设置GUI隐藏
	void UpdateGuiShow();			// 设置GUI显示
protected:
	void AddGuiToGuiDisplay();
	void RemoveGuiFromGuiDisplay();
public:
	scGuiDisplay* m_pGuiDisplay;
private: 
	SearchResultCrossSize m_GuiProperty;
	COLORREF		m_NGColor;
	scGuiLineSeg m_vcGuiLineSeg[MAX_GUI_COMBINE_COUNT_PER_POS];
	bool m_vbVisibleAlnGuiLineSeg[MAX_GUI_COMBINE_COUNT_PER_POS];
	scLineSeg m_vcAlnGuiLineSeg[MAX_GUI_COMBINE_COUNT_PER_POS];
	COLORREF m_vColorAlnGuiLineSeg[MAX_GUI_COMBINE_COUNT_PER_POS];
};

// 分区域补偿GUI
class CVisionAlignPosOffsetAeraGui
{
public:
	CVisionAlignPosOffsetAeraGui();
	CVisionAlignPosOffsetAeraGui(const CVisionAlignPosOffsetAeraGui& rhs);
	virtual ~CVisionAlignPosOffsetAeraGui();
	void InitGui();
	void FreeGui();
	void SetGuiDisplay(scGuiDisplay* pGuiDisplay);	// 设置显示窗口
	void SetGuiProperty(AlignOffsetByAreaGui guiProperty);
	void SetAlignOffsetByArea(AlignOffsetByAera alignOffsetByArea);
	void SetGuiData(sc2Vector vPoint);  // 设置产品坐标
	void SetGuiData(std::vector<sc2Vector>& vvPoint);
	void SetGuiInvisible();
	void UpdateGuiInvisible();		// 设置GUI隐藏
	void UpdateGuiShow();			// 设置GUI显示
	int  WithIn(sc2Vector vPoint);  // 返回点在哪个区域内
protected:
	void AddGuiToGuiDisplay();
	void RemoveGuiFromGuiDisplay();
public:
	scGuiDisplay* m_pGuiDisplay;
private: 
	AlignOffsetByAera  m_AlignOffsetArea;		// 分区域属性
	AlignOffsetByAreaGui m_GuiProperty;			// 分区域显示属性
	COLORREF		m_NGColor;
	LOGFONT			m_font;
	int  m_vnGuiLineWidth[MAX_GUI_AREA_COUNT_PER_POS];					// 线的宽度
	bool m_vbVisibleAlnGuiAreaRect[MAX_GUI_AREA_COUNT_PER_POS];			// GUI显示属性控制
	scRect m_vcAlnGuiAreaRect[MAX_GUI_AREA_COUNT_PER_POS];				// GUI位置属性控制
	COLORREF m_vColorAlnGuiAreaRect[MAX_GUI_AREA_COUNT_PER_POS];		// GUI颜色属性控制
	scGuiRect m_vcGuiAreaRect[MAX_GUI_AREA_COUNT_PER_POS];				// GUI控件
};



