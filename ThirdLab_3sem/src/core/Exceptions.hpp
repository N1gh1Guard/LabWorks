#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>
#include <map>
#include <fstream>

class BaseException : public std::exception {
public:
    BaseException(const std::string& message, 
                  const std::string& file = "", 
                  int line = 0,
                  const std::string& function = "");
    
    virtual ~BaseException() = default;
    
    const char* what() const noexcept override;
    const std::string& getMessage() const;
    const std::string& getFile() const;
    int getLine() const;
    const std::string& getFunction() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    
    std::string getFullInfo() const;

protected:
    std::string message_;
    std::string file_;
    int line_;
    std::string function_;
    std::chrono::system_clock::time_point timestamp_;
    mutable std::string whatBuffer_;
};

class MapException : public BaseException {
public:
    MapException(const std::string& message,
                const std::string& file = "",
                int line = 0,
                const std::string& function = "");
};

class MapInvalidDimensionsException : public MapException {
public:
    MapInvalidDimensionsException(int rows, int cols,
                                 const std::string& file = "",
                                 int line = 0,
                                 const std::string& function = "");
};

class MapOutOfBoundsException : public MapException {
public:
    MapOutOfBoundsException(int row, int col,
                           const std::string& file = "",
                           int line = 0,
                           const std::string& function = "");
};

class GraphException : public BaseException {
public:
    GraphException(const std::string& message,
                  const std::string& file = "",
                  int line = 0,
                  const std::string& function = "");
};

class GraphInvalidVertexException : public GraphException {
public:
    GraphInvalidVertexException(int vertex, int maxVertices,
                               const std::string& file = "",
                               int line = 0,
                               const std::string& function = "");
};

class GraphInvalidEdgeException : public GraphException {
public:
    GraphInvalidEdgeException(double weight,
                             const std::string& file = "",
                             int line = 0,
                             const std::string& function = "");
};

class PathfinderException : public BaseException {
public:
    PathfinderException(const std::string& message,
                      const std::string& file = "",
                      int line = 0,
                      const std::string& function = "");
};

class PathNotFoundException : public PathfinderException {
public:
    PathNotFoundException(int start, int end,
                        const std::string& file = "",
                        int line = 0,
                        const std::string& function = "");
};

class PathfinderInvalidVertexException : public PathfinderException {
public:
    PathfinderInvalidVertexException(int vertex, int maxVertices,
                                    const std::string& file = "",
                                    int line = 0,
                                    const std::string& function = "");
};

class IOException : public BaseException {
public:
    IOException(const std::string& message,
               const std::string& file = "",
               int line = 0,
               const std::string& function = "");
};

class FileNotFoundException : public IOException {
public:
    FileNotFoundException(const std::string& filename,
                        const std::string& file = "",
                        int line = 0,
                        const std::string& function = "");
};

class FileReadException : public IOException {
public:
    FileReadException(const std::string& filename,
                     const std::string& file = "",
                     int line = 0,
                     const std::string& function = "");
};

class FileWriteException : public IOException {
public:
    FileWriteException(const std::string& filename,
                      const std::string& file = "",
                      int line = 0,
                      const std::string& function = "");
};

class MapGenerationException : public BaseException {
public:
    MapGenerationException(const std::string& message,
                          const std::string& file = "",
                          int line = 0,
                          const std::string& function = "");
};

struct ExceptionRecord {
    std::string type;
    std::string message;
    std::string file;
    int line;
    std::string function;
    std::chrono::system_clock::time_point timestamp;
    
    ExceptionRecord(const BaseException& ex);
};

class ExceptionTracker {
public:
    static ExceptionTracker& getInstance();
    
    void registerException(const BaseException& ex);
    
    size_t getTotalExceptions() const;
    size_t getExceptionCount(const std::string& type) const;
    std::map<std::string, size_t> getAllExceptionCounts() const;
    
    const std::vector<ExceptionRecord>& getExceptionHistory() const;
    std::vector<ExceptionRecord> getRecentExceptions(size_t count) const;
    std::vector<ExceptionRecord> getExceptionsByType(const std::string& type) const;
    
    void clearHistory();
    void clearHistoryByType(const std::string& type);
    
    void enableLogging(bool enable = true);
    void setLogFile(const std::string& filename);
    bool isLoggingEnabled() const;
    
    std::string getStatisticsString() const;
    
    ~ExceptionTracker();

private:
    ExceptionTracker() : loggingEnabled_(false) {}
    ExceptionTracker(const ExceptionTracker&) = delete;
    ExceptionTracker& operator=(const ExceptionTracker&) = delete;
    
    void logException(const ExceptionRecord& record);
    std::string getExceptionTypeName(const BaseException& ex) const;
    
    std::vector<ExceptionRecord> exceptionHistory_;
    std::map<std::string, size_t> exceptionCounts_;
    mutable std::mutex mutex_;
    
    bool loggingEnabled_;
    std::string logFilename_;
    std::unique_ptr<std::ofstream> logFile_;
};

#define THROW_MAP_EXCEPTION(msg) \
    throw MapException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_MAP_INVALID_DIMENSIONS(rows, cols) \
    throw MapInvalidDimensionsException(rows, cols, __FILE__, __LINE__, __FUNCTION__)

#define THROW_MAP_OUT_OF_BOUNDS(row, col) \
    throw MapOutOfBoundsException(row, col, __FILE__, __LINE__, __FUNCTION__)

#define THROW_GRAPH_EXCEPTION(msg) \
    throw GraphException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_GRAPH_INVALID_VERTEX(vertex, maxVertices) \
    throw GraphInvalidVertexException(vertex, maxVertices, __FILE__, __LINE__, __FUNCTION__)

#define THROW_GRAPH_INVALID_EDGE(weight) \
    throw GraphInvalidEdgeException(weight, __FILE__, __LINE__, __FUNCTION__)

#define THROW_PATHFINDER_EXCEPTION(msg) \
    throw PathfinderException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_PATH_NOT_FOUND(start, end) \
    throw PathNotFoundException(start, end, __FILE__, __LINE__, __FUNCTION__)

#define THROW_PATHFINDER_INVALID_VERTEX(vertex, maxVertices) \
    throw PathfinderInvalidVertexException(vertex, maxVertices, __FILE__, __LINE__, __FUNCTION__)

#define THROW_IO_EXCEPTION(msg) \
    throw IOException(msg, __FILE__, __LINE__, __FUNCTION__)

#define THROW_FILE_NOT_FOUND(filename) \
    throw FileNotFoundException(filename, __FILE__, __LINE__, __FUNCTION__)

#define THROW_FILE_READ_EXCEPTION(filename) \
    throw FileReadException(filename, __FILE__, __LINE__, __FUNCTION__)

#define THROW_FILE_WRITE_EXCEPTION(filename) \
    throw FileWriteException(filename, __FILE__, __LINE__, __FUNCTION__)

#define THROW_MAP_GENERATION_EXCEPTION(msg) \
    throw MapGenerationException(msg, __FILE__, __LINE__, __FUNCTION__)

#define CATCH_AND_TRACK(ex_type) \
    catch (const ex_type& e) { \
        ExceptionTracker::getInstance().registerException(e); \
        throw; \
    }

#define CATCH_AND_TRACK_ALL() \
    catch (const BaseException& e) { \
        ExceptionTracker::getInstance().registerException(e); \
        throw; \
    } catch (const std::exception& e) { \
        BaseException baseEx(e.what()); \
        ExceptionTracker::getInstance().registerException(baseEx); \
        throw; \
    }
