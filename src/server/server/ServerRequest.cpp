//
// Created by matthieu on 08/01/25.
//
#include <iostream>

#include "server.h"
using namespace server;

std::string ServerRequest::serialize() {
    Json::Value root;
    root["type"] = static_cast<int>(type);
    root["data"] = data;
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

ServerRequest ServerRequest::deserialize (std::string& jsonString) {
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::string errors;
    std::cout<<"request :"<<jsonString<<std::endl;
    std::istringstream jsonStream(jsonString);
    if (!Json::parseFromStream(reader, jsonStream, &root, &errors)) {
        throw std::runtime_error("Failed to parse JSON: " + errors);
    }

    ServerRequest request;
    request.type = static_cast<RequestType>(root["type"].asInt());
    request.data = root["data"];
    return request;
}