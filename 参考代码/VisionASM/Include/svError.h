#pragma once

#pragma pack(push,8)

typedef long HStatus;

#define IsSuccess(status) (status == HSuccess)

#define HSuccess					0

//数学错误
#define HSingular					001001			// 奇异（不可逆）
#define HNullVector					001002			// 零向量
#define HDivisionByZero				001003			// 分母为0

//图像错误
#define HInvalidImage				002001			// 图像内存为空，或者图像大小不合法
#define HInvalidImageFormat			002002			// 无效的图像格式
#define HInvalidImageSize			002002			// 无效的图像尺寸

//图像文件错误
#define HNoData						003001			// 无图像数据
#define HBadFile					003002			// 打开文件失败
#define HBadFormat					003003			// 文件类型错误
#define HBadParam					003004			// 无效输入参数
#define HMemErr						003005			// 内存分配失败
#define HUnsupport					003006			// 不支持的图像格式

//ROI区域错误
#define HInvalidRegion				004001			// 无效的区域

//直方图错误
#define HInvalidHistogramSize		005001			// 输入的直方图尺寸不对（不能为0）

//几何变换错误
#define HBadNumSamples				006001			// 输入的采样参数小于1

#pragma pack(pop)