#pragma once
#include "includes.h"
#include <iostream>
#include <vector>

class Resources
{
	private:
		std::vector<int> resourcesStock = {0, 0, 0, 0};
	
	public:
		Resources() = default;
		Resources(std::vector<int> newStock) : resourcesStock(newStock) {}

		std::vector<int> getResources() const { return resourcesStock; }
		int getRiceStock() const { return resourcesStock[RICE]; }
		int getTimberStock() const { return resourcesStock[TIMBER]; }
		int getIronStock() const { return resourcesStock[IRON]; }
		int getGoldStock() const { return resourcesStock[GOLD]; }
		
		void scaleStock(int index, int value) { resourcesStock[index] = resourcesStock[index] * value / 100; }
		void addToStock(int index, int value) { resourcesStock[index] += value; }
		void subFromStock(int index, int value) { resourcesStock[index] = std::max(resourcesStock[index] - value, 0); }
		
		void operator+=(const Resources& other);
		void operator-=(const Resources& other);
		bool operator>=(const Resources& other) const;
		int operator[](int index) const { return resourcesStock[index]; }
};
