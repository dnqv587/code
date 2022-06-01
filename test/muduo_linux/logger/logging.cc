#include "logging.h"

Logger::Impl::Impl(LogLevel level, int old_errno, const SourceFile& file, int line)
	:m_logLevel(level), m_stream(),m_time(Timestamp::now()), m_line(line), m_baseName(file)
{

}
