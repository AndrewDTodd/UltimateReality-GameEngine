#pragma once

#include "../MemoryAllocation/MemoryManager.h"
#include "../FileSystem/FileManager.h"
#include "../StringTools/StringManager.h"

namespace Ultrality
{
	class UltralityEngine_Core
	{
	private:
		//pointer used to track singleton instance
		static UltralityEngine_Core* p_SingletonInstace;

		UltralityEngine_Core();
		~UltralityEngine_Core();

	public:
		//Memory Manager
		Memory::MemoryManager* s_MemoryManager = nullptr;
		//String Manager
		StringTools::StringManager* s_StringManager = nullptr;
		//File Manager
		FileIO::FileManager* s_FileManager = nullptr;

		//use to construct a singleton instance
		static UltralityEngine_Core* getInstance();

		//use to start up the Engine
		void startUp();

		//use to shut down the Engine
		void shutDown();

		void run(bool* run);

		//do not attempt to copy, manager is a singleton
		UltralityEngine_Core(const UltralityEngine_Core& manager) = delete;
		//do not attempt to copy, manager is a singleton
		void operator=(const UltralityEngine_Core& manager) = delete;
		//do not attempt to copy, manager is a singleton
		UltralityEngine_Core(UltralityEngine_Core&& manager) = delete;
		//do not attempt to copy, manager is a singleton
		void operator=(UltralityEngine_Core&& manager) = delete;
	};
}