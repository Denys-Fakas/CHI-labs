#pragma once

#include <iostream>
#include <string>
#include <fstream>

namespace l1
{
	class Txt
	{
	public:
		Txt() noexcept;				// Default constructor for the Txt class.
		explicit Txt(const std::string& filename);	// Constructor that reads text from a file
		Txt(const Txt& txt);		// Copy constructor
		Txt(Txt&& txt) noexcept;	// Move constructor
		~Txt();						// Destructor 

		Txt& operator=(const Txt& txt);		// Copy assignment operator
		Txt& operator=(Txt&& txt) noexcept;	// Move assignment operator

		inline size_t size() const noexcept { return m_size; };		// Returns the number of characters in the text.

	private:
		// Pointer to the dynamically allocated memory that holds the text data.
		std::string* m_data;

		// Number of characters in the text.
		size_t m_size;

		/* Helper function used by copy constructorand copy assignment operator
		to copy the contents of one Txt object to another.*/
		void Copy(const Txt& txt);

		/* Helper function used by move constructorand move assignment operator
		to move the contents of one Txt object to another.*/
		void Move(Txt&& txt) noexcept;
	};
}