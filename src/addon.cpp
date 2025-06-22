#include <napi.h>
#include "csv_parser.h"
#include <memory>

class CSVParserWrapper : public Napi::ObjectWrap<CSVParserWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    CSVParserWrapper(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    std::unique_ptr<CSVParser> parser_;
    
    Napi::Value Parse(const Napi::CallbackInfo& info);
    Napi::Value ParseToObjects(const Napi::CallbackInfo& info);
    Napi::Value GetHeaders(const Napi::CallbackInfo& info);
};

Napi::FunctionReference CSVParserWrapper::constructor;

Napi::Object CSVParserWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "CSVParser", {
        InstanceMethod("parse", &CSVParserWrapper::Parse),
        InstanceMethod("parseToObjects", &CSVParserWrapper::ParseToObjects),
        InstanceMethod("getHeaders", &CSVParserWrapper::GetHeaders)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("CSVParser", func);
    return exports;
}

CSVParserWrapper::CSVParserWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<CSVParserWrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    // Default options
    char delimiter = ',';
    bool hasHeader = true;
    bool skipEmptyLines = true;

    // Parse options if provided
    if (info.Length() > 0 && info[0].IsObject()) {
        Napi::Object options = info[0].As<Napi::Object>();
        
        if (options.Has("delimiter")) {
            std::string delimStr = options.Get("delimiter").As<Napi::String>();
            if (!delimStr.empty()) {
                delimiter = delimStr[0];
            }
        }
        
        if (options.Has("hasHeader")) {
            hasHeader = options.Get("hasHeader").As<Napi::Boolean>();
        }
        
        if (options.Has("skipEmptyLines")) {
            skipEmptyLines = options.Get("skipEmptyLines").As<Napi::Boolean>();
        }
    }

    parser_ = std::make_unique<CSVParser>(delimiter, hasHeader, skipEmptyLines);
}

Napi::Value CSVParserWrapper::Parse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string csvData = info[0].As<Napi::String>();
    
    try {
        auto result = parser_->parse(csvData);
        
        // Convert C++ vector to JS array
        Napi::Array jsResult = Napi::Array::New(env, result.size());
        for (size_t i = 0; i < result.size(); ++i) {
            Napi::Array row = Napi::Array::New(env, result[i].size());
            for (size_t j = 0; j < result[i].size(); ++j) {
                row[j] = Napi::String::New(env, result[i][j]);
            }
            jsResult[i] = row;
        }
        
        return jsResult;
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Value CSVParserWrapper::ParseToObjects(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string csvData = info[0].As<Napi::String>();
    
    try {
        auto result = parser_->parseToObjects(csvData);
        
        // Convert C++ vector of maps to JS array of objects
        Napi::Array jsResult = Napi::Array::New(env, result.size());
        for (size_t i = 0; i < result.size(); ++i) {
            Napi::Object obj = Napi::Object::New(env);
            for (const auto& pair : result[i]) {
                obj.Set(pair.first, pair.second);
            }
            jsResult[i] = obj;
        }
        
        return jsResult;
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Value CSVParserWrapper::GetHeaders(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    const auto& headers = parser_->getHeaders();
    Napi::Array jsHeaders = Napi::Array::New(env, headers.size());
    
    for (size_t i = 0; i < headers.size(); ++i) {
        jsHeaders[i] = Napi::String::New(env, headers[i]);
    }
    
    return jsHeaders;
}

// Standalone parse function
Napi::Value ParseCSV(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string csvData = info[0].As<Napi::String>();
    
    // Parse options
    char delimiter = ',';
    bool hasHeader = true;
    bool skipEmptyLines = true;
    
    if (info.Length() > 1 && info[1].IsObject()) {
        Napi::Object options = info[1].As<Napi::Object>();
        
        if (options.Has("delimiter")) {
            std::string delimStr = options.Get("delimiter").As<Napi::String>();
            if (!delimStr.empty()) {
                delimiter = delimStr[0];
            }
        }
        
        if (options.Has("hasHeader")) {
            hasHeader = options.Get("hasHeader").As<Napi::Boolean>();
        }
        
        if (options.Has("skipEmptyLines")) {
            skipEmptyLines = options.Get("skipEmptyLines").As<Napi::Boolean>();
        }
    }

    try {
        CSVParser parser(delimiter, hasHeader, skipEmptyLines);
        auto result = parser.parse(csvData);
        
        // Convert C++ vector to JS array
        Napi::Array jsResult = Napi::Array::New(env, result.size());
        for (size_t i = 0; i < result.size(); ++i) {
            Napi::Array row = Napi::Array::New(env, result[i].size());
            for (size_t j = 0; j < result[i].size(); ++j) {
                row[j] = Napi::String::New(env, result[i][j]);
            }
            jsResult[i] = row;
        }
        
        return jsResult;
    } catch (const std::exception& e) {
        Napi::Error::New(env, e.what()).ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    CSVParserWrapper::Init(env, exports);
    exports.Set(Napi::String::New(env, "parse"), Napi::Function::New(env, ParseCSV));
    return exports;
}

NODE_API_MODULE(csv_parser, Init)