/**
 * @file TranslationManager.h
 * @brief 翻译管理器 - 多语言支持
 * @author VisionForge Team
 * @date 2025-12-18
 *
 * @details
 * 本文件实现了VisionForge项目的多语言支持系统，基于Qt的国际化机制。
 *
 * ## 设计模式
 * - **单例模式 (Singleton)**：全局唯一的翻译管理器实例
 * - **观察者模式 (Observer)**：语言切换时通过信号通知UI刷新
 *
 * ## 支持的语言
 * - 简体中文 (zh_CN) - 默认语言
 * - English (en_US)
 * - 日本語 (ja_JP)
 * - 한국어 (ko_KR)
 * - Auto - 跟随系统语言
 *
 * ## 翻译文件
 * 翻译文件使用Qt的.qm格式：
 * - visionforge_en_US.qm
 * - visionforge_ja_JP.qm
 * - visionforge_ko_KR.qm
 *
 * 中文作为源语言，不需要翻译文件。
 *
 * ## 工作流程
 * 1. 使用Qt Linguist工具提取源码中的tr()字符串
 * 2. 翻译人员在.ts文件中翻译
 * 3. 使用lrelease编译为.qm二进制文件
 * 4. 运行时由TranslationManager加载
 *
 * ## 线程安全
 * - 语言切换操作非线程安全，应在主线程进行
 * - Qt的tr()机制本身是线程安全的
 *
 * ## 内存管理
 * - QTranslator对象使用unique_ptr管理
 * - 切换语言时自动卸载旧翻译，加载新翻译
 *
 * ## 使用示例
 * @code
 * // 初始化
 * TR_MANAGER.initialize();
 *
 * // 切换语言
 * TR_MANAGER.setLanguage(Language::English);
 *
 * // 获取当前语言
 * QString name = TR_MANAGER.currentLanguageName();  // "English"
 *
 * // 监听语言变化
 * connect(&TR_MANAGER, &TranslationManager::languageChanged,
 *         this, &MainWindow::retranslateUi);
 *
 * // 在代码中使用tr()
 * label->setText(tr("工位配置"));  // 自动翻译
 * @endcode
 */

#pragma once

#include <QObject>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include <QMap>
#include <memory>

namespace VisionForge {
namespace Base {

/**
 * @brief 支持的语言枚举
 */
enum class Language {
    Chinese,        // 中文（默认）
    English,        // 英语
    Japanese,       // 日语
    Korean,         // 韩语
    Auto            // 跟随系统
};

/**
 * @class TranslationManager
 * @brief 翻译管理器（单例）
 *
 * 功能：
 * - 加载/切换语言
 * - 管理翻译文件
 * - 提供翻译接口
 */
class TranslationManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 获取单例实例
     */
    static TranslationManager& instance();

    /**
     * @brief 初始化翻译系统
     * @param translationsPath 翻译文件目录路径
     */
    void initialize(const QString& translationsPath = QString());

    /**
     * @brief 设置当前语言
     * @param language 语言枚举
     * @return true如果成功
     */
    bool setLanguage(Language language);

    /**
     * @brief 设置当前语言（按语言代码）
     * @param languageCode 语言代码（如 "zh_CN", "en_US"）
     * @return true如果成功
     */
    bool setLanguage(const QString& languageCode);

    /**
     * @brief 获取当前语言
     */
    Language currentLanguage() const { return currentLanguage_; }

    /**
     * @brief 获取当前语言代码
     */
    QString currentLanguageCode() const;

    /**
     * @brief 获取当前语言名称
     */
    QString currentLanguageName() const;

    /**
     * @brief 获取可用语言列表
     */
    QList<Language> availableLanguages() const;

    /**
     * @brief 获取语言名称
     */
    static QString languageName(Language language);

    /**
     * @brief 获取语言代码
     */
    static QString languageCode(Language language);

    /**
     * @brief 从语言代码获取枚举
     */
    static Language languageFromCode(const QString& code);

signals:
    /**
     * @brief 语言改变信号
     */
    void languageChanged(Language language);

private:
    TranslationManager();
    ~TranslationManager();
    TranslationManager(const TranslationManager&) = delete;
    TranslationManager& operator=(const TranslationManager&) = delete;

    /**
     * @brief 加载翻译文件
     */
    bool loadTranslation(const QString& languageCode);

    /**
     * @brief 卸载当前翻译
     */
    void unloadCurrentTranslation();

private:
    QString translationsPath_;
    Language currentLanguage_;
    std::unique_ptr<QTranslator> appTranslator_;
    std::unique_ptr<QTranslator> qtTranslator_;
    bool initialized_;
};

} // namespace Base
} // namespace VisionForge

// 便捷宏
#define TR_MANAGER VisionForge::Base::TranslationManager::instance()
