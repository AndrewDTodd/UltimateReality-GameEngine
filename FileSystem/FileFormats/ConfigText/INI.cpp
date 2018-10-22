#include "INI.h"
#include "../../../StringTools/StringList.h"
#include "../../../MemoryAllocation/MemoryManager.h"
#include "../../FileManager.h"

namespace Ultrality
{
	namespace FileIO
	{
		Segment::Segment()
		{
			this->m_variableNamesList = nullptr;
			this->m_itemsConfiguredByVariables = nullptr;
		}
		Segment::~Segment()
		{
			this->m_name.~LocalString();
		}

		std::ostream& operator<<(std::ostream& stream, const Segment& segment)
		{
			stream << '[' << segment.m_name << ']' << '\n';
			for (unsigned __int8 i = 0; i < segment.m_filledVariables; i++)
			{
				stream << segment.m_variableNamesList[i] << '=';
				switch (*segment.m_variableNamesList[i].m_charArray)
				{
				case 'i':
					stream << *(int*)segment.m_itemsConfiguredByVariables[i] << '\n';
					break;
				case 'f':
					stream << *(float*)segment.m_itemsConfiguredByVariables[i] << '\n';
					break;
				case 'b':
					stream << *(bool*)segment.m_itemsConfiguredByVariables[i] << '\n';
					break;
				case 's':
					if ((segment.m_variableNamesList[i].m_charArray[1]) == 'l')
					{
						stream << *(StringTools::StringList*)segment.m_itemsConfiguredByVariables[i] << '\n';
						break;
					}
					else
					{
						stream << *(StringTools::LocalString*)segment.m_itemsConfiguredByVariables[i] << '\n';
						break;
					}
				}
			}

			return stream;
		}
		std::istream& operator>>(std::istream& stream, Segment& segment)
		{
			char tempArray[255] = {NULL};

			stream.seekg(1, std::ios::cur);

			stream.getline(tempArray, 255, ']');

			//segment.m_name = StringTools::LocalString(tempArray);

			stream.seekg(2, std::ios::cur);

			for (unsigned __int8 i = 0; i < segment.m_totalVariables; i++)
			{
				if (i != 0)
					stream.seekg(1, std::ios::cur);

				stream.getline(tempArray, 255, '=');

				//segment.m_variableNamesList[i] = StringTools::LocalString(tempArray);

				switch (*segment.m_variableNamesList[i].m_charArray)
				{
				case 'i':
					stream >> *(int*)segment.m_itemsConfiguredByVariables[i];
					if(i != segment.m_totalVariables - 1)
						stream.seekg(1, std::ios::cur);
					break;
				case 'f':  
					stream >> *(float*)segment.m_itemsConfiguredByVariables[i];
					if (i != segment.m_totalVariables - 1)
						stream.seekg(1, std::ios::cur);
					break;
				case 'b':  
					stream >> *(bool*)segment.m_itemsConfiguredByVariables[i];
					if (i != segment.m_totalVariables - 1)
						stream.seekg(1, std::ios::cur);
					break;
				case 's':
					if (segment.m_variableNamesList[i].m_charArray[1] == 'l')
					{
						stream >> *(StringTools::StringList*)segment.m_itemsConfiguredByVariables[i];
						if (i != segment.m_totalVariables - 1)
							stream.seekg(-1, std::ios::cur);
						break;
					}
					else
					{
						stream >> *(StringTools::LocalString*)segment.m_itemsConfiguredByVariables[i];
						if (i != segment.m_totalVariables - 1)
							stream.seekg(-1, std::ios::cur);
						break;
					}
				}
			}
			return stream;
		}

#ifdef DEBUG
		unsigned __int16 INI::m_numOfItemsMade = 0;
		unsigned __int8 INI::m_numOfSegments = 0;
#endif // DEBUG

		//defines the char used to separate variables from data (=)
		char INI::m_DelimiterOne = '=';
		//defines the char used to separate data elements of a list
		char INI::m_DelimiterTwo = ',';

		INI::INI(StringTools::LocalString&& fileName, StringTools::LocalString&& fileLocation, unsigned __int8&& numOfSegments)
			:FileFormat((StringTools::LocalString&&)fileName, (StringTools::LocalString&&)fileLocation)
		{
			this->p_FileManager = FileManager::getInstance();
			this->m_fileExtension = StringTools::LocalString(".INI");
			this->m_format = StorageFormat::TEXT;
			this->m_segments = Memory::allocateArray<Segment>(this->p_FileManager->m_INIsegmentAllocator, numOfSegments);
			this->m_totalSegments = numOfSegments;
		}

		INI::~INI()
		{
			if (this->m_segments)
			{
				for (unsigned __int8 i = 0; i < this->m_segmentsDefined; i++)
				{
					if (this->m_segments[i].m_variableNamesList)
					{
						Memory::deallocateArray<StringTools::LocalString>(this->p_FileManager->m_INIsegmentNameAllocator, this->m_segments[i].m_variableNamesList);
						Memory::deallocateArray<void*>(this->p_FileManager->m_INIsegmentPointerAllocator, this->m_segments[i].m_itemsConfiguredByVariables);
					}
				}

				Memory::deallocateArray<Segment>(this->p_FileManager->m_INIsegmentAllocator, this->m_segments);
				this->m_segments = nullptr;
				this->m_segmentsDefined = 0;
				this->m_totalSegments = 0;
			}
		}

		void INI::DefineSegment(StringTools::LocalString&& segmentName, const unsigned __int8& numOfVariables)
		{
			if (this->m_segmentsDefined != this->m_totalSegments)
			{
				this->m_segments[this->m_segmentsDefined].m_name = (StringTools::LocalString&&)(segmentName);
				this->m_segments[this->m_segmentsDefined].m_variableNamesList = Memory::allocateArray<StringTools::LocalString>(this->p_FileManager->m_INIsegmentNameAllocator, numOfVariables);
				this->m_segments[this->m_segmentsDefined].m_itemsConfiguredByVariables = Memory::allocateArray<void*>(this->p_FileManager->m_INIsegmentPointerAllocator, numOfVariables);
				this->m_segments[this->m_segmentsDefined].m_totalVariables = numOfVariables;
				this->m_segmentsDefined++;
			}
#ifdef DEBUG
			else
				std::cout << "All segments of this configuration file are in use" << std::endl;
#endif // DEBUG
		}

		void INI::DefineVariable(const unsigned __int8& indexOfSegment, StringTools::LocalString&& nameOfVariable, void* dataPointedTo)
		{
			if (this->m_segments[indexOfSegment].m_variableNamesList)
			{
				this->m_segments[indexOfSegment].m_variableNamesList[this->m_segments[indexOfSegment].m_filledVariables] = (StringTools::LocalString&&)(nameOfVariable);
				this->m_segments[indexOfSegment].m_itemsConfiguredByVariables[this->m_segments[indexOfSegment].m_filledVariables] = dataPointedTo;
				this->m_segments[indexOfSegment].m_filledVariables++;
			}
#ifdef DEBUG
			else
				std::cout << "This Segment is not established";
#endif // DEBUG

		}

		std::ostream& INI::write(std::ostream& stream)
		{
			for (unsigned __int8 i = 0; i < this->m_segmentsDefined; i++)
			{
				stream << this->m_segments[i];
			}

			return stream;
		}
		std::istream& INI::read(std::istream& stream)
		{

			for (unsigned __int8 i = 0; i < this->m_segmentsDefined; i++)
			{
				stream >> this->m_segments[i];
				
				stream.seekg(2, std::ios::cur);
			}

			return stream;
		}
	}
}