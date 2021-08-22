#include "datafw/ImageData.h"
#include <string.h>

using namespace uit::datafw;
ImageData::ImageData()
	: m_pData(nullptr)
	, m_Bytes(0)
	, m_Width(0)
	, m_Height(0)
	, m_Format(invalidFormat)
	, m_FormatKnown(false)
{

}

ImageData::ImageData(const uint8_t *pData, uint32_t bytes)
	: m_pData(nullptr)
{
	load(pData, bytes);
}

ImageData::ImageData(const uint8_t *pData, uint32_t width, uint32_t height, ImageFormat format)
	: m_pData(nullptr)
{
	load(pData, width, height, format);
}

ImageData::~ImageData()
{
	delete [] m_pData;
}

void ImageData::load(const uint8_t *pData, uint32_t bytes)
{
	delete[]m_pData;
	m_pData = new uint8_t[bytes];
	memcpy(m_pData, pData, bytes);
	m_Bytes = bytes;
	m_Width = 0;
	m_Height = 0;
	m_Format = invalidFormat;
	m_FormatKnown = false;
}

void ImageData::load(const uint8_t *pData, uint32_t width, uint32_t height, ImageFormat format)
{
	delete[]m_pData;
	uint32_t bytes = 0;
	switch (format)
	{
	case rgb555Format:		bytes = (width * 2 + 3) / 4 * 4 * height;	break;
	case rgb565Format:		bytes = (width * 2 + 3) / 4 * 4 * height;	break;
	case rgb888Format:		bytes = (width * 3 + 3) / 4 * 4 * height;	break;
	case rgb8888Format:		bytes = (width * 4 + 3) / 4 * 4 * height;	break;
	default:				bytes = 0;									break;
	}
	m_pData = new uint8_t[bytes];
	memcpy(m_pData, pData, bytes);
	m_Bytes = bytes;
	m_Width = width;
	m_Height = height;
	m_Format = format;
	m_FormatKnown = true;
}

uint8_t *ImageData::data()
{
	return m_pData;
}

const uint8_t *ImageData::getData() const
{
	return m_pData;
}

uint32_t ImageData::bytes() const
{
	return m_Bytes;
}

uint32_t ImageData::width() const
{
	return m_Width;
}

uint32_t ImageData::height() const
{
	return m_Height;
}

ImageFormat ImageData::format() const
{
	return m_Format;
}

bool ImageData::isFormatKnown() const
{
	return m_FormatKnown;
}

ImageData::ImageData(const ImageData &other)
	: m_pData(nullptr)
	, m_Bytes(other.m_Bytes)
	, m_Width(other.m_Width)
	, m_Height(other.m_Height)
	, m_Format(other.m_Format)
	, m_FormatKnown(other.m_FormatKnown)
{
	m_pData = new uint8_t[m_Bytes];
	memcpy(m_pData, other.m_pData, m_Bytes);
}

void ImageData::operator = (const ImageData &other)
{
	delete []m_pData;
	m_pData = new uint8_t[other.m_Bytes];
	memcpy(m_pData, other.m_pData, other.m_Bytes);
	m_Bytes = other.m_Bytes;
	m_Width = other.m_Width;
	m_Height = other.m_Height;
	m_Format = other.m_Format;
	m_FormatKnown = other.m_FormatKnown;
}
