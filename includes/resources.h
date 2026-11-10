#pragma once
#include "includes.h"

class Resources
{
	private:
		int riceStock = 500;
		int timberStock = 200;
		int ironStock = 100;
		int goldStock = 1000;
	
	public:
		std::vector<int> getResources() const;
		
		void operator+=(const std::vector<int> productionIncrement);
};
