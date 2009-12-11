#pragma once
#include <string>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <stdexcept>

/// @TODO atoi doesn't handle errors nicely

namespace game {
  using namespace std;

  class Object {
  public:
    // These are important enough to have their own public slots for
    // easy access.  However, much code should talk to this through
    // the above methods.
    std::string id, type;
    int x, y;

    Object() {}
    Object(const string &id, const string &type, int x, int y)
      : id(id), type(type), x(x), y(y) {}
    Object(const string &id, const string &type,
           const string& x, const string& y)
        : id(id), type(type) {
      setProperty("x", x);
      setProperty("y", y); }

    virtual string getProperty(const string &property) {
      if (!property.compare("x")) {
        ostringstream o;
        o << x;
        return string(o.str()); }
      if (!property.compare("y")) {
        ostringstream o;
        o << y;
        return string(o.str()); }
      if (!property.compare("id")) return id;
      if (!property.compare("type")) return type;
      throw runtime_error("Invalid ID for getting.  "); }

    virtual void setProperty(const string &property, const string &value) {
      if (!property.compare("x")) x = atoi(value.c_str());
      else if (!property.compare("y")) y = atoi(value.c_str());
      else throw runtime_error("Invalid ID for setting.  "); }

    friend bool operator< (const Object &x, const Object &y) {
      return x.id < y.id; }

    string &operator[] (const string &prop) {
      // TODO This is a memory leak and bad...  Rework this
      string *result = new string;
      *result = getProperty(prop);
      return *result; }};}
