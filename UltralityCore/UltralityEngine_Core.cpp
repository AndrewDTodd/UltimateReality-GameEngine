#include "UltralityEngine_Core.h"

namespace Ultrality
{
	UltralityEngine_Core* UltralityEngine_Core::p_SingletonInstace = nullptr;

	UltralityEngine_Core::UltralityEngine_Core()
	{

	}
	UltralityEngine_Core::~UltralityEngine_Core()
	{

	}

	UltralityEngine_Core* UltralityEngine_Core::getInstance()
	{
		if (!UltralityEngine_Core::p_SingletonInstace)
		{
			static UltralityEngine_Core s_singleton;
			return &s_singleton;
		}
		else
		{
			return UltralityEngine_Core::p_SingletonInstace;
		}
	}

	void UltralityEngine_Core::startUp()
	{
		this->s_MemoryManager = Memory::MemoryManager::getInstance();
		this->s_MemoryManager->startUp();

		this->s_StringManager = StringTools::StringManager::getInstance();
		this->s_StringManager->startUp();

		this->s_FileManager = FileIO::FileManager::getInstance();
		this->s_FileManager->startUp();


		UltralityEngine_Core::p_SingletonInstace = this;
	}

	void UltralityEngine_Core::shutDown()
	{
		this->s_FileManager->shutDown();
		this->s_FileManager = nullptr;

		this->s_StringManager->shutDown();
		this->s_StringManager = nullptr;

		this->s_MemoryManager->shutDown();
		this->s_MemoryManager = nullptr;

		UltralityEngine_Core::p_SingletonInstace = nullptr;

		this->~UltralityEngine_Core();
	}

	void UltralityEngine_Core::run(bool* run)
	{
		while (*run)
		{

		}
	}
}