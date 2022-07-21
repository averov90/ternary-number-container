#pragma once
#include <iostream>
#include <string>
#include <limits>

static const unsigned int MAX_EXP = log(UINT_MAX) / log(3) + 1;

class tnumber {
	char *start = nullptr, //Pointer to start of allocated memory
		*view; //Pointer to start of binary number representation

	//Initialize class variables (perform conversion)
	inline void init(unsigned int number) { 
		start = new char[33]; //Initializing array for store a binary number

		view = start + 32;

		*view = '\0';
		while (number != 0) {
			*--view = (number & 0x1) + '0';
			number >>= 1;
		}
	}

	//Raise an integer to an integer power
	inline unsigned int ipow(unsigned int base, unsigned char exp) {
		int result = 1;
		while(true) {
			if (exp & 1)
				result *= base;
			exp >>= 1;
			if (!exp)
				break;
			base *= base;
		}
		return result;
	}

	//Convert binary representation from *view to integer
	inline unsigned int to_int() {
		char *cp = view - 1, *end = start + 31;
		unsigned int result = 0;
		while (cp != end) {
			result <<= 1;
			result += *++cp - '0';
		}
		return result;
	}

public:
	//Constructor
	tnumber(unsigned int number) {
		init(number);
	}
	
	tnumber(const tnumber &another) {
		init(another.to_int());
	}
	
	//Assign operator
	tnumber& operator=(const tnumber &another) {
		if (&another == this) return *this;
		this->~tnumber();
		init(another.to_int());
		return *this;
	}
	
	tnumber& operator=(const unsigned int number) {
		this->~tnumber();
		init(number);
		return *this;
	}

	//Public wrapper for to_int()
	unsigned int get_decimal() {
		return to_int();
	}

	//Get binary representation in C-string
	const char *c_str() {
		return view;
	}

	//Get binary representation in std::string
	std::string get() {
		return std::string(view);
	}

	//Get ternary number representation form stored binary
	std::string get_3based() {
		unsigned int number = to_int();

		char *start3b = new char[33], *view3b;
		view3b = start3b + 32;

		*view3b = '\0';
		while (number != 0) {
			*--view3b = (number % 3) + '0';
			number /= 3;
		}
		
		auto ternary = std::string(view3b);
		delete[] start3b;
		return ternary;
	}

	//Compare given ternary representation with stored binary
	bool compare_with_3based(std::string based3) {
		if (*based3.c_str() == '0')
			throw("Not-meaning zeros must be deleted!");
			
		unsigned int number = to_int();

		std::string::const_iterator cp = based3.cbegin(), end = based3.cend();
		unsigned int result = 0, prew_result = 0, exp = based3.length();
		if (exp > MAX_EXP)
			throw("Given 3-based number exponent is too big!\nObviously, given 3-based number is not same as contained.");
		while (cp != end) {
			if (*cp < '0' || *cp > '2')
				throw("Not allowed character in 3-based string!");
			--exp;
			result += ipow(3, exp) * (*cp++ - '0');
			if (result < prew_result)
				throw("3-based number is too big! Overflow happened.\nObviously, given 3-based number is not same as contained.");
			prew_result = result;
		}

		return result == number;
	}

	friend std::ostream &operator <<(std::ostream &ostr, const tnumber &num);

	~tnumber() {
		delete[] start;
	}
};

std::ostream &operator<<(std::ostream &ostr, const tnumber &num) {
	ostr << num.view;
	return ostr;
}