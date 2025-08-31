//
// Created by alexus on 31.08.2025.
//

#ifndef EFFECTIVE_MOBILE_VEHICLE_H
#define EFFECTIVE_MOBILE_VEHICLE_H
#include "ISerializer.h"


class IVehicle {
public:
	virtual void serialize(ISerializer& serializer) const = 0;

	virtual ~IVehicle() = default;
};

class Car : public IVehicle {
public:
	int doors = 5;
	int passengers = 7;

	void serialize(ISerializer& serializer) const override {
		serializer.add_field("doors", std::to_string(doors));
		serializer.add_field("passengers", std::to_string(passengers));
	}
};

class Plane : public IVehicle {
public:
	int seats = 400;

	void serialize(ISerializer& serializer) const override {
		serializer.add_field("seats", std::to_string(seats));
	}
};

class Ship : public IVehicle {
public:
	int cargoCapacity = 10000;

	void serialize(ISerializer& serializer) const override {
		serializer.add_field("cargo_capacity", std::to_string(cargoCapacity));
	}
};


#endif //EFFECTIVE_MOBILE_VEHICLE_H