#include "Debug.h"

MessageType Debug::currentSev = MessageType::TYPE_NONE;

void Debug::DebugInit() {
	//CREATES LOG 
	std::ofstream out;
	out.open("V3ENGINELOG.txt", std::ios::out);
	out.close();
	currentSev = MessageType::TYPE_INFO;

}
void Debug::SetSeverity(MessageType type_) {

	currentSev = type_;

}

void Debug::Log(const MessageType type_, const std::string& message_, const std::string& filename_, const int line_) {

	if (type_ <= currentSev && currentSev > MessageType::TYPE_NONE) {

		std::ofstream file;
		file.open("V3ENGINELOG.txt", std::ios::out | std::ios::app);
		file << message_ << "in" << filename_ << "on line: " << line_;
		file.flush();
		file.close();

	}

}

void Debug::Info(const std::string& message_, const std::string& filename_, const int line_) {

	Log(MessageType::TYPE_INFO, "[INFO]" + message_, filename_, line_);
}
void Debug::Trace(const std::string& message_, const std::string& filename_, const int line_) {

	Log(MessageType::TYPE_TRACE, "[TRACE]" + message_, filename_, line_);
}

void Debug::Warning(const std::string& message_, const std::string& filename_, const int line_) {

	Log(MessageType::TYPE_WARNING, "[WARNING]" + message_, filename_, line_);
}
void Debug::Error(const std::string& message_, const std::string& filename_, const int line_) {

	Log(MessageType::TYPE_ERROR, "[ERROR]" + message_, filename_, line_);
}
void Debug::FatalError(const std::string& message_, const std::string& filename_, const int line_) {

	Log(MessageType::TYPE_FATAL_ERROR, "[FATAL_ERROR]" + message_, filename_, line_);
}



