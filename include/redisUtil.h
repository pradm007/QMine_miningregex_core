#ifndef REDIS_UTIL
#define REDIS_UTIL

#include "../include/commonHeader.h"
#include <stdio.h>
// string Q_KEY = "qmine";

class RedisUtil {
  public:
  
  static Redis connectRedis() throw () {
    try {
      // Create an Redis object, which is movable but NOT copyable.
      Redis redis("tcp://127.0.0.1:6379");
      return redis;
    } catch (const Error &e) {
      cout << e.what() << endl;
      throw e;
    }
  }
  
  static RequestObject getLastRequestObject() throw() {
    
    try {
      Redis redis("tcp://127.0.0.1:6379");
      
      unordered_map<string, string> obj;
      redis.hgetall("qmine", inserter(obj, obj.begin()));
      
      RequestObject reqObj;
      if (obj.size() == 0) {
        throw Error("No entry found in Redis !!");
      }
      
      for (auto it= obj.begin(); it!=obj.end();it++) {
          if (it->first == "requestId") {
            reqObj.requestId = it->second;
          }
          if (it->first == "pattern") {
            reqObj.pattern = it->second;
          }
          if (it->first == "status") {
            reqObj.status = it->second;
          }
          if (it->first == "event_length") {
            reqObj.event_length = it->second;
          }
          if (it->first == "input_file_path") {
            reqObj.input_file_path = it->second;
          }
          if (it->first == "output_file_path") {
            reqObj.output_file_path = it->second;
          }
      }
      
      if (DEBUG) {
        cout << "Request Object " << endl << "reqObj.requestId " << reqObj.requestId << endl
            << "reqObj.pattern " << reqObj.pattern << endl
            << "reqObj.event_length " << reqObj.event_length << endl
            << "reqObj.input_file_path " << reqObj.input_file_path << endl
            << "reqObj.output_file_path " << reqObj.output_file_path << endl;
      }
      
      return reqObj;
    } catch (const Error &e) {
      cout << e.what() << endl;
      throw e;
    }
    
  }
  
  static int insertOrUpdate(RequestObject reqObj) throw() {
    try {
      
      Redis redis("tcp://127.0.0.1:6379");
      
      unordered_map<string, string> obj;
      
      obj.insert(make_pair("requestId", reqObj.requestId));
      obj.insert(make_pair("pattern", reqObj.pattern));
      obj.insert(make_pair("status", reqObj.status));
      obj.insert(make_pair("event_length", reqObj.event_length));
      obj.insert(make_pair("input_file_path", reqObj.input_file_path));
      obj.insert(make_pair("output_file_path", reqObj.output_file_path));
      
      redis.hmset("qmine", obj.begin(), obj.end());
      
      return 1;
    } catch (const Error &e) {
      cout << e.what() << endl;
      throw(e);
    }
  }
  
};

#endif