//
// Created by alexus on 31.08.2025.
//

#ifndef EFFECTIVE_MOBILE_SERIALIZERS_H
#define EFFECTIVE_MOBILE_SERIALIZERS_H
#include <sstream>
#include <iomanip>
#include <stack>
#include <nlohmann/json.hpp>
#include "ISerializer.h"

class XmlSerializer final : public ISerializer {
	std::ostringstream xml;
	std::string lastTag;
public:
	void add_field(const std::string& name, const std::string& value) override {
		xml << "<" << name << ">" << value << "</" << name << ">\n";
	}

	void add_block(const std::string& name) override {
		xml << "<" << name << ">\n";
		lastTag = name;
	}

	void end_block() override {

		xml << "</" << lastTag << ">\n";
	}

	std::string build() const override {
		return xml.str();
	}

};

class JsonSerializer final : public ISerializer {
	nlohmann::json json;
	std::stack<std::pair<nlohmann::json, std::string>> blocks;
public:
	void add_field(const std::string& name, const std::string& value) override {
		if (blocks.empty())
			json[name] = value;
		else blocks.top().first[name] = value;
	}

	void add_block(const std::string& name) override {
		blocks.emplace(nlohmann::json{},  name);
	}
	void end_block() override {
		auto &pair = blocks.top();
		json[pair.second] = pair.first;
		blocks.pop();
	}

	std::string build() const override {
		return json.dump();
	}

};

#endif //EFFECTIVE_MOBILE_SERIALIZERS_H