#include "Txt.h"

namespace l1
{
	void Txt::Copy(const Txt& txt)
	{
		m_size = txt.m_size;
		m_data = new std::string[m_size];
		for (size_t i{}; i < m_size; ++i)
			m_data[i] = txt.m_data[i];

	}

	void Txt::Move(Txt&& txt) noexcept
	{
		m_size = txt.m_size;
		m_data = txt.m_data;
		txt.m_size = 0;
		txt.m_data = nullptr;
	}

	Txt::Txt() noexcept : m_size(0), m_data(nullptr)
	{

	}

	Txt::Txt(const std::string& filename) : m_size(0), m_data(nullptr)
	{
		std::ifstream file(filename);
		if (file.is_open())
		{
			std::cout << "Open file: " << filename;
			std::string str;
			while (getline(file, str))
				++m_size;
			file.clear();
			file.seekg(0, std::ios::beg);

			m_data = new std::string[m_size];
			for (size_t i{}; i < m_size; ++i)
				getline(file, m_data[i]);
			file.close();
		}
		else
			std::cerr << "Can't open file: " << filename;
	}

	Txt::Txt(const Txt& txt)
	{
		Copy(txt);
	}

	Txt::Txt(Txt&& txt) noexcept
	{
		Move(std::move(txt));
	}

	Txt::~Txt()
	{
		delete[] m_data;
	}

	Txt& Txt::operator=(const Txt& txt)
	{
		if (this != &txt)
		{
			delete[] m_data;
			Copy(txt);
		}
		return *this;
	}

	Txt& Txt::operator=(Txt&& txt) noexcept
	{
		if (this != &txt)
		{
			delete[] m_data;
			Move(std::move(txt));
		}
		return *this;
	}
}
