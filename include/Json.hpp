#pragma once
#include <memory>

template<typename T>
using Ptr = std::shared_ptr<T>;

#ifdef _RAPIDJSON
#define CHECK(name) document.HasMember(name) && !document[name].IsNull()
#define CHECK_M(d, name) d.HasMember(name) && !d[name].IsNull()
#define CHECK_P(d, name) d->HasMember(name) && !d[name].IsNull()

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#else
#define CHECK(name) json.contains(name) && !json[name].is_null()
#define CHECK_M(j, name) j.contains(name) && !j[name].is_null()
#define CHECK_P(j, name) j->contains(name) && !j[name].is_null()

#include <nlohmann/json.hpp>
#endif

namespace Discord { using Json = nlohmann::json; }