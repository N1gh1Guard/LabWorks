#include "Exceptions.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <algorithm>

BaseException::BaseException(const std::string& message,
                           const std::string& file,
                           int line,
                           const std::string& function)
    : message_(message), file_(file), line_(line), function_(function),
      timestamp_(std::chrono::system_clock::now()) {
}

const char* BaseException::what() const noexcept {
    if (whatBuffer_.empty()) {
        whatBuffer_ = getFullInfo();
    }
    return whatBuffer_.c_str();
}

const std::string& BaseException::getMessage() const {
    return message_;
}

const std::string& BaseException::getFile() const {
    return file_;
}

int BaseException::getLine() const {
    return line_;
}

const std::string& BaseException::getFunction() const {
    return function_;
}

std::chrono::system_clock::time_point BaseException::getTimestamp() const {
    return timestamp_;
}

std::string BaseException::getFullInfo() const {
    std::ostringstream oss;
    oss << "Exception: " << message_;
    
    if (!file_.empty()) {
        oss << " [File: " << file_;
        if (line_ > 0) {
            oss << ":" << line_;
        }
        oss << "]";
    }
    
    if (!function_.empty()) {
        oss << " [Function: " << function_ << "]";
    }
    
    auto time_t = std::chrono::system_clock::to_time_t(timestamp_);
    std::tm tm_buf;
    localtime_r(&time_t, &tm_buf);
    oss << " [Time: " << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "]";
    
    return oss.str();
}

MapException::MapException(const std::string& message,
                           const std::string& file,
                           int line,
                           const std::string& function)
    : BaseException("MapException: " + message, file, line, function) {
}

MapInvalidDimensionsException::MapInvalidDimensionsException(int rows, int cols,
                                                             const std::string& file,
                                                             int line,
                                                             const std::string& function)
    : MapException("Invalid map dimensions: rows=" + std::to_string(rows) +
                   ", cols=" + std::to_string(cols) +
                   " (both must be positive)", file, line, function) {
}

MapOutOfBoundsException::MapOutOfBoundsException(int row, int col,
                                                 const std::string& file,
                                                 int line,
                                                 const std::string& function)
    : MapException("Cell coordinates out of bounds: row=" + std::to_string(row) +
                   ", col=" + std::to_string(col), file, line, function) {
}

GraphException::GraphException(const std::string& message,
                              const std::string& file,
                              int line,
                              const std::string& function)
    : BaseException("GraphException: " + message, file, line, function) {
}

GraphInvalidVertexException::GraphInvalidVertexException(int vertex, int maxVertices,
                                                         const std::string& file,
                                                         int line,
                                                         const std::string& function)
    : GraphException("Invalid vertex index: " + std::to_string(vertex) +
                     " (valid range: 0-" + std::to_string(maxVertices - 1) + ")",
                     file, line, function) {
}

GraphInvalidEdgeException::GraphInvalidEdgeException(double weight,
                                                     const std::string& file,
                                                     int line,
                                                     const std::string& function)
    : GraphException("Invalid edge weight: " + std::to_string(weight) +
                     " (must be non-negative)", file, line, function) {
}

PathfinderException::PathfinderException(const std::string& message,
                                        const std::string& file,
                                        int line,
                                        const std::string& function)
    : BaseException("PathfinderException: " + message, file, line, function) {
}

PathNotFoundException::PathNotFoundException(int start, int end,
                                             const std::string& file,
                                             int line,
                                             const std::string& function)
    : PathfinderException("Path not found from vertex " + std::to_string(start) +
                         " to vertex " + std::to_string(end), file, line, function) {
}

PathfinderInvalidVertexException::PathfinderInvalidVertexException(int vertex, int maxVertices,
                                                                   const std::string& file,
                                                                   int line,
                                                                   const std::string& function)
    : PathfinderException("Invalid vertex index: " + std::to_string(vertex) +
                         " (valid range: 0-" + std::to_string(maxVertices - 1) + ")",
                         file, line, function) {
}

IOException::IOException(const std::string& message,
                        const std::string& file,
                        int line,
                        const std::string& function)
    : BaseException("IOException: " + message, file, line, function) {
}

FileNotFoundException::FileNotFoundException(const std::string& filename,
                                            const std::string& file,
                                            int line,
                                            const std::string& function)
    : IOException("File not found: " + filename, file, line, function) {
}

FileReadException::FileReadException(const std::string& filename,
                                     const std::string& file,
                                     int line,
                                     const std::string& function)
    : IOException("Failed to read file: " + filename, file, line, function) {
}

FileWriteException::FileWriteException(const std::string& filename,
                                      const std::string& file,
                                      int line,
                                      const std::string& function)
    : IOException("Failed to write file: " + filename, file, line, function) {
}

MapGenerationException::MapGenerationException(const std::string& message,
                                               const std::string& file,
                                               int line,
                                               const std::string& function)
    : BaseException("MapGenerationException: " + message, file, line, function) {
}

ExceptionRecord::ExceptionRecord(const BaseException& ex)
    : type(typeid(ex).name()),
      message(ex.getMessage()),
      file(ex.getFile()),
      line(ex.getLine()),
      function(ex.getFunction()),
      timestamp(ex.getTimestamp()) {
}

ExceptionTracker& ExceptionTracker::getInstance() {
    static ExceptionTracker instance;
    return instance;
}

void ExceptionTracker::registerException(const BaseException& ex) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    ExceptionRecord record(ex);
    exceptionHistory_.push_back(record);
    
    std::string typeName = getExceptionTypeName(ex);
    exceptionCounts_[typeName]++;
    
    if (loggingEnabled_) {
        logException(record);
    }
}

size_t ExceptionTracker::getTotalExceptions() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return exceptionHistory_.size();
}

size_t ExceptionTracker::getExceptionCount(const std::string& type) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = exceptionCounts_.find(type);
    return (it != exceptionCounts_.end()) ? it->second : 0;
}

std::map<std::string, size_t> ExceptionTracker::getAllExceptionCounts() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return exceptionCounts_;
}

const std::vector<ExceptionRecord>& ExceptionTracker::getExceptionHistory() const {
    return exceptionHistory_;
}

std::vector<ExceptionRecord> ExceptionTracker::getRecentExceptions(size_t count) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t start = (exceptionHistory_.size() > count) 
                   ? exceptionHistory_.size() - count 
                   : 0;
    
    return std::vector<ExceptionRecord>(
        exceptionHistory_.begin() + start,
        exceptionHistory_.end()
    );
}

std::vector<ExceptionRecord> ExceptionTracker::getExceptionsByType(const std::string& type) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::vector<ExceptionRecord> result;
    for (const auto& record : exceptionHistory_) {
        if (record.type.find(type) != std::string::npos) {
            result.push_back(record);
        }
    }
    return result;
}

void ExceptionTracker::clearHistory() {
    std::lock_guard<std::mutex> lock(mutex_);
    exceptionHistory_.clear();
    exceptionCounts_.clear();
}

void ExceptionTracker::clearHistoryByType(const std::string& type) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    exceptionHistory_.erase(
        std::remove_if(exceptionHistory_.begin(), exceptionHistory_.end(),
            [&type](const ExceptionRecord& record) {
                return record.type.find(type) != std::string::npos;
            }),
        exceptionHistory_.end()
    );
    
    exceptionCounts_.clear();
    for (const auto& record : exceptionHistory_) {
        std::string typeName = record.type;
        exceptionCounts_[typeName]++;
    }
}

void ExceptionTracker::enableLogging(bool enable) {
    std::lock_guard<std::mutex> lock(mutex_);
    loggingEnabled_ = enable;
    
    if (enable && !logFilename_.empty()) {
        logFile_ = std::make_unique<std::ofstream>(logFilename_, std::ios::app);
    } else {
        logFile_.reset();
    }
}

void ExceptionTracker::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFilename_ = filename;
    
    if (loggingEnabled_) {
        logFile_ = std::make_unique<std::ofstream>(logFilename_, std::ios::app);
    }
}

bool ExceptionTracker::isLoggingEnabled() const {
    return loggingEnabled_;
}

std::string ExceptionTracker::getStatisticsString() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::ostringstream oss;
    oss << "=== Exception Statistics ===\n";
    oss << "Total exceptions: " << exceptionHistory_.size() << "\n\n";
    
    oss << "By type:\n";
    for (const auto& pair : exceptionCounts_) {
        oss << "  " << pair.first << ": " << pair.second << "\n";
    }
    
    return oss.str();
}

ExceptionTracker::~ExceptionTracker() {
    if (logFile_ && logFile_->is_open()) {
        logFile_->close();
    }
}

void ExceptionTracker::logException(const ExceptionRecord& record) {
    if (!logFile_ || !logFile_->is_open()) {
        return;
    }
    
    auto time_t = std::chrono::system_clock::to_time_t(record.timestamp);
    std::tm tm_buf;
    localtime_r(&time_t, &tm_buf);
    
    *logFile_ << "[" << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "] "
              << "Type: " << record.type << "\n"
              << "  Message: " << record.message << "\n";
    
    if (!record.file.empty()) {
        *logFile_ << "  File: " << record.file;
        if (record.line > 0) {
            *logFile_ << ":" << record.line;
        }
        *logFile_ << "\n";
    }
    
    if (!record.function.empty()) {
        *logFile_ << "  Function: " << record.function << "\n";
    }
    
    *logFile_ << "\n";
    logFile_->flush();
}

std::string ExceptionTracker::getExceptionTypeName(const BaseException& ex) const {
    std::string typeName = typeid(ex).name();
    
    size_t pos = typeName.find_last_of("0123456789");
    if (pos != std::string::npos && pos < typeName.length() - 1) {
        size_t start = pos + 1;
        if (start < typeName.length()) {
            return typeName.substr(start);
        }
    }
    
    size_t lastColon = typeName.find_last_of("::");
    if (lastColon != std::string::npos) {
        return typeName.substr(lastColon + 1);
    }
    
    return typeName;
}
