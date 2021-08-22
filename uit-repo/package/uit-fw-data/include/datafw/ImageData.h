#pragma once
#include "Type.h"

namespace uit { namespace datafw {

class UIT_API ImageData final
{
public:
	//构建一个空的图片数据
	ImageData();

	//构建一个图片数据，它的数据头为pData，长度为bytes
	ImageData(const uint8_t *pData, uint32_t bytes);

	//构建一个图片数据，它的数据头为pData，宽高为width/height，它的格式为format
	ImageData(const uint8_t *pData, uint32_t width, uint32_t height, ImageFormat format);
	~ImageData();

	//加载新的数据，并覆盖原来的数据
	void load(const uint8_t *pData, uint32_t bytes);
	void load(const uint8_t *pData, uint32_t width, uint32_t height, ImageFormat format);

	//获取数据头
	uint8_t *data();
	const uint8_t *getData() const;

	//获取字节数
	uint32_t bytes() const;

	//获取宽高
	uint32_t width() const;
	uint32_t height() const;

	//获取图片格式
	ImageFormat format() const;

	//是否是已知格式的图片数据
	bool isFormatKnown() const;

public:
	ImageData(const ImageData &other);
	void operator = (const ImageData &other);
		
private:
	uint8_t				*m_pData;
	uint32_t			m_Bytes;
	uint32_t			m_Width;
	uint32_t			m_Height;
	ImageFormat			m_Format;
	bool				m_FormatKnown;
};

}}