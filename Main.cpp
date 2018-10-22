#include "UltralityCore\UltralityEngine.h"
#include "UltralityCore\UltralityEngine_Core.h"

//this is just for testing purposes, the final build mode of the Core is a static library
int main()
{
	using namespace Ultrality;

	UltralityEngine_Core* gs_Core;
		
	gs_Core = UltralityEngine_Core::getInstance();

	gs_Core->startUp();

	StringTools::StringTable table;

	FileIO::UltraText UtextOne(&table, StringTools::LocalString("/UltraTextOne"), StringTools::LocalString(".."));

	FileIO::FileRequest request(FileIO::READ, FileIO::IMMEDIATE, &UtextOne);

	request.WaitForFile();

	std::cout << table << std::endl;

	table.~StringTable();

	gs_Core->shutDown();

	system("PAUSE");

	return 0;

	system("pause");
}