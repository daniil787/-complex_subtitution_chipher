

#include <iostream> 
#include <string> 
#include<cctype>  
#include<fstream>
#include<stdexcept>
#include<algorithm>

std::string readFile(const std::string& filename) {

	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		throw std::runtime_error("\nError while opening the file!\n");
	}

	//determining the file size
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string text(size, '\0');

	if (!file.read(&text[0], size)) {
		throw std::runtime_error("\nError while reading the file\n");
	}

	file.close();
	return text;
}


void writeFile(const std::string& filename, const std::string& text) {

	std::ofstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("\nError while opening the file\n");
	}

	if (!file.write(text.c_str(), text.size())) {
		throw std::runtime_error("\nError while writing the file\n");
	}

	file.close();
}

std::string encrypt(const std::string& data, const std::string& key, bool encrypt = true) {

	std::string result = data;

	std::size_t sizeKey = key.size();
	//going through each letter(i) of the text and changing it through the letters of the key(j)
	for (std::size_t i = 0, j = 0; i < data.size(); ++i) {

		if (std::isalpha(data[i])) {
			// the first letter of the alphabet
			char base = std::islower(data[i]) ? 'a' : 'A';

			//the letter in the key is chosen so that only the letters of the key are read if it is duplicated
			char keyChar = key[j % sizeKey];

			//conversion of keyChar into a position number in the alphabet
			keyChar = tolower(keyChar) - 'a';

			if (encrypt) {
				// finding the position in the alphabet, taking into account the shift "to the right" to the position of the letter of the key
				result[i] = static_cast<char>((data[i] - base + keyChar + 26) % 26 + base);
			}
			else {
				// for decryption shift "left" to the position of the letter of the key
				result[i] = static_cast<char>((data[i] - base - keyChar + 26) % 26 + base);

			}
			j++;
		}

	}

	return result;
}

int main() {

	char choice;

	while (true) {
		try {
			//entering the name of the file to be decrypted or encrypted
			std::string fileName;
			std::cout << "\nEnter name of file: ";

			std::cin >> fileName;
			std::cin.ignore();

			//reading into the variable "data" the contents of the file
			std::string data = readFile(fileName);


			//key input - words or pairs of words (the otsup between them will be deleted)
			std::string key;
			std::cout << "Enter a key: ";
			std::getline(std::cin, key);

			// removing indents in the key, if any
			key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

			// mode selection: encryption\decryption
			char mode;
			std::cout << "Enter a mode: e - encryption, d - decryption: ";
			std::cin >> mode;

			bool encryptes = (mode == 'e');

			if (mode != 'e' && mode != 'd') {
				throw std::invalid_argument("\nIncorrect mode was entered! Use 'e' or 'd'\n");
			}

			//encrypt/decrypt
			std::string result = encrypt(data, key, encryptes);

			writeFile(fileName, result);

			mode == 'e' ? std::cout << "\nEncrypted was complete" << std::endl : std::cout << "\nDecrypted was complete" << std::endl;

			std::cout << "\nContinue (y/n)? ";

			std::cin >> choice;
			choice = std::tolower(choice);

			if (choice == 'n') {
				break;
			}

		}
		catch (const std::exception& er) {

			std::cerr << "Error: " << er.what() << std::endl;

		}
	}
	return 0;
}