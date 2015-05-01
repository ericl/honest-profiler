#include "log_writer.h"
#include <iostream>
#include <iomanip>

using std::copy;

bool isLittleEndian() {
    short int number = 0x1;
    char *numPtr = (char *) &number;
    return (numPtr[0] == 1);
}

static bool IS_LITTLE_ENDIAN = isLittleEndian();

template<typename T>
void LogWriter::writeValue(const T &value) {
//    if (IS_LITTLE_ENDIAN) {
//        const char *data = reinterpret_cast<const char *>(&value);
//        for (int i = sizeof(value) - 1; i >= 0; i--) {
//            output_.put(data[i]);
//        }
//    } else {
//        output_.write(reinterpret_cast<const char *>(&value), sizeof(value));
//    }
}

// TODO: implement
static int64_t getThreadId(JNIEnv *env_id) {
    return (int64_t) env_id;
}

void LogWriter::record(const JVMPI_CallTrace &trace) {
    int64_t threadId = getThreadId(trace.env_id);
    recordTraceStart(trace.num_frames, threadId);

    for (int i = 0; i < trace.num_frames; i++) {
        JVMPI_CallFrame frame = trace.frames[i];
        method_id methodId = (method_id) frame.method_id;
        recordFrame(frame.lineno, methodId);
        inspectMethod(methodId, frame);
    }

    // JSON
    output_ << "{";
    output_ << "\"type\": \"frame\", ";
    output_ << "\"numFrames\": " << std::dec << trace.num_frames << ", ";
    output_ << "\"frames\": [";
    for (int i = 0; i < trace.num_frames; i++) {
        JVMPI_CallFrame frame = trace.frames[i];
        output_ << "{\"methodId\": \"" << std::hex << frame.method_id << "\", ";
        output_ << "\"lineno\": " << std::dec << frame.lineno << "}";
        if (i != trace.num_frames - 1) {
          output_ << ", ";
        }
    }
    output_ << "]}" << std::endl;
}

void LogWriter::inspectMethod(const method_id methodId,
        const JVMPI_CallFrame &frame) {
    if (knownMethods.count(methodId) > 0) {
        return;
    }

    knownMethods.insert(methodId);
    frameLookup_(frame, jvmti_, *this);
}

void LogWriter::recordTraceStart(const jint numFrames, const int64_t threadId) {
//    output_.put(TRACE_START);
//    writeValue(numFrames);
//    writeValue(threadId);
//    output_.flush();
}

void LogWriter::recordFrame(const jint lineNumber, const method_id methodId) {
//    output_.put(FRAME);
//    writeValue(lineNumber);
//    writeValue(methodId);
//    output_.flush();
}

void LogWriter::writeWithSize(const char *value) {
//    jint size = (jint) strlen(value);
//    writeValue(size);
//    output_.write(value, size);
}

void LogWriter::recordNewMethod(const int64_t methodId, const char *fileName,
        const char *className, const char *methodName) {
    // JSON
    output_ << "{";
    output_ << "\"type\": \"newMethod\",";
    output_ << "\"methodId\": \"0x" << std::hex << methodId << "\", ";
    output_ << "\"fileName\": \"" << fileName << "\", ";
    output_ << "\"className\": \"" << className << "\", ";
    output_ << "\"methodName\": \"" << methodName << "\"";
    output_ << "}" << std::endl;

//    output_.put(NEW_METHOD);
//    writeValue(methodId);
//    writeWithSize(fileName);
//    writeWithSize(className);
//    writeWithSize(methodName);
//    output_.flush();
}
