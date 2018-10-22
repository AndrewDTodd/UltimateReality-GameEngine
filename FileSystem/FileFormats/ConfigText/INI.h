#pragma once

#include "../../FileSystem.h"
#include <iostream>

namespace Ultrality
{
	namespace FileIO
	{
		class Segment
		{
			friend class INI;
		private:
			StringTools::LocalString m_name;
			StringTools::LocalString* m_variableNamesList = nullptr;
			void** m_itemsConfiguredByVariables = nullptr;
			unsigned __int8 m_filledVariables = 0;
			unsigned __int8 m_totalVariables = 0;
		public:
			Segment();
			~Segment();
		
			friend std::ostream& operator<<(std::ostream& stream, const Segment& segment);
			friend std::istream& operator>>(std::istream& stream, Segment& segment);
		};

		class INI : public FileFormat
		{
			friend class Segment;
			friend class FileManager;
		private:
#ifdef DEBUG
			static unsigned __int16 m_numOfItemsMade;
			static unsigned __int8 m_numOfSegments;
#endif // DEBUG
			//list of managers this system is dependent on

			//file manager pointer
			FileManager* p_FileManager = nullptr;

			//defines the char used to sperate tokens in a line
			static char m_DelimiterOne;
			//defines the char that may seperate multiple listed values
			static char m_DelimiterTwo;

			//per file objects
			Segment* m_segments = nullptr;
			unsigned __int8 m_segmentsDefined = 0;
			unsigned __int8 m_totalSegments = 0;

		public:
			//constructor defines the files name, and the number of configuration segments within the file
			//use the DefineSegment methode, after construction to construct the segments, in the order they will appear in the file
			INI(StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation, unsigned __int8&& numOfSegments);
			~INI();

			//is used to define the segment's name, and the number of variables represented in that segment
			//use after construction
			//filles the next avalable slot in the array, whose length is defined during construction
			void DefineSegment(StringTools::LocalString&& segmentName, const unsigned __int8& numOfVariables);

			//defines the name of a variable, and the data that variable is pointed to for data storage or configuration
			//define integers with the "i" prefix
			//define floats with the "f" prefix
			//define bools with the "b" prefix
			//define strings with the "s" prefix
			//define string lists with the "sl" prefix
			//data pointed to should be of the associated type
			void DefineVariable(const unsigned __int8& indexOfSegment, StringTools::LocalString&& nameOfVariable, void* dataPointedTo);

			std::ostream& write(std::ostream& stream);
			std::istream& read(std::istream& stream);
		};
	}
}
