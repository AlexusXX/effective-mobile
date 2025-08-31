//
// Created by alexus on 31.08.2025.
//

#ifndef EFFECTIVE_MOBILE_ISERIALIZER_H
#define EFFECTIVE_MOBILE_ISERIALIZER_H

#include <string>

class ISerializer {
public:
	virtual void add_field(const std::string& name, const std::string& value) = 0;
	virtual void add_block(const std::string& name) = 0;
	virtual void end_block() = 0;
	[[nodiscard]] virtual std::string build() const = 0;
	virtual ~ISerializer() {}
};



#endif //EFFECTIVE_MOBILE_ISERIALIZER_H