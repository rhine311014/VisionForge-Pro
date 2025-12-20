



# 12月16号更新通讯问题

### 关键位置

### 1.重新打开通信接口

去掉`m_CommStatus != eCommTimeOut`

![image-20211216125213931](readme.assets/image-20211216125213931.png)

### 2.设置超时时间2000ms

![image-20211216125602994](readme.assets/image-20211216125602994.png)

### 3.接收指令去掉重发并添加日志

![image-20211216125708800](readme.assets/image-20211216125708800.png)

![image-20211216130001787](readme.assets/image-20211216130001787.png)

### 4.多段接收去掉重发

![image-20211216130424330](readme.assets/image-20211216130424330.png)

![image-20211216130444673](readme.assets/image-20211216130444673.png)

### 5.心跳信号重发去掉

![image-20211216130555241](readme.assets/image-20211216130555241.png)

### 6.获取轴位置重发去掉

![image-20211216130652926](readme.assets/image-20211216130652926.png)

### 7.驱动轴移动到位重发去掉

![image-20211216130746598](readme.assets/image-20211216130746598.png)

### 8.发送读PLC寄存器字符串重发去掉

![image-20211216130926729](readme.assets/image-20211216130926729.png)

### 9.发送写PLC寄存器字符串重发去掉

![image-20211216131135936](readme.assets/image-20211216131135936.png)

# 12月17号已经全部合并了

## 1.添加了新的工具针对拾取工位

![image-20211217213730217](readme.assets/image-20211217213730217.png)

# 12月22号

## 1.拾取工具忘记添加（叶向阳）

![image-20211222093321420](readme.assets/image-20211222093321420.png)

## 2.切换产品地方有bug（叶向阳）

修改当EnableNonFixedCode切换成1时，依然执行FSPC 1 1

![image-20211222093440718](readme.assets/image-20211222093440718.png)

![image-20211222093650201](readme.assets/image-20211222093650201.png)

## 3.修改保存CSV的函数，方便针对不同个数的检测数据，平台选项中添加了检测存CSV的文件头，可以自己配置。

```
<FCHBSaveCSVHead>时间,ID,检测结果,L1,L2,LX,LY,RX,RY</FCHBSaveCSVHead>
```

```
这个函数里面都进行了修改
OnSerialCameraACFInspectCandidateExFCHB_Ex2
```

## 4.几何、区域、点线距离检测工具里面也进行了修改，当不起用找线工具2的时候检测结果1给-2.

```
searchResult.m_resultPosLineDistanceTool.m_nResult1=-2;
```

## 5.添加使用ID存VDB

![image-20211222153955847](readme.assets/image-20211222153955847.png)

# 12月29号

## 1.添加了目标注册功能，以前的FAAL当只进行一次对位的时候，目标对象都拍照，现在加了个checkbox，用来选择是否对目标拍照，针对上料工位

![image-20211229163134998](readme.assets/image-20211229163134998.png)

# 1月5号

## 1.更新了最新的库，现在存压缩的VDB比较快

**工控机**：U8H7

**相机:**1200万相机

软件版本: VisionASM20210810(V1.100.1.0.0.0)合并定制指令版本（视觉库最新12月24）

**存VDB:**非消息存VDB

| VDB类型        | 速度(存2张时间) | VDB大小 |
| -------------- | --------------- | ------- |
| BMP            | 273ms           | 11.6 MB |
| PNG            | 2787ms          | 3.23 MB |
| JEPG*100压缩率 | 381ms           | 3.53 MB |
| JEPG*50压缩率  | 237ms           | 375 KB  |
| JEPG*10压缩率  | 227ms           | 188 KB  |
| JEPG*5压缩率   | 226ms           | 161 KB  |

## 2.界面上添加了控件，设置压缩比例

![image-20220105172021572](readme.assets/image-20220105172021572.png)

## 3.增加了目标拍照存VDB，对象拍照存VDB，防止出现目标拍照存的是上次对象拍照的Mark，增加了存VDB的时候存坐标



# 1月6号

## 将原本的按小时存图修改成按天存图，并修改了发哥说的bug

![image-20220106143313962](readme.assets/image-20220106143313962.png)

![image-20220106143407390](readme.assets/image-20220106143407390.png)



# 1月7号

## 1.更新了uvw平台的问题，固定滑块标定的时候，使用相对偏差回不到起始点的问题，现在使用的是按照绝对偏差量来计算，然后发相对偏差量，龙哥也更新了主界面日志显示的问题，目前已经测试OK

![image-20220111142508836](readme.assets/image-20220111142508836.png)

## 2.更新了UVW使用移动滑块，从xyd转到uvw，再从uvw转xyd出问题，目前研发中心那边已经解决此问题，更新的dll。

![image-20220111142602527](readme.assets/image-20220111142602527.png)

# 1月19号

## 1.通讯界面发送折弯路径的地方之前是不能保存的，修改后可以保存，并且将其保存到产品中，切换产品的时候，参数也会跟着改变。
