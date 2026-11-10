#pragma once
#include "includes.h"

class Resources
{
	private:
		int riceCapacity = 500;
		int timberCapacity = 200;
		int ironCapacity = 100;
		int goldCapacity = 1000;
	
	public:
		std::vector<int> getResources() const;
		
		void operator+=(const std::vector<int> productionIncrement);
};
