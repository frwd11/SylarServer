#include "log.h"


namespace sylar
{

    Logger::Logger(const std::string &name)
        :m_name(name)
    {
    }
    void Logger::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level) //大于等于当前日志登记的才输出 -- 等级越大越严重
        {
            for (auto& i : m_appenders)
                i->log(level, event);
        }
    }

    void Logger::debug(LogEvent::ptr event)
    {
        log(LogLevel::DEBUG, event);
    }

    void Logger::info(LogEvent::ptr event)
    {
        log(LogLevel::INFO, event);
    }

    void Logger::warn(LogEvent::ptr event)
    {
        log(LogLevel::WARN, event);
    }

    void Logger::error(LogEvent::ptr event)
    {
        log(LogLevel::ERROR, event);
    }

    void Logger::fatal(LogEvent::ptr event)
    {
        log(LogLevel::FATAL, event);
    }

    void Logger::addAppender(LogAppender::ptr appender)
    {
        m_appenders.push_back(appender);
    }

    void Logger::delAppender(LogAppender::ptr appender)
    {
        for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it)
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
        }
    }

    void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
            std::cout << m_formatter->format(event);
    }

    FileLogAppender::FileLogAppender(const std::string& fileName)
        : m_filename(fileName)
    {

    }

    void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
            m_filestream << m_formatter->format(event);
    }

    bool FileLogAppender::reopen()
    {
        if (m_filestream)
            m_filestream.close();
        
        m_filestream.open(m_filename.c_str());
        return !!m_filestream; //!!将非0值转换为true,0值仍然为0
    }

    std::string LogFormatter::format(LogEvent::ptr event)
    {
        std::stringstream ss;
        for (auto& i : m_items)
            i->format(ss, event);
        return ss.str();
    }

    LogFormatter::LogFormatter(const std::string& pattern)
        : m_pattern(pattern)
    {

    }

    void LogFormatter::init()
    {
        std::vector<std::pair<std::string, int>> vec;
        size_t last_pos = 0;
        for (size_t i=0; i<m_pattern.size(); ++i)
        {
            if (m_pattern[i] == '%')
            {
                
            }
        }
    }
}