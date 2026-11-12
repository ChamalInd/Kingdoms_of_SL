#pragma once
#include "includes.h"

class Resources
{
	private:
		std::vector<int> resourcesStock;
	
	public:
		Resources() = default;
		Resources(std::vector<int> newStock) : resourcesStock(newStock) {}

		std::vector<int> getResources() const { return resourcesStock; }
		int getRiceStock() const { return resourcesStock[0]; }
		int getTimberStock() const { return resourcesStock[1]; }
		int getIronStock() const { return resourcesStock[2]; }
		int getGoldStock() const { return resourcesStock[3]; }
		
		void addToStock(int index, int value) { resourcesStock[index] += value; }
		void subFromStock(int index, int value) { resourcesStock[index] -= value; }
		
		void operator+=(Resources& other);
		void operator-=(Resources& other);
		bool operator<(Resources& other);
		bool operator>(Resources& other);
		int operator[](int index) const { return resourcesStock[index]; }
};
