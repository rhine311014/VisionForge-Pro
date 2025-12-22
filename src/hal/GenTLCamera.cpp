/**
 * @file GenTLCamera.cpp
 * @brief GenICam Transport Layer 通用相机接口实现
 * @author VisionForge Team
 * @date 2025-12-22
 */

#ifdef USE_GENTL

#include "hal/GenTLCamera.h"
#include "base/Logger.h"
#include "base/ImageMemoryPool.h"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QElapsedTimer>
#include <QThread>
#include <QCoreApplication>

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace VisionForge {
namespace HAL {

// ============================================================
// GenTL 类型定义（基于 GenTL 1.5 规范）
// ============================================================

// GenTL 句柄类型
typedef void* TL_HANDLE;
typedef void* IF_HANDLE;
typedef void* DEV_HANDLE;
typedef void* DS_HANDLE;
typedef void* PORT_HANDLE;
typedef void* BUFFER_HANDLE;
typedef void* EVENTSRC_HANDLE;

// GenTL 错误代码
enum GC_ERROR {
    GC_ERR_SUCCESS = 0,
    GC_ERR_ERROR = -1001,
    GC_ERR_NOT_INITIALIZED = -1002,
    GC_ERR_NOT_IMPLEMENTED = -1003,
    GC_ERR_RESOURCE_IN_USE = -1004,
    GC_ERR_ACCESS_DENIED = -1005,
    GC_ERR_INVALID_HANDLE = -1006,
    GC_ERR_INVALID_ID = -1007,
    GC_ERR_NO_DATA = -1008,
    GC_ERR_INVALID_PARAMETER = -1009,
    GC_ERR_IO = -1010,
    GC_ERR_TIMEOUT = -1011,
    GC_ERR_ABORT = -1012,
    GC_ERR_INVALID_BUFFER = -1013,
    GC_ERR_NOT_AVAILABLE = -1014,
    GC_ERR_INVALID_ADDRESS = -1015,
    GC_ERR_BUFFER_TOO_SMALL = -1016,
    GC_ERR_INVALID_INDEX = -1017,
    GC_ERR_PARSING_CHUNK_DATA = -1018,
    GC_ERR_INVALID_VALUE = -1019,
    GC_ERR_RESOURCE_EXHAUSTED = -1020,
    GC_ERR_OUT_OF_MEMORY = -1021,
    GC_ERR_BUSY = -1022
};

// GenTL 信息命令
enum TL_INFO_CMD {
    TL_INFO_ID = 0,
    TL_INFO_VENDOR = 1,
    TL_INFO_MODEL = 2,
    TL_INFO_VERSION = 3,
    TL_INFO_TLTYPE = 4,
    TL_INFO_NAME = 5,
    TL_INFO_PATHNAME = 6,
    TL_INFO_DISPLAYNAME = 7,
    TL_INFO_GENTL_VER_MAJOR = 8,
    TL_INFO_GENTL_VER_MINOR = 9
};

enum IF_INFO_CMD {
    IF_INFO_ID = 0,
    IF_INFO_DISPLAYNAME = 1,
    IF_INFO_TLTYPE = 2
};

enum DEV_INFO_CMD {
    DEV_INFO_ID = 0,
    DEV_INFO_VENDOR = 1,
    DEV_INFO_MODEL = 2,
    DEV_INFO_TLTYPE = 3,
    DEV_INFO_DISPLAYNAME = 4,
    DEV_INFO_ACCESS_STATUS = 5,
    DEV_INFO_USER_DEFINED_NAME = 6,
    DEV_INFO_SERIAL_NUMBER = 7,
    DEV_INFO_VERSION = 8
};

enum BUFFER_INFO_CMD {
    BUFFER_INFO_BASE = 0,
    BUFFER_INFO_SIZE = 1,
    BUFFER_INFO_USER_PTR = 2,
    BUFFER_INFO_TIMESTAMP = 3,
    BUFFER_INFO_NEW_DATA = 4,
    BUFFER_INFO_IS_QUEUED = 5,
    BUFFER_INFO_IS_ACQUIRING = 6,
    BUFFER_INFO_IS_INCOMPLETE = 7,
    BUFFER_INFO_TLTYPE = 8,
    BUFFER_INFO_SIZE_FILLED = 9,
    BUFFER_INFO_WIDTH = 10,
    BUFFER_INFO_HEIGHT = 11,
    BUFFER_INFO_XOFFSET = 12,
    BUFFER_INFO_YOFFSET = 13,
    BUFFER_INFO_XPADDING = 14,
    BUFFER_INFO_YPADDING = 15,
    BUFFER_INFO_FRAMEID = 16,
    BUFFER_INFO_IMAGEPRESENT = 17,
    BUFFER_INFO_IMAGEOFFSET = 18,
    BUFFER_INFO_PAYLOADTYPE = 19,
    BUFFER_INFO_PIXELFORMAT = 20,
    BUFFER_INFO_PIXELFORMAT_NAMESPACE = 21,
    BUFFER_INFO_DELIVERED_IMAGEHEIGHT = 22,
    BUFFER_INFO_DELIVERED_CHUNKPAYLOADSIZE = 23,
    BUFFER_INFO_CHUNKLAYOUTID = 24,
    BUFFER_INFO_FILENAME = 25,
    BUFFER_INFO_PIXEL_ENDIANNESS = 26,
    BUFFER_INFO_DATA_SIZE = 27,
    BUFFER_INFO_TIMESTAMP_NS = 28
};

// GenTL 像素格式命名空间
enum PIXELFORMAT_NAMESPACE {
    PIXELFORMAT_NAMESPACE_UNKNOWN = 0,
    PIXELFORMAT_NAMESPACE_GEV = 1,
    PIXELFORMAT_NAMESPACE_IIDC = 2,
    PIXELFORMAT_NAMESPACE_PFNC_16BIT = 3,
    PIXELFORMAT_NAMESPACE_PFNC_32BIT = 4
};

// 访问模式
enum DEVICE_ACCESS_FLAGS {
    DEVICE_ACCESS_UNKNOWN = 0,
    DEVICE_ACCESS_NONE = 1,
    DEVICE_ACCESS_READONLY = 2,
    DEVICE_ACCESS_CONTROL = 3,
    DEVICE_ACCESS_EXCLUSIVE = 4
};

// ============================================================
// GenTL Producer 管理器实现
// ============================================================

class GenTLProducerManager::Impl {
public:
    struct LoadedProducer {
        GenTLProducerInfo info;
        void* libraryHandle = nullptr;
        TL_HANDLE tlHandle = nullptr;

        // GenTL 函数指针
        void* GCInitLib = nullptr;
        void* GCCloseLib = nullptr;
        void* TLOpen = nullptr;
        void* TLClose = nullptr;
        void* TLGetInfo = nullptr;
        void* TLGetNumInterfaces = nullptr;
        void* TLGetInterfaceID = nullptr;
        void* TLGetInterfaceInfo = nullptr;
        void* TLOpenInterface = nullptr;
        void* TLUpdateInterfaceList = nullptr;
        void* IFClose = nullptr;
        void* IFGetInfo = nullptr;
        void* IFGetNumDevices = nullptr;
        void* IFGetDeviceID = nullptr;
        void* IFGetDeviceInfo = nullptr;
        void* IFOpenDevice = nullptr;
        void* IFUpdateDeviceList = nullptr;
        void* DevClose = nullptr;
        void* DevGetInfo = nullptr;
        void* DevGetNumDataStreams = nullptr;
        void* DevGetDataStreamID = nullptr;
        void* DevOpenDataStream = nullptr;
        void* DevGetPort = nullptr;
        void* DSClose = nullptr;
        void* DSGetInfo = nullptr;
        void* DSAnnounceBuffer = nullptr;
        void* DSAllocAndAnnounceBuffer = nullptr;
        void* DSRevokeBuffer = nullptr;
        void* DSQueueBuffer = nullptr;
        void* DSFlushQueue = nullptr;
        void* DSStartAcquisition = nullptr;
        void* DSStopAcquisition = nullptr;
        void* DSGetBufferID = nullptr;
        void* DSGetBufferInfo = nullptr;
        void* GCGetNumPortURLs = nullptr;
        void* GCGetPortURLInfo = nullptr;
        void* GCReadPort = nullptr;
        void* GCWritePort = nullptr;
    };

    QList<GenTLProducerInfo> discoveredProducers_;
    QMap<QString, LoadedProducer> loadedProducers_;
    QMutex mutex_;

    QStringList getSearchPaths() const {
        QStringList paths;

        // 1. 环境变量 GENICAM_GENTL64_PATH / GENICAM_GENTL32_PATH
#ifdef Q_OS_WIN
    #if defined(_WIN64) || defined(__x86_64__)
        QString envPath = qEnvironmentVariable("GENICAM_GENTL64_PATH");
    #else
        QString envPath = qEnvironmentVariable("GENICAM_GENTL32_PATH");
    #endif
#else
        QString envPath = qEnvironmentVariable("GENICAM_GENTL64_PATH");
#endif
        if (!envPath.isEmpty()) {
            paths << envPath.split(';', Qt::SkipEmptyParts);
        }

        // 2. 常见安装路径
#ifdef Q_OS_WIN
        paths << "C:/Program Files/MATRIX VISION/mvIMPACT Acquire/bin/x64";
        paths << "C:/Program Files/Basler/pylon 7/Runtime/x64";
        paths << "C:/Program Files/Baumer/Baumer GAPI SDK/Components/bin";
        paths << "C:/Program Files/Allied Vision/Vimba X/VimbaGigETL/Bin/Win64";
        paths << "C:/Program Files/STEMMER IMAGING/Common Vision Blox/GenICam/bin/Win64_x64";
        paths << "C:/Program Files/IDS/ids_peak/generic_x64";
#else
        paths << "/opt/mvIMPACT_Acquire/lib/x86_64";
        paths << "/opt/pylon/lib";
        paths << "/opt/Vimba_X/VimbaGigETL/Bin/Linux64_ARM";
#endif

        // 3. 应用程序目录
        paths << QCoreApplication::applicationDirPath();
        paths << QCoreApplication::applicationDirPath() + "/gentl";

        return paths;
    }

    bool loadLibrary(const QString& path, LoadedProducer& producer) {
#ifdef Q_OS_WIN
        HMODULE handle = LoadLibraryW(reinterpret_cast<LPCWSTR>(path.utf16()));
        if (!handle) {
            LOG_ERROR(QString("加载GenTL Producer失败: %1, 错误码: %2")
                      .arg(path).arg(GetLastError()));
            return false;
        }
        producer.libraryHandle = handle;

        // 获取函数指针
        #define LOAD_FUNC(name) producer.name = GetProcAddress(handle, #name)
#else
        void* handle = dlopen(path.toUtf8().constData(), RTLD_NOW);
        if (!handle) {
            LOG_ERROR(QString("加载GenTL Producer失败: %1, 错误: %2")
                      .arg(path).arg(dlerror()));
            return false;
        }
        producer.libraryHandle = handle;

        #define LOAD_FUNC(name) producer.name = dlsym(handle, #name)
#endif

        LOAD_FUNC(GCInitLib);
        LOAD_FUNC(GCCloseLib);
        LOAD_FUNC(TLOpen);
        LOAD_FUNC(TLClose);
        LOAD_FUNC(TLGetInfo);
        LOAD_FUNC(TLGetNumInterfaces);
        LOAD_FUNC(TLGetInterfaceID);
        LOAD_FUNC(TLGetInterfaceInfo);
        LOAD_FUNC(TLOpenInterface);
        LOAD_FUNC(TLUpdateInterfaceList);
        LOAD_FUNC(IFClose);
        LOAD_FUNC(IFGetInfo);
        LOAD_FUNC(IFGetNumDevices);
        LOAD_FUNC(IFGetDeviceID);
        LOAD_FUNC(IFGetDeviceInfo);
        LOAD_FUNC(IFOpenDevice);
        LOAD_FUNC(IFUpdateDeviceList);
        LOAD_FUNC(DevClose);
        LOAD_FUNC(DevGetInfo);
        LOAD_FUNC(DevGetNumDataStreams);
        LOAD_FUNC(DevGetDataStreamID);
        LOAD_FUNC(DevOpenDataStream);
        LOAD_FUNC(DevGetPort);
        LOAD_FUNC(DSClose);
        LOAD_FUNC(DSGetInfo);
        LOAD_FUNC(DSAnnounceBuffer);
        LOAD_FUNC(DSAllocAndAnnounceBuffer);
        LOAD_FUNC(DSRevokeBuffer);
        LOAD_FUNC(DSQueueBuffer);
        LOAD_FUNC(DSFlushQueue);
        LOAD_FUNC(DSStartAcquisition);
        LOAD_FUNC(DSStopAcquisition);
        LOAD_FUNC(DSGetBufferID);
        LOAD_FUNC(DSGetBufferInfo);
        LOAD_FUNC(GCGetNumPortURLs);
        LOAD_FUNC(GCGetPortURLInfo);
        LOAD_FUNC(GCReadPort);
        LOAD_FUNC(GCWritePort);

        #undef LOAD_FUNC

        // 检查必要函数
        if (!producer.GCInitLib || !producer.TLOpen) {
            LOG_ERROR(QString("GenTL Producer缺少必要函数: %1").arg(path));
            unloadLibrary(producer);
            return false;
        }

        return true;
    }

    void unloadLibrary(LoadedProducer& producer) {
        if (producer.libraryHandle) {
#ifdef Q_OS_WIN
            FreeLibrary(static_cast<HMODULE>(producer.libraryHandle));
#else
            dlclose(producer.libraryHandle);
#endif
            producer.libraryHandle = nullptr;
        }
    }

    QString getInfoString(LoadedProducer& producer, TL_HANDLE handle, int cmd) {
        if (!producer.TLGetInfo) return QString();

        typedef int (*TLGetInfoFunc)(TL_HANDLE, int, int*, void*, size_t*);
        auto func = reinterpret_cast<TLGetInfoFunc>(producer.TLGetInfo);

        size_t size = 0;
        int type = 0;
        if (func(handle, cmd, &type, nullptr, &size) != GC_ERR_SUCCESS) {
            return QString();
        }

        QByteArray buffer(static_cast<int>(size), 0);
        if (func(handle, cmd, &type, buffer.data(), &size) != GC_ERR_SUCCESS) {
            return QString();
        }

        return QString::fromUtf8(buffer.constData());
    }
};

GenTLProducerManager& GenTLProducerManager::instance() {
    static GenTLProducerManager instance;
    return instance;
}

GenTLProducerManager::GenTLProducerManager()
    : impl_(std::make_unique<Impl>())
{
}

GenTLProducerManager::~GenTLProducerManager() {
    unloadAllProducers();
}

int GenTLProducerManager::discoverProducers(const QStringList& additionalPaths) {
    QMutexLocker locker(&impl_->mutex_);

    impl_->discoveredProducers_.clear();

    QStringList searchPaths = impl_->getSearchPaths();
    searchPaths << additionalPaths;

    for (const QString& path : searchPaths) {
        QDir dir(path);
        if (!dir.exists()) continue;

        QStringList filters;
        filters << "*.cti";
        QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

        for (const QFileInfo& fileInfo : files) {
            GenTLProducerInfo info;
            info.filePath = fileInfo.absoluteFilePath();

            // 尝试加载以获取信息
            Impl::LoadedProducer tempProducer;
            if (impl_->loadLibrary(info.filePath, tempProducer)) {
                // 初始化并获取信息
                typedef int (*GCInitLibFunc)();
                auto initFunc = reinterpret_cast<GCInitLibFunc>(tempProducer.GCInitLib);
                if (initFunc && initFunc() == GC_ERR_SUCCESS) {
                    typedef int (*TLOpenFunc)(TL_HANDLE*);
                    auto openFunc = reinterpret_cast<TLOpenFunc>(tempProducer.TLOpen);

                    TL_HANDLE tlHandle = nullptr;
                    if (openFunc && openFunc(&tlHandle) == GC_ERR_SUCCESS) {
                        tempProducer.tlHandle = tlHandle;

                        info.vendorName = impl_->getInfoString(tempProducer, tlHandle, TL_INFO_VENDOR);
                        info.modelName = impl_->getInfoString(tempProducer, tlHandle, TL_INFO_MODEL);
                        info.version = impl_->getInfoString(tempProducer, tlHandle, TL_INFO_VERSION);
                        info.tlType = impl_->getInfoString(tempProducer, tlHandle, TL_INFO_TLTYPE);

                        // 关闭
                        typedef int (*TLCloseFunc)(TL_HANDLE);
                        auto closeFunc = reinterpret_cast<TLCloseFunc>(tempProducer.TLClose);
                        if (closeFunc) closeFunc(tlHandle);
                    }

                    typedef int (*GCCloseLibFunc)();
                    auto closeLibFunc = reinterpret_cast<GCCloseLibFunc>(tempProducer.GCCloseLib);
                    if (closeLibFunc) closeLibFunc();
                }

                impl_->unloadLibrary(tempProducer);
            }

            impl_->discoveredProducers_.append(info);
            LOG_INFO(QString("发现GenTL Producer: %1 (%2)")
                     .arg(info.modelName.isEmpty() ? fileInfo.fileName() : info.modelName)
                     .arg(info.tlType));
        }
    }

    return impl_->discoveredProducers_.size();
}

QList<GenTLProducerInfo> GenTLProducerManager::getProducers() const {
    QMutexLocker locker(&impl_->mutex_);
    return impl_->discoveredProducers_;
}

bool GenTLProducerManager::loadProducer(const QString& ctiPath) {
    QMutexLocker locker(&impl_->mutex_);

    if (impl_->loadedProducers_.contains(ctiPath)) {
        return true;  // 已加载
    }

    Impl::LoadedProducer producer;
    if (!impl_->loadLibrary(ctiPath, producer)) {
        return false;
    }

    // 初始化GenTL库
    typedef int (*GCInitLibFunc)();
    auto initFunc = reinterpret_cast<GCInitLibFunc>(producer.GCInitLib);
    if (!initFunc || initFunc() != GC_ERR_SUCCESS) {
        LOG_ERROR(QString("初始化GenTL库失败: %1").arg(ctiPath));
        impl_->unloadLibrary(producer);
        return false;
    }

    // 打开传输层
    typedef int (*TLOpenFunc)(TL_HANDLE*);
    auto openFunc = reinterpret_cast<TLOpenFunc>(producer.TLOpen);
    if (!openFunc || openFunc(&producer.tlHandle) != GC_ERR_SUCCESS) {
        LOG_ERROR(QString("打开GenTL传输层失败: %1").arg(ctiPath));

        typedef int (*GCCloseLibFunc)();
        auto closeLibFunc = reinterpret_cast<GCCloseLibFunc>(producer.GCCloseLib);
        if (closeLibFunc) closeLibFunc();

        impl_->unloadLibrary(producer);
        return false;
    }

    // 获取Producer信息
    producer.info.filePath = ctiPath;
    producer.info.vendorName = impl_->getInfoString(producer, producer.tlHandle, TL_INFO_VENDOR);
    producer.info.modelName = impl_->getInfoString(producer, producer.tlHandle, TL_INFO_MODEL);
    producer.info.version = impl_->getInfoString(producer, producer.tlHandle, TL_INFO_VERSION);
    producer.info.tlType = impl_->getInfoString(producer, producer.tlHandle, TL_INFO_TLTYPE);
    producer.info.isLoaded = true;

    impl_->loadedProducers_[ctiPath] = producer;

    LOG_INFO(QString("已加载GenTL Producer: %1 v%2 (%3)")
             .arg(producer.info.modelName)
             .arg(producer.info.version)
             .arg(producer.info.tlType));

    return true;
}

void GenTLProducerManager::unloadProducer(const QString& ctiPath) {
    QMutexLocker locker(&impl_->mutex_);

    if (!impl_->loadedProducers_.contains(ctiPath)) {
        return;
    }

    auto& producer = impl_->loadedProducers_[ctiPath];

    // 关闭传输层
    if (producer.tlHandle) {
        typedef int (*TLCloseFunc)(TL_HANDLE);
        auto closeFunc = reinterpret_cast<TLCloseFunc>(producer.TLClose);
        if (closeFunc) closeFunc(producer.tlHandle);
        producer.tlHandle = nullptr;
    }

    // 关闭GenTL库
    typedef int (*GCCloseLibFunc)();
    auto closeLibFunc = reinterpret_cast<GCCloseLibFunc>(producer.GCCloseLib);
    if (closeLibFunc) closeLibFunc();

    impl_->unloadLibrary(producer);
    impl_->loadedProducers_.remove(ctiPath);

    LOG_INFO(QString("已卸载GenTL Producer: %1").arg(ctiPath));
}

void GenTLProducerManager::unloadAllProducers() {
    QMutexLocker locker(&impl_->mutex_);

    QStringList paths = impl_->loadedProducers_.keys();
    locker.unlock();

    for (const QString& path : paths) {
        unloadProducer(path);
    }
}

QList<GenTLDeviceInfo> GenTLProducerManager::enumerateDevices() {
    QMutexLocker locker(&impl_->mutex_);

    QList<GenTLDeviceInfo> devices;

    for (auto& producer : impl_->loadedProducers_) {
        if (!producer.tlHandle) continue;

        // 更新接口列表
        typedef int (*TLUpdateInterfaceListFunc)(TL_HANDLE, bool*, uint64_t);
        auto updateFunc = reinterpret_cast<TLUpdateInterfaceListFunc>(producer.TLUpdateInterfaceList);
        if (updateFunc) {
            bool changed = false;
            updateFunc(producer.tlHandle, &changed, 1000);
        }

        // 获取接口数量
        typedef int (*TLGetNumInterfacesFunc)(TL_HANDLE, uint32_t*);
        auto getNumFunc = reinterpret_cast<TLGetNumInterfacesFunc>(producer.TLGetNumInterfaces);
        if (!getNumFunc) continue;

        uint32_t numInterfaces = 0;
        if (getNumFunc(producer.tlHandle, &numInterfaces) != GC_ERR_SUCCESS) continue;

        // 遍历接口
        typedef int (*TLGetInterfaceIDFunc)(TL_HANDLE, uint32_t, char*, size_t*);
        typedef int (*TLOpenInterfaceFunc)(TL_HANDLE, const char*, IF_HANDLE*);
        typedef int (*IFUpdateDeviceListFunc)(IF_HANDLE, bool*, uint64_t);
        typedef int (*IFGetNumDevicesFunc)(IF_HANDLE, uint32_t*);
        typedef int (*IFGetDeviceIDFunc)(IF_HANDLE, uint32_t, char*, size_t*);
        typedef int (*IFGetDeviceInfoFunc)(IF_HANDLE, const char*, int, int*, void*, size_t*);
        typedef int (*IFCloseFunc)(IF_HANDLE);

        auto getInterfaceIDFunc = reinterpret_cast<TLGetInterfaceIDFunc>(producer.TLGetInterfaceID);
        auto openInterfaceFunc = reinterpret_cast<TLOpenInterfaceFunc>(producer.TLOpenInterface);
        auto updateDeviceListFunc = reinterpret_cast<IFUpdateDeviceListFunc>(producer.IFUpdateDeviceList);
        auto getNumDevicesFunc = reinterpret_cast<IFGetNumDevicesFunc>(producer.IFGetNumDevices);
        auto getDeviceIDFunc = reinterpret_cast<IFGetDeviceIDFunc>(producer.IFGetDeviceID);
        auto getDeviceInfoFunc = reinterpret_cast<IFGetDeviceInfoFunc>(producer.IFGetDeviceInfo);
        auto closeInterfaceFunc = reinterpret_cast<IFCloseFunc>(producer.IFClose);

        for (uint32_t i = 0; i < numInterfaces; ++i) {
            // 获取接口ID
            size_t idSize = 0;
            if (getInterfaceIDFunc(producer.tlHandle, i, nullptr, &idSize) != GC_ERR_SUCCESS) continue;

            QByteArray interfaceId(static_cast<int>(idSize), 0);
            if (getInterfaceIDFunc(producer.tlHandle, i, interfaceId.data(), &idSize) != GC_ERR_SUCCESS) continue;

            // 打开接口
            IF_HANDLE ifHandle = nullptr;
            if (openInterfaceFunc(producer.tlHandle, interfaceId.constData(), &ifHandle) != GC_ERR_SUCCESS) continue;

            // 更新设备列表
            if (updateDeviceListFunc) {
                bool changed = false;
                updateDeviceListFunc(ifHandle, &changed, 1000);
            }

            // 获取设备数量
            uint32_t numDevices = 0;
            if (getNumDevicesFunc(ifHandle, &numDevices) != GC_ERR_SUCCESS) {
                closeInterfaceFunc(ifHandle);
                continue;
            }

            // 遍历设备
            for (uint32_t j = 0; j < numDevices; ++j) {
                // 获取设备ID
                size_t devIdSize = 0;
                if (getDeviceIDFunc(ifHandle, j, nullptr, &devIdSize) != GC_ERR_SUCCESS) continue;

                QByteArray deviceIdBuf(static_cast<int>(devIdSize), 0);
                if (getDeviceIDFunc(ifHandle, j, deviceIdBuf.data(), &devIdSize) != GC_ERR_SUCCESS) continue;

                GenTLDeviceInfo devInfo;
                devInfo.deviceId = QString::fromUtf8(deviceIdBuf.constData());
                devInfo.interfaceId = QString::fromUtf8(interfaceId.constData());
                devInfo.producerPath = producer.info.filePath;
                devInfo.tlType = producer.info.tlType;

                // 获取设备详细信息
                auto getStringInfo = [&](int cmd) -> QString {
                    size_t size = 0;
                    int type = 0;
                    if (getDeviceInfoFunc(ifHandle, deviceIdBuf.constData(), cmd, &type, nullptr, &size) != GC_ERR_SUCCESS) {
                        return QString();
                    }
                    QByteArray buf(static_cast<int>(size), 0);
                    if (getDeviceInfoFunc(ifHandle, deviceIdBuf.constData(), cmd, &type, buf.data(), &size) != GC_ERR_SUCCESS) {
                        return QString();
                    }
                    return QString::fromUtf8(buf.constData());
                };

                devInfo.vendorName = getStringInfo(DEV_INFO_VENDOR);
                devInfo.modelName = getStringInfo(DEV_INFO_MODEL);
                devInfo.serialNumber = getStringInfo(DEV_INFO_SERIAL_NUMBER);
                devInfo.userDefinedName = getStringInfo(DEV_INFO_USER_DEFINED_NAME);

                // 获取访问状态
                size_t accessSize = sizeof(int32_t);
                int32_t accessStatus = 0;
                int type = 0;
                if (getDeviceInfoFunc(ifHandle, deviceIdBuf.constData(), DEV_INFO_ACCESS_STATUS,
                                     &type, &accessStatus, &accessSize) == GC_ERR_SUCCESS) {
                    switch (accessStatus) {
                    case DEVICE_ACCESS_READONLY:
                        devInfo.accessStatus = "ReadOnly";
                        break;
                    case DEVICE_ACCESS_CONTROL:
                    case DEVICE_ACCESS_EXCLUSIVE:
                        devInfo.accessStatus = "ReadWrite";
                        break;
                    default:
                        devInfo.accessStatus = "Unknown";
                        break;
                    }
                }

                devices.append(devInfo);
            }

            closeInterfaceFunc(ifHandle);
        }
    }

    return devices;
}

QString GenTLProducerManager::getGenTLVersion() const {
    return "GenTL 1.5";
}

void GenTLProducerManager::setEnvironmentPath(const QString& path) {
#ifdef Q_OS_WIN
    #if defined(_WIN64) || defined(__x86_64__)
        qputenv("GENICAM_GENTL64_PATH", path.toUtf8());
    #else
        qputenv("GENICAM_GENTL32_PATH", path.toUtf8());
    #endif
#else
    qputenv("GENICAM_GENTL64_PATH", path.toUtf8());
#endif
}

// ============================================================
// GenTLCamera 实现
// ============================================================

class GenTLCamera::Impl {
public:
    // GenTL 句柄
    TL_HANDLE tlHandle_ = nullptr;
    IF_HANDLE ifHandle_ = nullptr;
    DEV_HANDLE devHandle_ = nullptr;
    DS_HANDLE dsHandle_ = nullptr;
    PORT_HANDLE portHandle_ = nullptr;

    // 缓冲区
    struct BufferInfo {
        BUFFER_HANDLE handle = nullptr;
        void* data = nullptr;
        size_t size = 0;
    };
    QList<BufferInfo> buffers_;
    BufferConfig bufferConfig_;

    // Producer 引用
    QString producerPath_;

    // 采集线程
    std::atomic<bool> grabbing_{false};

    // 统计
    StreamStatistics stats_;
    QElapsedTimer statsTimer_;

    // GenICam 节点缓存
    mutable QMap<QString, GenICamNodeInfo> nodeCache_;
    mutable bool nodeCacheValid_ = false;

    Impl() {
        statsTimer_.start();
    }
};

// 标准GenICam节点名映射
const QMap<QString, QString>& GenTLCamera::getStandardNodeNames() {
    static QMap<QString, QString> names = {
        // 图像格式
        {"Width", "Width"},
        {"Height", "Height"},
        {"OffsetX", "OffsetX"},
        {"OffsetY", "OffsetY"},
        {"PixelFormat", "PixelFormat"},

        // 采集控制
        {"AcquisitionMode", "AcquisitionMode"},
        {"AcquisitionStart", "AcquisitionStart"},
        {"AcquisitionStop", "AcquisitionStop"},
        {"AcquisitionFrameRate", "AcquisitionFrameRate"},
        {"AcquisitionFrameRateEnable", "AcquisitionFrameRateEnable"},

        // 触发
        {"TriggerMode", "TriggerMode"},
        {"TriggerSource", "TriggerSource"},
        {"TriggerActivation", "TriggerActivation"},
        {"TriggerSoftware", "TriggerSoftware"},

        // 曝光
        {"ExposureTime", "ExposureTime"},
        {"ExposureAuto", "ExposureAuto"},
        {"ExposureMode", "ExposureMode"},

        // 增益
        {"Gain", "Gain"},
        {"GainAuto", "GainAuto"},

        // 白平衡
        {"BalanceWhiteAuto", "BalanceWhiteAuto"},
        {"BalanceRatioSelector", "BalanceRatioSelector"},
        {"BalanceRatio", "BalanceRatio"},

        // Action Command (GigE)
        {"ActionDeviceKey", "ActionDeviceKey"},
        {"ActionGroupKey", "ActionGroupKey"},
        {"ActionGroupMask", "ActionGroupMask"},

        // 设备信息
        {"DeviceVendorName", "DeviceVendorName"},
        {"DeviceModelName", "DeviceModelName"},
        {"DeviceSerialNumber", "DeviceSerialNumber"},
        {"DeviceVersion", "DeviceVersion"},
        {"DeviceFirmwareVersion", "DeviceFirmwareVersion"},
        {"DeviceUserID", "DeviceUserID"},

        // GigE 特有
        {"GevSCPSPacketSize", "GevSCPSPacketSize"},
        {"GevSCPD", "GevSCPD"},
        {"GevHeartbeatTimeout", "GevHeartbeatTimeout"},

        // 用户设置
        {"UserSetSelector", "UserSetSelector"},
        {"UserSetLoad", "UserSetLoad"},
        {"UserSetSave", "UserSetSave"},
        {"UserSetDefault", "UserSetDefault"}
    };
    return names;
}

GenTLCamera::GenTLCamera(QObject* parent)
    : ICamera(parent)
    , impl_(std::make_unique<Impl>())
{
    LOG_INFO("创建GenTL通用相机实例");
}

GenTLCamera::~GenTLCamera() {
    close();
    LOG_INFO("销毁GenTL通用相机实例");
}

QList<GenTLDeviceInfo> GenTLCamera::enumerateDevices(bool refreshProducers) {
    auto& manager = GenTLProducerManager::instance();

    if (refreshProducers || manager.getProducers().isEmpty()) {
        manager.discoverProducers();

        // 加载所有发现的Producer
        for (const auto& producer : manager.getProducers()) {
            manager.loadProducer(producer.filePath);
        }
    }

    return manager.enumerateDevices();
}

QString GenTLCamera::sdkVersion() {
    return GenTLProducerManager::instance().getGenTLVersion();
}

QStringList GenTLCamera::supportedTransportLayers() {
    return QStringList() << "GigE" << "USB3" << "CameraLink" << "CoaXPress" << "Custom";
}

void GenTLCamera::selectDevice(int index) {
    auto devices = enumerateDevices();
    if (index >= 0 && index < devices.size()) {
        deviceInfo_ = devices[index];
        selectedDeviceIndex_ = index;
        LOG_INFO(QString("选择设备[%1]: %2 (%3)")
                 .arg(index)
                 .arg(deviceInfo_.modelName)
                 .arg(deviceInfo_.serialNumber));
    }
}

bool GenTLCamera::selectByDeviceId(const QString& deviceId) {
    auto devices = enumerateDevices();
    for (int i = 0; i < devices.size(); ++i) {
        if (devices[i].deviceId == deviceId) {
            selectDevice(i);
            return true;
        }
    }
    setLastError(QString("未找到设备ID: %1").arg(deviceId));
    return false;
}

bool GenTLCamera::selectBySerialNumber(const QString& serialNumber) {
    auto devices = enumerateDevices();
    for (int i = 0; i < devices.size(); ++i) {
        if (devices[i].serialNumber == serialNumber) {
            selectDevice(i);
            return true;
        }
    }
    setLastError(QString("未找到序列号: %1").arg(serialNumber));
    return false;
}

bool GenTLCamera::selectByIP(const QString& ipAddress) {
    auto devices = enumerateDevices();
    for (int i = 0; i < devices.size(); ++i) {
        if (devices[i].ipAddress == ipAddress) {
            selectDevice(i);
            return true;
        }
    }
    setLastError(QString("未找到IP地址: %1").arg(ipAddress));
    return false;
}

bool GenTLCamera::selectByUserName(const QString& userName) {
    auto devices = enumerateDevices();
    for (int i = 0; i < devices.size(); ++i) {
        if (devices[i].userDefinedName == userName) {
            selectDevice(i);
            return true;
        }
    }
    setLastError(QString("未找到用户名: %1").arg(userName));
    return false;
}

bool GenTLCamera::open() {
    QMutexLocker locker(&mutex_);

    if (isOpen_) {
        return true;
    }

    if (selectedDeviceIndex_ < 0) {
        setLastError("未选择设备");
        return false;
    }

    LOG_INFO(QString("打开GenTL相机: %1").arg(deviceInfo_.modelName));

    // 加载Producer
    auto& manager = GenTLProducerManager::instance();
    if (!manager.loadProducer(deviceInfo_.producerPath)) {
        setLastError("加载GenTL Producer失败");
        return false;
    }

    impl_->producerPath_ = deviceInfo_.producerPath;

    // 这里应该调用GenTL API打开设备
    // 由于实际的GenTL API调用需要完整的SDK，这里提供框架
    // TODO: 实现实际的设备打开逻辑

    isOpen_ = true;
    LOG_INFO(QString("GenTL相机已打开: %1 (%2)")
             .arg(deviceInfo_.modelName)
             .arg(deviceInfo_.serialNumber));

    return true;
}

void GenTLCamera::close() {
    QMutexLocker locker(&mutex_);

    if (!isOpen_) {
        return;
    }

    stopGrabbing();

    // 关闭数据流
    if (impl_->dsHandle_) {
        // DSClose(impl_->dsHandle_);
        impl_->dsHandle_ = nullptr;
    }

    // 关闭设备
    if (impl_->devHandle_) {
        // DevClose(impl_->devHandle_);
        impl_->devHandle_ = nullptr;
    }

    // 关闭接口
    if (impl_->ifHandle_) {
        // IFClose(impl_->ifHandle_);
        impl_->ifHandle_ = nullptr;
    }

    impl_->tlHandle_ = nullptr;

    isOpen_ = false;
    LOG_INFO("GenTL相机已关闭");
}

bool GenTLCamera::isOpen() const {
    return isOpen_;
}

QString GenTLCamera::deviceName() const {
    return deviceInfo_.modelName;
}

QString GenTLCamera::serialNumber() const {
    return deviceInfo_.serialNumber;
}

bool GenTLCamera::startGrabbing() {
    QMutexLocker locker(&mutex_);

    if (!isOpen_) {
        setLastError("相机未打开");
        return false;
    }

    if (isGrabbing_) {
        return true;
    }

    LOG_INFO("开始GenTL相机采集");

    // 配置缓冲区
    if (!allocateBuffers()) {
        setLastError("分配缓冲区失败");
        return false;
    }

    // 启动采集
    impl_->grabbing_ = true;
    isGrabbing_ = true;

    // TODO: 调用实际的DSStartAcquisition

    impl_->stats_ = StreamStatistics();
    impl_->statsTimer_.restart();

    return true;
}

void GenTLCamera::stopGrabbing() {
    QMutexLocker locker(&mutex_);

    if (!isGrabbing_) {
        return;
    }

    LOG_INFO("停止GenTL相机采集");

    impl_->grabbing_ = false;

    // TODO: 调用实际的DSStopAcquisition

    freeBuffers();
    isGrabbing_ = false;
}

bool GenTLCamera::isGrabbing() const {
    return isGrabbing_;
}

Base::ImageData::Ptr GenTLCamera::grabImage(int timeoutMs) {
    if (!isGrabbing_) {
        return nullptr;
    }

    QMutexLocker locker(&mutex_);

    // TODO: 实际实现需要调用GenTL的DSGetFilledBuffer等API
    // 这里提供框架实现

    // 模拟采集延迟
    QThread::msleep(qMin(timeoutMs, 100));

    // 创建测试图像 (使用OpenCV格式 CV_8UC1)
    auto image = std::make_shared<Base::ImageData>(config_.width, config_.height, CV_8UC1);

    // 获取底层cv::Mat并填充测试数据（棋盘图案）
    cv::Mat& mat = image->mat();
    for (int y = 0; y < mat.rows; ++y) {
        uchar* row = mat.ptr<uchar>(y);
        for (int x = 0; x < mat.cols; ++x) {
            bool white = ((x / 64) + (y / 64)) % 2 == 0;
            row[x] = white ? 200 : 55;
        }
    }

    // 更新统计
    impl_->stats_.deliveredFrameCount++;
    double elapsedSec = impl_->statsTimer_.elapsed() / 1000.0;
    if (elapsedSec > 0) {
        impl_->stats_.frameRate = impl_->stats_.deliveredFrameCount / elapsedSec;
    }

    emit imageGrabbed(image);
    return image;
}

bool GenTLCamera::trigger() {
    if (!isOpen_) {
        return false;
    }

    return executeCommand("TriggerSoftware");
}

void GenTLCamera::setConfig(const Config& config) {
    config_ = config;

    if (isOpen_) {
        setFeature("Width", config.width);
        setFeature("Height", config.height);
        setFeature("OffsetX", config.offsetX);
        setFeature("OffsetY", config.offsetY);
        setExposure(config.exposure);
        setGain(config.gain);
        setTriggerMode(config.triggerMode);
    }
}

ICamera::Config GenTLCamera::getConfig() const {
    return config_;
}

void GenTLCamera::setExposure(double exposure) {
    config_.exposure = exposure;
    if (isOpen_) {
        setFeature("ExposureTime", exposure);
    }
}

double GenTLCamera::getExposure() const {
    if (isOpen_) {
        QVariant val = getFeature("ExposureTime");
        if (val.isValid()) {
            return val.toDouble();
        }
    }
    return config_.exposure;
}

void GenTLCamera::setGain(double gain) {
    config_.gain = gain;
    if (isOpen_) {
        setFeature("Gain", gain);
    }
}

double GenTLCamera::getGain() const {
    if (isOpen_) {
        QVariant val = getFeature("Gain");
        if (val.isValid()) {
            return val.toDouble();
        }
    }
    return config_.gain;
}

void GenTLCamera::setTriggerMode(TriggerMode mode) {
    config_.triggerMode = mode;

    if (isOpen_) {
        switch (mode) {
        case Continuous:
            setFeature("TriggerMode", "Off");
            break;
        case Software:
            setFeature("TriggerMode", "On");
            setFeature("TriggerSource", "Software");
            break;
        case Hardware:
            setFeature("TriggerMode", "On");
            setFeature("TriggerSource", "Line1");
            break;
        }
    }
}

ICamera::TriggerMode GenTLCamera::getTriggerMode() const {
    return config_.triggerMode;
}

void GenTLCamera::setTriggerModeEx(TriggerModeEx mode) {
    syncConfig_.triggerMode = mode;

    switch (mode) {
    case TriggerModeEx::Continuous:
        setTriggerMode(Continuous);
        break;
    case TriggerModeEx::Software:
        setTriggerMode(Software);
        break;
    case TriggerModeEx::Hardware:
    case TriggerModeEx::ExternalGPIO:
        setTriggerMode(Hardware);
        setFeature("TriggerSource", "Line1");
        break;
    case TriggerModeEx::ActionCommand:
        setTriggerMode(Hardware);
        setFeature("TriggerSource", "Action1");
        break;
    }
}

TriggerModeEx GenTLCamera::getTriggerModeEx() const {
    return syncConfig_.triggerMode;
}

bool GenTLCamera::configureActionCommand(const ActionCommandConfig& config) {
    if (!isOpen_ || !supportsActionCommand()) {
        return false;
    }

    actionConfig_ = config;

    bool success = true;
    success &= setFeature("ActionDeviceKey", static_cast<qint64>(config.deviceKey));
    success &= setFeature("ActionGroupKey", static_cast<qint64>(config.groupKey));
    success &= setFeature("ActionGroupMask", static_cast<qint64>(config.groupMask));

    return success;
}

ActionCommandConfig GenTLCamera::getActionCommandConfig() const {
    return actionConfig_;
}

bool GenTLCamera::supportsActionCommand() const {
    return isGigECamera() && isFeatureAvailable("ActionDeviceKey");
}

bool GenTLCamera::configureHardwareSync(const HardwareSyncConfig& config) {
    syncConfig_ = config;
    setTriggerModeEx(config.triggerMode);

    if (config.triggerMode == TriggerModeEx::ActionCommand) {
        return configureActionCommand(config.actionCmd);
    }

    return true;
}

HardwareSyncConfig GenTLCamera::getHardwareSyncConfig() const {
    return syncConfig_;
}

bool GenTLCamera::supportsHardwareSync() const {
    return isFeatureAvailable("TriggerMode");
}

bool GenTLCamera::waitForActionTrigger(int timeoutMs) {
    Q_UNUSED(timeoutMs);
    // TODO: 实现Action Command等待逻辑
    return false;
}

bool GenTLCamera::setFeature(const QString& featureName, const QVariant& value) {
    if (!isOpen_) {
        return false;
    }

    QString stdName = mapToStandardName(featureName);
    LOG_DEBUG(QString("设置GenICam特性: %1 = %2").arg(stdName).arg(value.toString()));

    // TODO: 实现实际的GenICam节点写入
    // 通过GenTL的GCWritePort或GenApi实现

    // 使缓存失效
    impl_->nodeCacheValid_ = false;

    return true;
}

QVariant GenTLCamera::getFeature(const QString& featureName) const {
    if (!isOpen_) {
        return QVariant();
    }

    QString stdName = mapToStandardName(featureName);

    // TODO: 实现实际的GenICam节点读取
    // 通过GenTL的GCReadPort或GenApi实现

    return QVariant();
}

GenICamNodeInfo GenTLCamera::getFeatureInfo(const QString& featureName) const {
    QString stdName = mapToStandardName(featureName);

    // 检查缓存
    if (impl_->nodeCacheValid_ && impl_->nodeCache_.contains(stdName)) {
        return impl_->nodeCache_[stdName];
    }

    GenICamNodeInfo info;
    info.name = stdName;

    // TODO: 通过GenApi获取节点详细信息

    impl_->nodeCache_[stdName] = info;
    return info;
}

QStringList GenTLCamera::enumerateFeatures(const QString& category) const {
    Q_UNUSED(category);

    // 返回标准GenICam特性列表
    return getStandardNodeNames().keys();
}

bool GenTLCamera::executeCommand(const QString& commandName) {
    if (!isOpen_) {
        return false;
    }

    LOG_DEBUG(QString("执行GenICam命令: %1").arg(commandName));

    // TODO: 实现实际的命令执行

    return true;
}

bool GenTLCamera::isFeatureAvailable(const QString& featureName) const {
    auto info = getFeatureInfo(featureName);
    return info.isImplemented && info.isAvailable;
}

bool GenTLCamera::isFeatureWritable(const QString& featureName) const {
    auto info = getFeatureInfo(featureName);
    return info.isImplemented && info.isAvailable && info.isWritable;
}

void GenTLCamera::getExposureRange(double& minVal, double& maxVal) const {
    auto info = getFeatureInfo("ExposureTime");
    minVal = info.minValue;
    maxVal = info.maxValue;

    // 默认范围
    if (minVal == 0 && maxVal == 0) {
        minVal = 10.0;
        maxVal = 10000000.0;
    }
}

void GenTLCamera::getGainRange(double& minVal, double& maxVal) const {
    auto info = getFeatureInfo("Gain");
    minVal = info.minValue;
    maxVal = info.maxValue;

    if (minVal == 0 && maxVal == 0) {
        minVal = 0.0;
        maxVal = 48.0;
    }
}

void GenTLCamera::getWidthRange(int& minVal, int& maxVal) const {
    auto info = getFeatureInfo("Width");
    minVal = static_cast<int>(info.minValue);
    maxVal = static_cast<int>(info.maxValue);

    if (minVal == 0 && maxVal == 0) {
        minVal = 64;
        maxVal = 4096;
    }
}

void GenTLCamera::getHeightRange(int& minVal, int& maxVal) const {
    auto info = getFeatureInfo("Height");
    minVal = static_cast<int>(info.minValue);
    maxVal = static_cast<int>(info.maxValue);

    if (minVal == 0 && maxVal == 0) {
        minVal = 64;
        maxVal = 4096;
    }
}

void GenTLCamera::getFrameRateRange(double& minVal, double& maxVal) const {
    auto info = getFeatureInfo("AcquisitionFrameRate");
    minVal = info.minValue;
    maxVal = info.maxValue;

    if (minVal == 0 && maxVal == 0) {
        minVal = 0.1;
        maxVal = 1000.0;
    }
}

bool GenTLCamera::setROI(int offsetX, int offsetY, int width, int height) {
    if (!isOpen_) {
        return false;
    }

    bool success = true;
    success &= setFeature("OffsetX", 0);  // 先重置偏移
    success &= setFeature("OffsetY", 0);
    success &= setFeature("Width", width);
    success &= setFeature("Height", height);
    success &= setFeature("OffsetX", offsetX);
    success &= setFeature("OffsetY", offsetY);

    if (success) {
        config_.width = width;
        config_.height = height;
        config_.offsetX = offsetX;
        config_.offsetY = offsetY;
    }

    return success;
}

bool GenTLCamera::resetROI() {
    int minW, maxW, minH, maxH;
    getWidthRange(minW, maxW);
    getHeightRange(minH, maxH);
    return setROI(0, 0, maxW, maxH);
}

bool GenTLCamera::configureBuffers(const BufferConfig& config) {
    impl_->bufferConfig_ = config;
    return true;
}

StreamStatistics GenTLCamera::getStreamStatistics() const {
    return impl_->stats_;
}

void GenTLCamera::resetStreamStatistics() {
    impl_->stats_ = StreamStatistics();
    impl_->statsTimer_.restart();
}

GenTLDeviceInfo GenTLCamera::getDeviceInfo() const {
    return deviceInfo_;
}

QString GenTLCamera::getTransportLayerType() const {
    return deviceInfo_.tlType;
}

bool GenTLCamera::isGigECamera() const {
    return deviceInfo_.tlType.contains("GigE", Qt::CaseInsensitive) ||
           deviceInfo_.tlType.contains("GEV", Qt::CaseInsensitive);
}

bool GenTLCamera::isUSB3Camera() const {
    return deviceInfo_.tlType.contains("USB", Qt::CaseInsensitive) ||
           deviceInfo_.tlType.contains("U3V", Qt::CaseInsensitive);
}

bool GenTLCamera::configureGigENetwork(int packetSize, int interPacketDelay) {
    if (!isGigECamera()) {
        return false;
    }

    bool success = true;
    success &= setFeature("GevSCPSPacketSize", packetSize);
    success &= setFeature("GevSCPD", interPacketDelay);
    return success;
}

QMap<QString, QVariant> GenTLCamera::getGigEStatistics() const {
    QMap<QString, QVariant> stats;

    if (isGigECamera()) {
        stats["PacketSize"] = getFeature("GevSCPSPacketSize");
        stats["InterPacketDelay"] = getFeature("GevSCPD");
        stats["HeartbeatTimeout"] = getFeature("GevHeartbeatTimeout");
    }

    return stats;
}

bool GenTLCamera::setHeartbeatTimeout(int timeoutMs) {
    if (!isGigECamera()) {
        return false;
    }
    return setFeature("GevHeartbeatTimeout", timeoutMs);
}

bool GenTLCamera::saveUserSet(int setIndex) {
    if (!isOpen_) {
        return false;
    }

    setFeature("UserSetSelector", QString("UserSet%1").arg(setIndex));
    return executeCommand("UserSetSave");
}

bool GenTLCamera::loadUserSet(int setIndex) {
    if (!isOpen_) {
        return false;
    }

    setFeature("UserSetSelector", QString("UserSet%1").arg(setIndex));
    return executeCommand("UserSetLoad");
}

bool GenTLCamera::setDefaultUserSet(int setIndex) {
    if (!isOpen_) {
        return false;
    }

    return setFeature("UserSetDefault", QString("UserSet%1").arg(setIndex));
}

QString GenTLCamera::getLastError() const {
    return lastError_;
}

int GenTLCamera::getLastErrorCode() const {
    return lastErrorCode_;
}

QString GenTLCamera::mapToStandardName(const QString& name) const {
    const auto& stdNames = getStandardNodeNames();
    if (stdNames.contains(name)) {
        return stdNames[name];
    }
    return name;
}

bool GenTLCamera::allocateBuffers() {
    int count = impl_->bufferConfig_.bufferCount;
    if (count <= 0) count = 10;

    // TODO: 实际分配GenTL缓冲区

    return true;
}

void GenTLCamera::freeBuffers() {
    // TODO: 释放GenTL缓冲区
    impl_->buffers_.clear();
}

void GenTLCamera::setLastError(const QString& error, int code) {
    lastError_ = error;
    lastErrorCode_ = code;
    LOG_ERROR(QString("GenTL错误[%1]: %2").arg(code).arg(error));
    emit errorOccurred(error);
}

} // namespace HAL
} // namespace VisionForge

#endif // USE_GENTL
