// svGUIDef.h
// 图形用户接口头文件定义

#pragma once

#pragma pack(push,8)

#ifdef SVGUI_EXPORTS
#define SVGUI_API __declspec(dllexport)
#else
#define SVGUI_API __declspec(dllimport)
#endif

#pragma warning(disable: 4251)
#pragma warning(disable: 4996)

#define GuiDisplay	_T("GUIDisp")
#define GuiMaskEdit	_T("GUIMEdit")

#define WM_SV_USER					123
#define WM_SV_DISPLAY_MOUSEMOVE		(WM_USER+WM_SV_USER+1)
#define WM_SV_DISPLAY_MOUSEDOWN		(WM_USER+WM_SV_USER+2)
#define WM_SV_DISPLAY_MOUSEUP		(WM_USER+WM_SV_USER+3)
#define WM_SV_DISPLAY_CLICK			(WM_USER+WM_SV_USER+4)
#define WM_SV_DISPLAY_DBLCLICK		(WM_USER+WM_SV_USER+5)
#define WM_SV_DISPLAY_DRAGSTART		(WM_USER+WM_SV_USER+6)
#define WM_SV_DISPLAY_DRAGSTOP		(WM_USER+WM_SV_USER+7)
#define WM_SV_DISPLAY_HSCROLL		(WM_USER+WM_SV_USER+8)
#define WM_SV_DISPLAY_VSCROLL		(WM_USER+WM_SV_USER+9)
#define WM_SV_DISPLAY_TMCHANGE		(WM_USER+WM_SV_USER+10)
#define WM_SV_DISPLAY_MENU_CUSTOM1  (WM_USER+WM_SV_USER+11)
#define WM_SV_DISPLAY_MENU_CUSTOM2  (WM_USER+WM_SV_USER+12)
#define WM_SV_DISPLAY_MENU_CUSTOM3  (WM_USER+WM_SV_USER+13)
#define WM_SV_DISPLAY_LBUTTON_DOWN	(WM_USER+WM_SV_USER+14)        //鼠标左键
#define WM_SV_DISPLAY_RBUTTON_DOWN	(WM_USER+WM_SV_USER+15)        //鼠标右键
#define WM_SV_DISPLAY_MENU_MASK_DELETE	(WM_USER+WM_SV_USER+16)
#define WM_SV_DISPLAY_MENU_MASK_CLEAR	(WM_USER+WM_SV_USER+17)
#define WM_SV_DISPLAY_MENU_PARAM_EDIT	(WM_USER+WM_SV_USER+18)
#define WM_SV_DISPLAY_CONFUSION_CHANGE (WM_USER+WM_SV_USER+19)
#define WM_SV_DISPLAY_HEIGHT_PARAM_CHANGE (WM_USER+WM_SV_USER+20)
#define WM_SV_DISPLAY_LUMINANCE_PARAM_CHANGE (WM_USER+WM_SV_USER+21)
#define WM_SV_DISPLAY_EXIT_EDIT_MASK (WM_USER+WM_SV_USER+22)
#define WM_SV_DISPLAY_UPDATE		(WM_USER+WM_SV_USER+23)


#define SV_DISPLAY_MOUSEMOVE		0X0001
#define SV_DISPLAY_MOUSEDOWN		0X0002
#define SV_DISPLAY_MOUSEUP			0X0004
#define SV_DISPLAY_CLICK			0X0008
#define SV_DISPLAY_DBLCLICK			0X0010
#define SV_DISPLAY_DRAGSTART		0X0020
#define SV_DISPLAY_DRAGSTOP			0X0040
#define SV_DISPLAY_HSCROLL			0X0080
#define SV_DISPLAY_VSCROLL			0X0100
#define SV_DISPLAY_TMCHANGE			0X0200
#define SV_DISPLAY_CUSTOM1          0X0400
#define SV_DISPLAY_CUSTOM2          0X0800
#define SV_DISPLAY_CUSTOM3          0X1000
#define SV_DISPLAY_LBUTTON_DOWN		0X2000        //鼠标左键
#define SV_DISPLAY_RBUTTON_DOWN		0X4000        //鼠标右键
#define SV_DISPLAY_TRANS_PARAM_CHANGE 0X8000

#define RGBA(r,g,b,a)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)) | ((((DWORD)(BYTE)(a))<<24)))

//	图像像素格式
enum SVPixelFormat{
	pixelFormatUnknown   = -1,
	pixelFormatGrey8     = 8,		//每像素8bit的灰度图象
	pixelFormatGrey16    = 16,      //每像素16bit的灰度图像
	pixelFormatRGB888    = 24,		//每像素24bit的彩色图像
	pixelFormatRGB8888   = 32		//每像素32bit的彩色图像
};

// 用户自定义GuidisplayCustom消息枚举
enum CustomMsg {
	cmGuiDisplayNone      = -1,			// 无自定义Custom消息
	cmGuiDisplayCustom1,				// GuiDisplayCustom1消息
	cmGuiDisplayCustom2,				// GuiDisplayCustom2消息
	cmGuiDisplayCustom3				    // GuiDisplayCustom3消息
};

// 用户自定义GuidisplayCustom消息枚举
enum DispBGMode {
	bgDispBGPure = 0,				// 纯色
	bgDispBGChess =1,				// 棋盘格
	bgDispBGGrid,					// 网格
};

#pragma pack(pop)
