#include "WorkerThread.h"
#include <QDebug>


WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent), m_stopRequested(false)
{
}

WorkerThread::~WorkerThread()
{
    stopWork();
    wait();  // 确保线程安全退出
}

void WorkerThread::startWork()
{
    if (!isRunning()) {
        m_stopRequested = false;
        start();  // 启动 run()
    }
}

void WorkerThread::stopWork()
{

    m_stopRequested = true;

}

bool WorkerThread::isRunningSafely()
{
    return isRunning() && !m_stopRequested;
}

void WorkerThread::setDispWindow(HTuple &window)
{
    hv_window=window;
}
void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold,
                      HTuple hv_Slant)
{

    // Local iconic variables

    // Local control variables
    HTuple  hv_OS, hv_Fonts, hv_Style, hv_Exception;
    HTuple  hv_AvailableFonts, hv_Fdx, hv_Indices;

    //This procedure sets the text font of the current window with
    //the specified attributes.
    //
    //Input parameters:
    //WindowHandle: The graphics window for which the font will be set
    //Size: The font size. If Size=-1, the default of 16 is used.
    //Bold: If set to 'true', a bold font is used
    //Slant: If set to 'true', a slanted font is used
    //
    GetSystem("operating_system", &hv_OS);
    if (0 != (HTuple(int(hv_Size==HTuple())).TupleOr(int(hv_Size==-1))))
    {
        hv_Size = 16;
    }
    if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
    {
        //Restore previous behavior
        hv_Size = (1.13677*hv_Size).TupleInt();
    }
    else
    {
        hv_Size = hv_Size.TupleInt();
    }
    if (0 != (int(hv_Font==HTuple("Courier"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Courier";
        hv_Fonts[1] = "Courier 10 Pitch";
        hv_Fonts[2] = "Courier New";
        hv_Fonts[3] = "CourierNew";
        hv_Fonts[4] = "Liberation Mono";
    }
    else if (0 != (int(hv_Font==HTuple("mono"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Consolas";
        hv_Fonts[1] = "Menlo";
        hv_Fonts[2] = "Courier";
        hv_Fonts[3] = "Courier 10 Pitch";
        hv_Fonts[4] = "FreeMono";
        hv_Fonts[5] = "Liberation Mono";
        hv_Fonts[6] = "DejaVu Sans Mono";
    }
    else if (0 != (int(hv_Font==HTuple("sans"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Luxi Sans";
        hv_Fonts[1] = "DejaVu Sans";
        hv_Fonts[2] = "FreeSans";
        hv_Fonts[3] = "Arial";
        hv_Fonts[4] = "Liberation Sans";
    }
    else if (0 != (int(hv_Font==HTuple("serif"))))
    {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Times New Roman";
        hv_Fonts[1] = "Luxi Serif";
        hv_Fonts[2] = "DejaVu Serif";
        hv_Fonts[3] = "FreeSerif";
        hv_Fonts[4] = "Utopia";
        hv_Fonts[5] = "Liberation Serif";
    }
    else
    {
        hv_Fonts = hv_Font;
    }
    hv_Style = "";
    if (0 != (int(hv_Bold==HTuple("true"))))
    {
        hv_Style += HTuple("Bold");
    }
    else if (0 != (int(hv_Bold!=HTuple("false"))))
    {
        hv_Exception = "Wrong value of control parameter Bold";
        throw HException(hv_Exception);
    }
    if (0 != (int(hv_Slant==HTuple("true"))))
    {
        hv_Style += HTuple("Italic");
    }
    else if (0 != (int(hv_Slant!=HTuple("false"))))
    {
        hv_Exception = "Wrong value of control parameter Slant";
        throw HException(hv_Exception);
    }
    if (0 != (int(hv_Style==HTuple(""))))
    {
        hv_Style = "Normal";
    }
    QueryFont(hv_WindowHandle, &hv_AvailableFonts);
    hv_Font = "";
    {
        HTuple end_val48 = (hv_Fonts.TupleLength())-1;
        HTuple step_val48 = 1;
        for (hv_Fdx=0; hv_Fdx.Continue(end_val48, step_val48); hv_Fdx += step_val48)
        {
            hv_Indices = hv_AvailableFonts.TupleFind(HTuple(hv_Fonts[hv_Fdx]));
            if (0 != (int((hv_Indices.TupleLength())>0)))
            {
                if (0 != (int(HTuple(hv_Indices[0])>=0)))
                {
                    hv_Font = HTuple(hv_Fonts[hv_Fdx]);
                    break;
                }
            }
        }
    }
    if (0 != (int(hv_Font==HTuple(""))))
    {
        throw HException("Wrong value of control parameter Font");
    }
    hv_Font = (((hv_Font+"-")+hv_Style)+"-")+hv_Size;
    SetFont(hv_WindowHandle, hv_Font);
    return;
}

void WorkerThread::run()
{

    try {


        // Local iconic variables
        HObject  ho_Image, ho_Regions, ho_ConnectedRegions;
        HObject  ho_SelectedRegions, ho_RegionUnion, ho_Partitioned;
        HObject  ho_SortedRegions;

        // Local control variables
        HTuple  hv_OCRHandle, hv_ImageFiles;
        HTuple  hv_Index, hv_Value, hv_Class, hv_Confidence, hv_Area;
        HTuple  hv_Row, hv_Column;

        //# 读取字体库
        ReadOcrClassMlp("ocr.omc", &hv_OCRHandle);
        set_display_font(hv_window, 40, "mono", "true", "false");

        int progressValue=0;
        //# 读图
        ListFiles("img", (HTuple("files").Append("follow_links")), &hv_ImageFiles);
        TupleRegexpSelect(hv_ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima|hobj)$").Append("ignore_case")),
                          &hv_ImageFiles);
        {
            HTuple end_val13 = (hv_ImageFiles.TupleLength())-1;
            HTuple step_val13 = 1;
            for (hv_Index=0; hv_Index.Continue(end_val13, step_val13); hv_Index += step_val13)
            {

                if (m_stopRequested) {
                    emit stopped();
                    break;
                }
                progressValue ++;
                emit progress(progressValue);

                ReadImage(&ho_Image, HTuple(hv_ImageFiles[hv_Index]));

                HalconCpp::HTuple hv_width,hv_height;
                HalconCpp::GetImageSize(ho_Image,&hv_width,&hv_height);
                HalconCpp::SetPart(hv_window,0,0,hv_height-1,hv_width-1);

                //阈值分割
                Threshold(ho_Image, &ho_Regions, 50, 140);

                //连通域分析
                Connection(ho_Regions, &ho_ConnectedRegions);

                //筛选字符区域
                SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 800, 3000);

                //合并操作
                Union1(ho_SelectedRegions, &ho_RegionUnion);

                //字符切分
                RegionFeatures(ho_RegionUnion, "width", &hv_Value);
                if (0 != (int((hv_Value.TupleLength())==0)))
                {

                    ClearWindow(hv_window);

                    DispObj(ho_Image, hv_window);

                    DispText(hv_window,//'未发现字符'
                             "\346\234\252\345\217\221\347\216\260\345\255\227\347\254\246",
                             "window", "top", "left", "red", "box", 0);
                    continue;
                }
                PartitionDynamic(ho_RegionUnion, &ho_Partitioned, hv_Value/3, 30);

                //字符排序
                SortRegion(ho_Partitioned, &ho_SortedRegions, "character", "true", "row");

                //字符识别
                DoOcrMultiClassMlp(ho_SortedRegions, ho_Image, hv_OCRHandle, &hv_Class, &hv_Confidence);

                //字符显示
                AreaCenter(ho_SortedRegions, &hv_Area, &hv_Row, &hv_Column);

                ClearWindow(hv_window);

                DispObj(ho_Image, hv_window);

                DispText(hv_window,hv_Class, "image", hv_Row-150, hv_Column-10,
                         "yellow", "box", 0);

                QThread::msleep(500);
            }
        }

        //# 释放OCR句柄
        ClearOcrClassMlp(hv_OCRHandle);


    } catch (const HalconCpp::HException&e)
    {
        qDebug()<<"Halcon算法异常"<<e.ProcName().Text();
    }

    emit stopped();

}

