/**
 * @file TranslationManager.h
 * @brief 翻译管理器 - 多语言支持
 * @author VisionForge Team
 * @date 2025-12-18
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
