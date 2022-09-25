#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <iostream>
#include <strstream>
#include <fstream>
#include <vector>

namespace sylar
{

    //日志事件
    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent();

    private:
        const char *m_file = nullptr; //文件名
        int32_t m_line = 0;           //行号
        uint32_t m_elapse = 0;        //程序启动到当前毫秒数量
        uint32_t m_threadId = 0;      //线程ID
        uint32_t m_fiberId = 0;       //协程ID
        uint64_t m_time = 0;          //时间戳
        std::string m_content;        //
    };

    //日志级别
    class LogLevel
    {
    public:
        enum Level
        { //日志级别
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };

    //日志格式器
    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string& pattern);
        
        //按照自定义的%x 的格式化方法输出格式化字符串  -- 需要解析输入的格式化字符串
        std::string format(LogEvent::ptr event);

        void init(); //用于解析pattern
    private:
        class FormatItem
        {
            public:
                typedef std::shared_ptr<FormatItem> ptr;
                virtual ~FormatItem() {}
                virtual void format(std::ostream& os, LogEvent::ptr event) = 0;
        };


    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
    };

    //日志输出地
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender();

        virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormater(LogFormatter::ptr val){m_formatter = val;}
        LogFormatter::ptr getFormatter() const {return m_formatter;}
    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };
 
    //日志器
    class Logger
    {
    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string &name = "root");
        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }

    private:
        std::string m_name;                      //日志名称
        LogLevel::Level m_level;                 //日志级别
        std::list<LogAppender::ptr> m_appenders; // appenders是集合
    };

    //输出到控制台
    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        void log(LogLevel::Level level, LogEvent::ptr event) override;
    };

    //输出到文件
    class FileLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;

        FileLogAppender(const std::string& fileName);

        void log(LogLevel::Level level, LogEvent::ptr event) override;
        bool reopen(); //重新打开日志文件

    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

}

#endif