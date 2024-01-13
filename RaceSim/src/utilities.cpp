#include <stdio.h>
#include <utilities.h>
#include <assert.h>
#include <cassert>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

bool isDouble(std::string str) {
	if (str[0] == '-' && str.size() >= 2) {
		return isdigit(str[1]);
	}
	return isdigit(str[0]);
}
