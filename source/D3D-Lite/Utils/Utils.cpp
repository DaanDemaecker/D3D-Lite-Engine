// Utils.cpp

// File includes
#include "Utils.h"

std::vector<char> Utils::readFile(const std::string& filename)
{
	// Open file
	std::ifstream file{ filename, std::ios::ate | std::ios::binary };

	// If file failed to open, throw runtime error
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file " + filename);
	}

	// Because of the std::ios::ate flag, the file will be read from the end, getting the current position in the file will therefore give its size
	size_t fileSize = static_cast<size_t>(file.tellg());

	// Char buffer created with size of the file
	std::vector<char> buffer(fileSize);

	// Now that we have the size of the file, we can move back to the start and store the data into the buffer
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	// Close the file
	file.close();

	// Return the buffer
	return buffer;
}