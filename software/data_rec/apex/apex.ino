#include <simpleRPC.h>
#include "structFS/module.hpp"

#define KB 1024
#define MB (KB * KB)
#define DEFAULT_BUFFER_SIZE 2 * KB

File file(2*MB);

class FileReader {
public:
	FileReader(File& file): FileReader(file, DEFAULT_BUFFER_SIZE) {}
	FileReader(File& file, size_t buffer_size): file(file) {
		buffer = new char[buffer_size];
	}

	~FileReader() {
		delete buffer;
	}

	// The important part of this example
	// Takes in an offset and length, and gives you that part of the file
	char* read_file_section(size_t offset, size_t len) {
		file.read(offset, buffer, len);
		return buffer;
	}

private:
	char* buffer;
	File& file;
};

FileReader reader(file);

void setup(void) {
	Serial.begin(9600);
	FS fs = FS(4 * MB);
	if (!fs.AddFile(file)) {
		Serial.println("Couldn't add file");
	}
}

class LED {
public:
	LED() {}

	void setBrightness(int brightness) {}
};

LED led;

void loop(void) {
	// Each call to interface listens for 1 request and then responds accordingly
	interface(
		Serial,
		pack(&reader, &FileReader::read_file_section),
		F("read_file_section: Read part of a file. @data: offset, length. @return: The data in the file.")
	);
}
