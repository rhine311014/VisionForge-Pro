# Halcon的模板匹配学习

## 一、基于灰度的模板匹配：

应用场合：定位对象内部的灰度值没有大的变化，没有缺失部分，没有干扰图像和噪声的场合。
1.创建模板：create_template()
2.寻找模板：best_match (ImageSearch, TemplateID, 30, 'false', RowNew, ColumnNew, Error)
参数：输入图像，模板编号，灰度值的最大平均差，是否采用亚像素，匹配的行坐标，匹配的列坐标，最佳匹配的灰度值的平均发散度
3.释放模板：clear_template()

~~~bash
dev_close_window ()
dev_open_window (0, 0, 599, 464, 'black', WindowID)
*读取了一幅彩色图像
read_image (Imagecolor, 'data/holesBoard')
*将其转化为灰度图像
rgb1_to_gray (Imagecolor, Image)
dev_set_draw ('margin')
dev_set_line_width(3)
Row1 :=700
Column1 := 950
Row2 := 906
Column2 := 1155
*选择了一块矩形的ROI区域
gen_rectangle1 (Rectangle, Row1, Column1, Row2, Column2)
dev_display (Rectangle)
*将ROI区域进行裁剪，变成模板图像
reduce_domain (Image, Rectangle, ImageReduced)
*创建模板，因为光照比较稳定，GrayValues选择‘original’
create_template (ImageReduced, 5, 4, 'sort', 'original', TemplateID)	
*读取测试图像
read_image (ImageNoise, 'data/holesBoardNoise')
*应用灰度模板并进行匹配
adapt_template (ImageNoise, TemplateID)
best_match_mg (ImageNoise, TemplateID, 35, 'false', 4, 'all', Row_, Column_, Error_)
dev_clear_window ()
dev_display (ImageNoise)
*根据匹配返回的坐标中心，绘制矩形标识框，将匹配到的目标框选出来
disp_rectangle2 (WindowID, Row_, Column_, 0, 95, 95)
*匹配结束，释放模板资源
clear_template (TemplateID)
~~~

## 二、基于互相关匹配：

应用场合：搜索对象有轻微的变形，大量的纹理，图像模糊等场合，速度快，精度低。
1.创建模板：create_ncc_model()
2.寻找模板：find_ncc_model()
3.释放模板：clear_ncc_model()

~~~bash
*读取参考的原始图像。如果是彩色的，需要先转化为单通道灰度图像
read_image (Image, 'data/carmex-0')
rgb1_to_gray (Image, GrayImage)
get_image_size (GrayImage, Width, Height)
dev_close_window ()
dev_open_window (0, 0, Width, Height, 'black', WindowHandle)
*设置窗口绘制参数，线宽设为3
dev_set_line_width(3)
dev_set_draw ('margin')
*创建圆形，因为目标区域是圆形，因为用圆形将ROI区域选择出来
gen_circle (Circle, 161, 208, 80)
*获取圆形的中心点，为匹配后的可视化显示结果做准备
area_center (Circle, Area, RowRef, ColumnRef)
*裁剪ROI区域，得到模板图像
reduce_domain (GrayImage, Circle, ImageReduced)
*创建基于相关性的匹配模型，输入模板图像和模型参数
create_ncc_model (ImageReduced, 'auto', 0, 0, 'auto', 'use_polarity', ModelID)
*显示原始图像和圆形框
dev_display (Image)
dev_display (Circle)
stop ()
*读取测试图像。该测试图像和参考图像比起来有轻微的位移，旋转，缩放，以及失焦
read_image (Image2, 'data/carmex-1')
rgb1_to_gray (Image2, GrayImage1)
*进行行基于相关性的模板匹配
find_ncc_model (GrayImage1, ModelID, 0, 0, 0.5, 1, 0.5, 'true', 0, Row, Column, Angle, Score)
vector_angle_to_rigid (RowRef, ColumnRef, 0, Row, Column, 0, HomMat2D)
*对圆形进行仿射变换，使其将匹配的结果目标标识出来
affine_trans_region (Circle, RegionAffineTrans, HomMat2D, 'nearest_neighbor')
*显示测试画面和圆形标记圈
dev_display (Image2)
dev_display (RegionAffineTrans)
*匹配结束，释放模板资源
clear_ncc_model (ModelID)
~~~

## 三、基于形状的模板匹配：

应用场合：定位对象内部的灰度值可以有变化，但对象轮廓一定要清晰平滑。
0.[可选]监视模板：inspect_shape_model()检查参数的适用性，还能帮助找到合适的参数
1.创建形状模型：create_shape_model()
2.保存、读入形状模板：write_shape_model (ModelID, 'green-dot.shm')、read_shape_model ('green-dot.shm', ModelID)
3.[可选]获得这个模板的轮廓，用于后面的匹配get_shape_model_contours ()
4.寻找形状模型：find_shpae_model()，返回一个模板实例的长、宽和旋转角度
5.释放形状模型：clear_shape_model()
6.结果转化 vector_angle_to_rigid或 affine_trans_contour_xld后期结果的仿射变换和轮廓处理

~~~bash
*读取参考图像
read_image(Image, 'data/labelShape-0')
*根据要匹配的目标，围绕目标创建一个矩形，获取ROI区域
gen_rectangle1 (Rectangle, 34, 290, 268, 460)
*对ROI区域进行裁剪，得到模板图像
reduce_domain (Image, Rectangle, ImageReduced)
*测试金字塔的层级参数
inspect_shape_model (ImageReduced, ModelImages, ModelRegions, 4, 30)
*设置显示图像、绘制线条的线宽等窗口参数
dev_set_draw ('margin')
dev_set_line_width(3)
dev_display(Image)
dev_display(Rectangle)
*根据剪裁的模板图像创建基于形状的模板，返回模板句柄ShapeModelID
create_shape_model (ImageReduced, 5, rad(-10), rad(20), 'auto', 'none', 'use_polarity', 20, 10, 	ShapeModelID)
stop()
*读取用于测试的图像
read_image(SearchImage, 'data/labelShape-1')
*使用匹配算子进行形状模板匹配
find_shape_model (SearchImage, ShapeModelID, 0, rad(360), 0.5, 3, 0, 'least_squares', 0, 0.5, 	RowCheck, ColumnCheck, AngleCheck, Score)
*显示匹配结果，将匹配得到的实例以形状轮廓的形式绘制出来
dev_display_shape_matching_results (ShapeModelID, 'red', RowCheck, ColumnCheck, AngleCheck, 1, 1, 0)
*匹配结束，释放模板资源
clear_shape_model (ShapeModelID)
~~~

## 四、基于组件的模板匹配：

应用场合：组件匹配是形状匹配的扩展，但不支持大小缩放匹配，一般用于多个对象（工件）定位的场合。
1.获取组件模型里的初始控件 gen_initial_components()
2.根据图像模型，初始组件，训练图片来训练组件和组件相互关系  train_model_components()
3.创建组件模型 create_trained_component_model()
4.寻找组件模型 find_component_model()
5.释放组件模型 clear_component_model()

~~~bash
dev_close_window ()
*读取参考图像，这里读取的是单通道灰度图像
read_image (ModelImage, 'data/bolts-0') 
*设置显示图像、绘制线条等窗口参数
dev_open_window_fit_image (ModelImage, 0, 0, -1, -1, WindowHandle)
dev_display (ModelImage)
dev_set_draw ('margin')
dev_set_line_width(3)
stop ()
*定义各个组件, 选取各个组件的ROI区域
gen_rectangle1 (Rectangle1, 140, 71, 279, 168)
gen_rectangle1 (Rectangle2, 181, 281,285, 430)
gen_circle (Circle, 106, 256, 60)
*将所有组件放进一个名为ComponentRegions的Tuple中
concat_obj (Rectangle1, Rectangle2, ComponentRegions)
concat_obj (ComponentRegions, Circle, ComponentRegions)
*显示参考图像，以及选择的各个组件区域。核对区域选择是否理想
dev_display (ModelImage)
dev_display (ComponentRegions)
stop ()
*创建基于组件的模板，返回模板句柄ComponentModelID
create_component_model (ModelImage, ComponentRegions, 20, 20, rad(25), 0, rad(360), 15, 40, 15, 10, 0.8, 3, 0, 'none', 'use_polarity', 'true', ComponentModelID, RootRanking)
*读取测试图像，该测试图像相对于参考图像有一定的位移和旋转。
read_image (SearchImage, 'data/bolts-1')
*在参考图像模板的基础上，进行基于基于组件的匹配
find_component_model (SearchImage, ComponentModelID, RootRanking, 0, rad(360), 0.5, 0, 0.5, 'stop_search', 'search_from_best', 'none', 0.8, 'interpolation', 0, 0.8, ModelStart, ModelEnd, Score, RowComp, ColumnComp, AngleComp, ScoreComp, ModelComp)
*显示测试图像
dev_display (SearchImage)
*对每一个检测到的组件实例进行可视化的显示
for Match := 0 to |ModelStart| - 1 by 1
	dev_set_line_width (4)
	*获得每个组件的实例和位移旋转等参数
	get_found_component_model (FoundComponents, ComponentModelID, ModelStart, ModelEnd, RowComp, ColumnComp, AngleComp, ScoreComp, ModelComp, Match, 'false', RowCompInst, ColumnCompInst, AngleCompInst, ScoreCompInst)
	dev_display (FoundComponents)
endfor
stop ()
*匹配结束，释放模板资源
clear_component_model (ComponentModelID)
~~~

## 五、基于局部变形匹配：

应用场合：搜索对象有轻微的变形。
1.创建模板：create_local_deformable_model()
2.寻找模板：find_local_deformable_model()
3.释放模板：clear_deformable_model()

~~~bash
dev_close_window ()
*读取参考图像，这里读取的是单通道灰度图像	
*这里的参考图像是已经剪裁好的感兴趣区域图像，可以直接作为模板图像
read_image (ModelImage, 'data/creamlabel')
*设置显示窗口参数
dev_open_window_fit_image (ModelImage, 0, 0, -1, -1, WindowHandle)
*创建局部形变模板，返回局部形变模板句柄ModelID
create_local_deformable_model (ModelImage, 'auto', rad(-15), rad(30), 'auto', 1, 1, 'auto', 1, 1, 'auto', 'none', 'use_polarity', [40,60], 'auto', [], [], ModelID)
*获取局部形变模板的轮廓
get_deformable_model_contours (ModelContours, ModelID, 1)
*为了将模板轮廓可视化显示，需要将轮廓与图像实物对应起来
*因此出于可视化显示的目的，先获取模板图像的几何中心
area_center (ModelImage, Area, Row, Column)
*进行仿射变换
hom_mat2d_identity (HomMat2DIdentity)
hom_mat2d_translate (HomMat2DIdentity, Row, Column, HomMat2DTranslate)
affine_trans_contour_xld (ModelContours, ContoursAffinTrans, HomMat2DTranslate)
*设置轮廓显示的线条参数，显示模板图像与轮廓
dev_set_line_width (2)
dev_display (ModelImage)
dev_display (ContoursAffinTrans)
stop ()
*读取测试图像，这里的图像中更包含模板图像，并且有一定的形变
read_image (DeformedImage, 'data/cream')
*显示用于测试的局部形变图像
dev_resize_window_fit_image (DeformedImage, 0, 0, -1, -1)
dev_display (DeformedImage)
*进行局部形变模板匹配
find_local_deformable_model (DeformedImage, ImageRectified, VectorField, DeformedContours, ModelID, rad(-14), rad(28), 0.9, 1, 0.9, 1, 0.78, 0, 0, 0, 0.7, ['image_rectified','vector_field','deformed_contours'], ['deformation_smoothness','expand_border','subpixel'], [18,0,0], Score, Row, Column)
*显示形变轮廓
dev_display (DeformedImage)
dev_set_line_width (2)
dev_set_color ('red')
dev_display (DeformedContours)
stop()
*匹配结束，释放模板资源
clear_deformable_model (ModelID)
~~~

## 六、基于描述匹配：

应用场合：搜索对象有轻微的变形，透视的场合，根据一些描述点的位置和灰度值来进行匹配。
1.创建模板：create_calib_descriptor_model()
2.寻找模板：find_calib_descriptor_model()
3.释放模板：clear_descriptor_model()

~~~bash
* This example shows a cookie box from different views.
* First, models from reference poses are trained.
* In the second step these models are searched in every image.
* By generating a virtual pose out of the projection result
* it can be estimated whether the box is upside down or not.
* 
dev_update_off ()
dev_close_window ()
read_image (Image, 'packaging/cookie_box_reference_01')
dev_open_window_fit_image (Image, 0, 0, -1, -1, WindowHandle)
dev_set_draw ('margin')
set_display_font (WindowHandle, 14, 'mono', 'true', 'false')
* 
* Generate ROI for model training
Row1Roi := [224, 224, 224, 128]
Col1Roi := [122, 112, 218, 130]
Row2Roi := [406, 406, 410, 406]
Col2Roi := [550, 540, 497, 556]
XWorldBox := []
YWorldBox := []
Models := []
NumModels := 4
TimeCreation := 0
TotalTime := 0
* 
* Approximately determined coordinates of the cookie box
XMod1To4 := [-189, 189, 189, -189, -189, 189, 189, -189, -120, 120, 120, -120, -185, 185, 185, -185] / 1000.0
YMod1To4 := [-80, -80, 80, 80, -80, -80, 80, 80, -80, -80, 80, 80, -120, -120, 120, 120] / 1000.0
* 
* Camera parameters
gen_cam_par_area_scan_division (0.0155565, -109.42, 1.28008e-005, 1.28e-005, 322.78, 240.31, 640, 480, CamParam)
dev_display (Image)
disp_message (WindowHandle, ['Press \'Run\' to start model creation ...', '(will take a few minutes)'], 'window', 10, 10, 'black', 'true')
disp_continue_message (WindowHandle, 'black', 'true')
stop ()
* 
* For every reference image the descriptor model is created
for IndexR := 0 to NumModels - 1 by 1
    read_image (Image, 'packaging/cookie_box_reference_' + (IndexR + 1)$'.02')
    gen_rectangle1 (Rectangle, Row1Roi[IndexR], Col1Roi[IndexR], Row2Roi[IndexR], Col2Roi[IndexR])
    dev_display (Image)
    dev_set_line_width (4)
    dev_set_color ('blue')
    dev_display (Rectangle)
    reduce_domain (Image, Rectangle, ImageReduced)
    disp_message (WindowHandle, 'Creating model no. ' + (IndexR + 1) + '/' + NumModels + ' ... please wait.', 'window', 10, 10, 'black', 'true')
    * 
    RowsRoi := [Row1Roi[IndexR],Row1Roi[IndexR],Row2Roi[IndexR],Row2Roi[IndexR]]
    ColumnsRoi := [Col1Roi[IndexR],Col2Roi[IndexR],Col2Roi[IndexR],Col1Roi[IndexR]]
    WorldX := XMod1To4[IndexR * 4:(IndexR + 1) * 4 - 1]
    WorldY := YMod1To4[IndexR * 4:(IndexR + 1) * 4 - 1]
    vector_to_pose (WorldX, WorldY, [], RowsRoi, ColumnsRoi, CamParam, 'planar_analytic', 'error', Pose, Quality)
    image_points_to_world_plane (CamParam, Pose, RowsRoi, ColumnsRoi, 'm', XBox, YBox)
    YWorldBox := [YWorldBox,YBox]
    XWorldBox := [XWorldBox,XBox]
    * 
    count_seconds (SecondsCreation1)
    create_calib_descriptor_model (ImageReduced, CamParam, Pose, 'harris_binomial', [], [], ['depth', 'number_ferns', 'patch_size', 'min_scale', 'max_scale'], [11, 30, 17, 0.4, 1.2], 42, ModelID)
    count_seconds (SecondsCreation2)
    TimeCreation := TimeCreation + (SecondsCreation2 - SecondsCreation1)
    Models := [Models,ModelID]
endfor
* 
* Model creation is finished
dev_display (Image)
disp_continue_message (WindowHandle, 'black', 'true')
disp_message (WindowHandle, NumModels + ' models created in ' + TimeCreation + ' seconds.', 'window', 10, 10, 'black', 'true')
stop ()
* 
* Main loop:
* Search the models in all images
for Index := 11 to 21 by 1
    InstancesFound := 0
    OutputString := []
    TextRow := 10
    TotalTime := 0
    dev_clear_window ()
    read_image (Image, 'packaging/cookie_box_' + Index$'.02')
    dev_display (Image)
    disp_message (WindowHandle, 'Searching image ...', 'window', 10, 10, 'black', 'true')
    * 
    * Search every model in each image
    * 
    for Index1 := 0 to NumModels - 1 by 1
        count_seconds (Seconds1)
        find_calib_descriptor_model (Image, Models[Index1], [], [], [], [], 0.25, 1, CamParam, 'num_points', Pose, Score)
        count_seconds (Seconds2)
        Time := Seconds2 - Seconds1
        TotalTime := TotalTime + Time
        * 
        if (|Score| > 0)
            get_descriptor_model_points (Models[Index1], 'search', 0, Row, Col)
            dev_set_color ('green')
            dev_set_colored (12)
            dev_set_line_width (1)
            gen_cross_contour_xld (Cross1, Row, Col, 6, 0.785398)
            dev_display (Cross1)
            dev_set_color ('lime green')
            dev_set_line_width (3)
            disp_3d_coord_system (WindowHandle, CamParam, Pose, 0.07)
            * 
            XWorldB := XWorldBox[Index1 * 4:(Index1 + 1) * 4 - 1]
            YWorldB := YWorldBox[Index1 * 4:(Index1 + 1) * 4 - 1]
            pose_to_hom_mat3d (Pose, HomMat3D)
            affine_trans_point_3d (HomMat3D, XWorldB, YWorldB, [0, 0, 0, 0], XTrans, YTrans, ZTrans)
            project_3d_point (XTrans, YTrans, ZTrans, CamParam, RowTrans, ColTrans)
            * 
            gen_contour_polygon_xld (Contour, RowTrans, ColTrans)
            close_contours_xld (Contour, Contour)
            dev_display (Contour)
            * 
            if ((Pose[5] > 45 and Pose[5] < 135) or (Pose[5] > 225 and Pose[5] < 315))
                OutputString := [OutputString,'Model ' + Index1 + ' found in ' + (Time * 1000)$'.4' + ' ms (on the side) \n']
            elseif (Pose[5] > 135 and Pose[5] < 225)
                OutputString := [OutputString,'Model ' + Index1 + ' found in ' + (Time * 1000)$'.4' + ' ms (upside down)\n']
            else
                OutputString := [OutputString,'Model ' + Index1 + ' found in ' + (Time * 1000)$'.4' + ' ms\n']
            endif
            if (|Score| == 0)
                OutputString := 'No model found'
                disp_message (WindowHandle, 'No model found.', 'window', 10, 10, 'black', 'true')
            endif
        endif
    endfor
    OutputString := [OutputString,'Overall search time: ' + (TotalTime * 1000)$'.4' + ' ms']
    disp_message (WindowHandle, OutputString, 'window', 10, 10, 'black', 'true')
    disp_continue_message (WindowHandle, 'black', 'true')
    stop ()
endfor
dev_display (Image)
disp_message (WindowHandle, 'Program finished.\nPress \'Run\' to clear all descriptor models.', 'window', 10, 10, 'black', 'true')
~~~

## 七、基于线性变形匹配：

应用场合：搜索对象有线性的变形，模板在行列方向上可以分别独立的进行一个适当的缩放变形来匹配， 主要参数有行列方向查找缩放比例、图像金字塔、行列方向匹配分数（指可接受的匹配分数，大于这个值就接受，小于它就舍弃）、设置超找的角度、已经超找结果后得到的位置和匹配分数。 分带标定的可变形模板匹配和不带标定的可变形模板匹配。
带标定的需要先读入摄像机内参 read_cam_par和外参 read_pose
1.创建模板：create_planar_calib_deformable_model()、create_planar_uncalib_deformable_model()
2.寻找模板：find_planar_calib_deformable_model()、find_planar_uncalib_deformable_model()
3.释放模板：clear_deformable_model()

~~~bash
dev_close_window ()
*读取参考图像，这里的参考图像应只包含识别的关键区域，用于创建模板
read_image (ImageLabel, 'data/labelShape-0')
*设置窗口参数用于显示图像
get_image_size (ImageLabel, Width, Height)
dev_open_window (0, 0, Width, Height, 'black', WindowHandle1)
dev_set_draw ('margin')
dev_display (ImageLabel)
*设置用于存储特征点和感兴趣区域的变量
NumPoints := []
RowRoi := [10,10,Height - 10,Height - 10]
ColRoi := [10,Width - 10,Width - 10,10]
*将参考图像中的除边缘外的区域都设为感兴趣区域。因为参考图像已经近似于匹配的纹理样本
gen_rectangle1 (Rectangle, 10, 10, Height - 10, Width - 10)
*显示参考图像上选择的ROI区域
dev_set_line_width (4)
dev_display (Rectangle)
stop ()
*将感兴趣区域剪裁为模板图像
reduce_domain (ImageLabel, Rectangle, ImageReduced)
dev_clear_window ()
dev_display (ImageLabel)
*创建基于描述符的模板
create_uncalib_descriptor_model (ImageReduced, 'harris_binomial', [], [], ['min_rot','max_rot','min_scale','max_scale'], [-90,90,0.2,1.1], 42, ModelID)
*设置模型的原点，为了后面获取坐标作参照
set_descriptor_model_origin (ModelID, -Height / 2, -Width / 2)
*获取模型中特征点的位置
get_descriptor_model_points (ModelID, 'model', 'all', Row_D, Col_D)
*将模型中计算出的特征点存入NumPoints变量中
NumPoints := [NumPoints,|Row_D|]

*读取测试图像，这里读取的是单通道灰度图像，因此省略了通道转化的步骤
read_image (ImageGray, 'data/labelShape-1')
dev_resize_window_fit_image (ImageGray, 0, 0, -1, -1)
dev_display (ImageGray)
*对描述符特征点进行匹配
find_uncalib_descriptor_model (ImageGray, ModelID, 'threshold', 800, ['min_score_descr','guided_matching'], [0.003,'on'], 0.25, 1, 'num_points', HomMat2D, Score)
*显示匹配结果，将特征点用不同的颜色绘制出来
if ((|HomMat2D| > 0) and (Score > NumPoints[0] / 4))
    get_descriptor_model_points (ModelID, 'search', 0, Row, Col)
	*创建十字标识符
    gen_cross_contour_xld (Cross, Row, Col, 6, 0.785398)
    projective_trans_region (Rectangle, TransRegion, HomMat2D, 'bilinear')
    projective_trans_pixel (HomMat2D, RowRoi, ColRoi, RowTrans, ColTrans)
    angle_ll (RowTrans[2], ColTrans[2], RowTrans[1], ColTrans[1], RowTrans[1], ColTrans[1], RowTrans[0], ColTrans[0], Angle)
    Angle := deg(Angle)
    if (Angle > 70 and Angle < 110)
        area_center (TransRegion, Area, Row, Column)
        dev_set_color ('green')
        dev_set_line_width (4)
        dev_display (TransRegion)
        dev_set_colored (6)
        dev_display (Cross)                
   endif
endif
stop ()
*匹配结束，释放模板资源
clear_descriptor_model (ModelID)
~~~

## 八、基于比例缩放变形匹配：

应用场合：搜索对象有比例缩放的变形， 介于一般形状匹配和线性变形匹配之间的一种方法。它可以匹配放大或是缩小的模板，但是仅限于模板大小的缩放，即行列缩放因子一样。这也是它和线性缩放最大的不同。
1.创建模板：create_scaled_shape_model()
2.寻找模板：find_scaled_shape_model()
3.释放模板：clear_deformable_model()

~~~bash
read_image (Image, 'green-dot')
threshold (Image, Region, 0, 128)
connection (Region, ConnectedRegions)
select_shape (ConnectedRegions, SelectedRegions, 'area', 'and', 10000, 20000)
fill_up (SelectedRegions, RegionFillUp)
dilation_circle (RegionFillUp, RegionDilation, 5.5)
reduce_domain (Image, RegionDilation, ImageReduced)
create_scaled_shape_model (ImageReduced, 5, rad(-45), rad(90), 'auto', 0.8, 1.0, 'auto', 'none', 'ignore_global_polarity', 40, 10, ModelID)
get_shape_model_contours (Model, ModelID, 1)
area_center (RegionFillUp, Area, RowRef, ColumnRef)
vector_angle_to_rigid (0, 0, 0, RowRef, ColumnRef, 0, HomMat2D)
affine_trans_contour_xld (Model, ModelTrans, HomMat2D)
read_image (ImageSearch, 'green-dots')
find_scaled_shape_model (ImageSearch, ModelID, rad(-45), rad(90), 0.8, 1.0, 0.5, 0, 0.5, 'least_squares', 5, 0.8, Row, Column, Angle, Scale, Score)
for I := 0 to |Score| - 1 by 1
    hom_mat2d_identity (HomMat2DIdentity)
    hom_mat2d_translate (HomMat2DIdentity, Row[I], Column[I], HomMat2DTranslate)
    hom_mat2d_rotate (HomMat2DTranslate, Angle[I], Row[I], Column[I], HomMat2DRotate)
    hom_mat2d_scale (HomMat2DRotate, Scale[I], Scale[I], Row[I], Column[I], HomMat2DScale)
    affine_trans_contour_xld (Model, ModelTrans, HomMat2DScale)
    dev_display (ModelTrans)
endfor
clear_shape_model (ModelID)
~~~

## 九、基于变化模型匹配

应用场合：搜索对象有轻微的变形。
1.创建模板：create_variation_model()
2.准备和训练模型：prepare_variation_model()、train_variation_model()
3.比较模板：compare_variation_model()
4.释放模板： clear_variation_model()

~~~bash
* 印刷Logo的缺陷检测核心逻辑梳理
* 1、找到标准图像作为模板图像
* 2、选出模板图像中需要进行检测区域（ROI）
* 3、进行区域处理，获取对应检测区域最小范围
* 4、根据选出检测区域范围图像进行创建对应形状模板
* 5、在创建变形模板
* 6、训练变形模板
* 7、通过变形模板检测出与其待测图像检测区域的差异化
** 获取窗口句柄
dev_get_window (WindowHandle)
** 设置窗口句柄的属性
dev_set_color ('green')
dev_set_line_width (2)
dev_set_draw ('margin')
*********1、获取模板图像************************
** 读取作为模板图像的内容
read_image (Image, 'pen-01')
** 选取做为模板图像的区域ROI
draw_rectangle1 (WindowHandle, Row1, Column1, Row2, Column2)
gen_rectangle1 (Rectangle, Row1, Column1, Row2, Column2)
** 锁定矩形区域的图像
reduce_domain (Image, Rectangle, ImageReduced)
** 二值化分割
threshold (ImageReduced, Region, 0, 80)
** 根据分割区域转换为一个多边形区域
shape_trans (Region, RegionTrans, 'convex')
** 区域膨胀
dilation_circle (RegionTrans, RegionDilation, 8)
** 获取模板区域中心
area_center (RegionDilation, Area, ModelCenterRow, ModelCenterColumn)
** 获取形状模板图像
reduce_domain (Image, RegionDilation, TemplateImage)
******************2、创建模板*************************
** 创建形状模板
create_shape_model (TemplateImage, 'auto', -0.39, 0.79, 'auto', 'auto', 'use_polarity', 'auto', 'auto', ModelID)
******************3、创建变形模板并进行训练*******************
** 获取加载模板内容图像的分辨率
get_image_size (Image, Width, Height)
** 创建变形模板
create_variation_model (Width, Height, 'byte', 'standard', VarModelID)
** 获取训练变形模板的标准图像
list_files ('./', 'files', Files)
** 获取所有图像文件
tuple_regexp_select (Files, '.png', ImageFiles)
** 训练使用前15张图像对变形模板进行训练
for Index := 0 to 14 by 1
    ** 获取当前遍历图像
    read_image (TrainImage, ImageFiles[Index])
    ** 使用形状模板来定位是否存在检测区域
    find_shape_model (TrainImage, ModelID, -0.39, 0.79, 0.5, 1, 0.5, 'least_squares', 0, 0.9, Row, Column, Angle, Score)
    if (|Row| == 0)
        continue
    endif
    ** 创建刚体变换矩阵
    vector_angle_to_rigid (Row, Column, Angle, ModelCenterRow, ModelCenterColumn, 0, HomMat2D)
    ** 仿射变换训练图像到模板位置
    affine_trans_image (TrainImage, ImageAffineTrans, HomMat2D, 'constant', 'false')
    ** 获取模板匹配的图像
    reduce_domain (ImageAffineTrans, RegionDilation, ImageReduced1)
    ** 使用匹配到图像进行变现模板训练
    train_variation_model (ImageReduced1, VarModelID)
endfor

**********************使用为训练图像来检测是否存在缺陷***************
** 获取变形模板
get_variation_model (Image1, VarImage, VarModelID)
** 准备分割参数
prepare_variation_model (VarModelID, 30, 2)
** 把字体设置大点
set_display_font (WindowHandle, 36, 'mono', 'true', 'false')
for Index1 := 14 to |ImageFiles| - 1 by 1
    dev_clear_window ()
    ** 获取当前遍历图像
    read_image (TrainImage1, ImageFiles[Index1])
    ** 使用形状模板来定位是否存在检测区域
    find_shape_model (TrainImage1, ModelID, -0.39, 0.79, 0.5, 1, 0.5, 'least_squares', 0, 0.9, Row, Column, Angle, Score)
    if (|Row| == 0)
        continue
    endif
    ** 创建刚体变换矩阵
    vector_angle_to_rigid (Row, Column, Angle, ModelCenterRow, ModelCenterColumn, 0, HomMat2D)
    ** 仿射变换训练图像到模板位置
    affine_trans_image (TrainImage1, ImageAffineTrans1, HomMat2D, 'constant', 'false')
    ** 获取模板匹配的图像
    reduce_domain (ImageAffineTrans1, RegionDilation, ImageReduced1)
    ** 获取模板区域与待测区域的差分区域
    compare_variation_model (ImageReduced1, Region1, VarModelID)
    ** 判定是否为缺陷
    select_shape (Region1, SelectedRegions, 'area', 'and', 20, 99999)
    ** 统计是存满足条件的区域
    count_obj (SelectedRegions, Number)
    ** 显示检测结果
    dev_display (ImageAffineTrans1)
    if (Number > 0)
        disp_message (WindowHandle, 'NG', 'window', 20, 20, 'red', 'false')
        dev_set_color ('red')
        dev_display (SelectedRegions)
    else
        disp_message (WindowHandle, 'OK', 'window', 20, 20, 'green', 'false')
    endif
endfor
~~~



