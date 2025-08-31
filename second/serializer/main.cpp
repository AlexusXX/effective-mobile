//
// Created by alexus on 31.08.2025.
//

#include <memory>

#include "IVehicle.h"
#include "Serializers.h"

void serialize(const IVehicle& vehicle, ISerializer& serializer) {
	vehicle.serialize(serializer);
}
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cerr << "Usage: program [json|xml]\n";
		return 1;
	}

	std::string arg = argv[1];
	std::shared_ptr<ISerializer> serializer;
	if (arg == "xml")
		serializer = std::make_shared<XmlSerializer>();
	else if (arg == "json")
		serializer = std::make_shared<JsonSerializer>();
	else
		std::cerr << "Unknown argument\n";

	Car car;
	serializer->add_block("BMW G30");
	serialize(car, *serializer);
	serializer->end_block();

	Plane plane;
	serializer->add_block("Boeing 747-400");
	serialize(plane, *serializer);
	serializer->end_block();

	Ship ship;
	serializer->add_block("MS Queen Victoria");
	serialize(ship, *serializer);
	serializer->end_block();

	std::cout << serializer->build() << std::endl;

	return 0;
}
