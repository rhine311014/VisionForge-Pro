/**
 * @file TranslationManager.cpp
 * @brief 翻译管理器实现
 * @author VisionForge Team
 * @date 2025-12-18
 *
 * @details
 * 本文件实现TranslationManager类的所有成员函数。
 *
 * ## 实现细节
 *
 * ### 自动语言检测
 * 当语言设置为Auto时，根据系统语言自动选择：
 * - zh_* -> Chinese
 * - ja_* -> Japanese
 * - ko_* -> Korean
 * - 其他 -> English
 *
 * ### 翻译文件加载
 * loadTranslation()加载两类翻译文件：
 * 1. 应用翻译：visionforge_{lang}.qm
 * 2. Qt翻译：qt_{lang}.qm（来自Qt安装目录）
 *
 * ### 中文处理
 * 中文(zh_CN)是源语言，不需要加载翻译文件，
 * loadTranslation()对中文直接返回true。
 *
 * ### 语言切换流程
 * 1. unloadCurrentTranslation() - 卸载当前翻译
 * 2. loadTranslation() - 加载新翻译
 * 3. emit languageChanged() - 通知UI刷新
 */

#include "base/TranslationManager.h"
#include "base/Logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>

namespace VisionForge {
namespace Base {

TranslationManager::TranslationManager()
    : QObject(nullptr)
    , currentLanguage_(Language::Chinese)
    , appTranslator_(std::make_unique<QTranslator>())
    , qtTranslator_(std::make_unique<QTranslator>())
    , initialized_(false)
{
}

TranslationManager::~TranslationManager()
{
    unloadCurrentTranslation();
}

TranslationManager& TranslationManager::instance()
{
    static TranslationManager instance;
    return instance;
}

void TranslationManager::initialize(const QString& translationsPath)
{
    if (initialized_) return;

    // 设置翻译文件路径
    if (translationsPath.isEmpty()) {
        // 默认路径：应用程序目录/translations
        translationsPath_ = QCoreApplication::applicationDirPath() + "/translations";
    } else {
        translationsPath_ = translationsPath;
    }

    // 确保目录存在
    QDir dir(translationsPath_);
    if (!dir.exists()) {
        dir.mkpath(".");
        LOG_INFO(QString("创建翻译目录: %1").arg(translationsPath_));
    }

    initialized_ = true;
    LOG_INFO(QString("翻译管理器初始化完成，翻译路径: %1").arg(translationsPath_));

    // 加载默认语言（中文）
    setLanguage(Language::Chinese);
}

bool TranslationManager::setLanguage(Language language)
{
    if (language == Language::Auto) {
        // 跟随系统语言
        QLocale systemLocale = QLocale::system();
        QString langCode = systemLocale.name(); // 如 "zh_CN", "en_US"

        if (langCode.startsWith("zh")) {
            language = Language::Chinese;
        } else if (langCode.startsWith("ja")) {
            language = Language::Japanese;
        } else if (langCode.startsWith("ko")) {
            language = Language::Korean;
        } else {
            language = Language::English;
        }
    }

    if (language == currentLanguage_ && initialized_) {
        return true;
    }

    QString langCode = languageCode(language);
    if (loadTranslation(langCode)) {
        currentLanguage_ = language;
        emit languageChanged(language);
        LOG_INFO(QString("语言已切换至: %1").arg(languageName(language)));
        return true;
    }

    return false;
}

bool TranslationManager::setLanguage(const QString& languageCode)
{
    Language lang = languageFromCode(languageCode);
    return setLanguage(lang);
}

QString TranslationManager::currentLanguageCode() const
{
    return languageCode(currentLanguage_);
}

QString TranslationManager::currentLanguageName() const
{
    return languageName(currentLanguage_);
}

QList<Language> TranslationManager::availableLanguages() const
{
    return { Language::Chinese, Language::English, Language::Japanese, Language::Korean };
}

QString TranslationManager::languageName(Language language)
{
    switch (language) {
    case Language::Chinese:  return "简体中文";
    case Language::English:  return "English";
    case Language::Japanese: return "日本語";
    case Language::Korean:   return "한국어";
    case Language::Auto:     return "跟随系统";
    default:                 return "Unknown";
    }
}

QString TranslationManager::languageCode(Language language)
{
    switch (language) {
    case Language::Chinese:  return "zh_CN";
    case Language::English:  return "en_US";
    case Language::Japanese: return "ja_JP";
    case Language::Korean:   return "ko_KR";
    default:                 return "zh_CN";
    }
}

Language TranslationManager::languageFromCode(const QString& code)
{
    if (code.startsWith("zh")) return Language::Chinese;
    if (code.startsWith("en")) return Language::English;
    if (code.startsWith("ja")) return Language::Japanese;
    if (code.startsWith("ko")) return Language::Korean;
    return Language::Chinese;
}

bool TranslationManager::loadTranslation(const QString& languageCode)
{
    // 卸载当前翻译
    unloadCurrentTranslation();

    // 中文是默认语言，不需要加载翻译文件
    if (languageCode == "zh_CN") {
        return true;
    }

    // 加载应用程序翻译
    QString appQmFile = QString("visionforge_%1.qm").arg(languageCode);
    QString appQmPath = translationsPath_ + "/" + appQmFile;

    if (QFile::exists(appQmPath)) {
        if (appTranslator_->load(appQmPath)) {
            QCoreApplication::installTranslator(appTranslator_.get());
            LOG_DEBUG(QString("加载应用翻译: %1").arg(appQmFile));
        } else {
            LOG_WARNING(QString("加载应用翻译失败: %1").arg(appQmFile));
        }
    } else {
        LOG_DEBUG(QString("应用翻译文件不存在: %1").arg(appQmPath));
    }

    // 加载Qt翻译
    QString qtQmFile = QString("qt_%1.qm").arg(languageCode);
    QString qtTransPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);

    if (qtTranslator_->load(qtQmFile, qtTransPath)) {
        QCoreApplication::installTranslator(qtTranslator_.get());
        LOG_DEBUG(QString("加载Qt翻译: %1").arg(qtQmFile));
    }

    return true;
}

void TranslationManager::unloadCurrentTranslation()
{
    QCoreApplication::removeTranslator(appTranslator_.get());
    QCoreApplication::removeTranslator(qtTranslator_.get());
}

} // namespace Base
} // namespace VisionForge
