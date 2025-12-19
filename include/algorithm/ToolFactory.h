/**
 * @file ToolFactory.h
 * @brief 视觉工具工厂
 * @details 使用单例模式和工厂模式，动态创建和管理视觉工具
 */

#ifndef VISIONFORGE_ALGORITHM_TOOLFACTORY_H
#define VISIONFORGE_ALGORITHM_TOOLFACTORY_H

#include "algorithm/VisionTool.h"
#include <QMap>
#include <QString>
#include <QList>
#include <QCoreApplication>
#include <functional>

namespace VisionForge {
namespace Algorithm {

/**
 * @class ToolFactory
 * @brief 工具工厂类
 * @details 负责注册、创建和管理所有视觉工具
 */
class ToolFactory
{
    Q_DECLARE_TR_FUNCTIONS(ToolFactory)

public:
    /**
     * @brief 工具信息结构
     */
    struct ToolInfo {
        QString name;                   ///< 工具名称
        QString category;               ///< 工具分类（如"图像预处理"）
        QString description;            ///< 工具描述
        QString iconPath;               ///< 图标路径
        VisionTool::ToolType type;      ///< 工具类型枚举

        ToolInfo() : type(VisionTool::Custom) {}

        ToolInfo(const QString& n, const QString& cat, const QString& desc,
                 const QString& icon, VisionTool::ToolType t)
            : name(n)
            , category(cat)
            , description(desc)
            , iconPath(icon)
            , type(t)
        {}
    };

    /**
     * @brief 工具创建器函数类型
     */
    using ToolCreator = std::function<VisionTool*()>;

    /**
     * @brief 获取工厂单例
     */
    static ToolFactory& instance();

    /**
     * @brief 注册工具
     * @param type 工具类型
     * @param info 工具信息
     * @param creator 工具创建函数
     */
    void registerTool(VisionTool::ToolType type,
                     const ToolInfo& info,
                     ToolCreator creator);

    /**
     * @brief 根据类型创建工具
     * @param type 工具类型
     * @return 工具实例指针（调用者负责释放）
     */
    VisionTool* createTool(VisionTool::ToolType type);

    /**
     * @brief 根据名称创建工具
     * @param name 工具名称
     * @return 工具实例指针（调用者负责释放）
     */
    VisionTool* createTool(const QString& name);

    /**
     * @brief 获取所有已注册的工具信息
     */
    QList<ToolInfo> getAllTools() const;

    /**
     * @brief 根据分类获取工具信息
     * @param category 工具分类
     */
    QList<ToolInfo> getToolsByCategory(const QString& category) const;

    /**
     * @brief 获取工具信息
     * @param type 工具类型
     */
    ToolInfo getToolInfo(VisionTool::ToolType type) const;

    /**
     * @brief 检查工具是否已注册
     * @param type 工具类型
     */
    bool isRegistered(VisionTool::ToolType type) const;

    /**
     * @brief 获取所有工具分类
     */
    QStringList getAllCategories() const;

private:
    ToolFactory();
    ~ToolFactory();

    // 禁止拷贝和赋值
    ToolFactory(const ToolFactory&) = delete;
    ToolFactory& operator=(const ToolFactory&) = delete;

    /**
     * @brief 注册内置工具
     */
    void registerBuiltInTools();

    /**
     * @brief 工具注册信息
     */
    struct ToolRegistration {
        ToolInfo info;
        ToolCreator creator;

        ToolRegistration() = default;
        ToolRegistration(const ToolInfo& i, ToolCreator c)
            : info(i), creator(c) {}
    };

    QMap<VisionTool::ToolType, ToolRegistration> tools_;    ///< 类型到工具的映射
    QMap<QString, VisionTool::ToolType> nameToType_;        ///< 名称到类型的映射
};

/**
 * @brief 工具注册辅助宏
 * @details 在工具类的cpp文件中使用此宏自动注册工具
 */
#define REGISTER_TOOL(Type, ToolClass, Name, Category, Description, Icon) \
    namespace { \
        struct ToolClass##Registrar { \
            ToolClass##Registrar() { \
                ToolFactory::instance().registerTool( \
                    Type, \
                    ToolFactory::ToolInfo(Name, Category, Description, Icon, Type), \
                    []() -> VisionTool* { return new ToolClass(); } \
                ); \
            } \
        }; \
        static ToolClass##Registrar g_##ToolClass##Registrar; \
    }

} // namespace Algorithm
} // namespace VisionForge

#endif // VISIONFORGE_ALGORITHM_TOOLFACTORY_H
