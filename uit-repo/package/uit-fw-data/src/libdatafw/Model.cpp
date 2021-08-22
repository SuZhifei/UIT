#include "datafw/Model.h"
#include "Model_Internal.h"
#include "Share.h"
#include "datafw/Parser.h"

using namespace uit::datafw;
using namespace v1::uit::datafw;
Model::Model()
{
	m_internal = std::make_shared<Model_Internal>(this);
}

bool Model::configDataEventDir(const std::string &sDir)
{
	return m_internal->configDataEventDir(sDir);
}

std::string Model::getDataEventDir() const
{
	return m_internal->getDataEventDir();
}

void Model::enableSocketDebugHelper(bool bEnable)
{
	m_internal->enableSocketDebugHelper(bEnable);
}

bool Model::isSocketDebugHelperEnable() const
{
	return m_internal->isSocketDebugHelperEnable();
}

bool Model::startup()
{
	return m_internal->startup();
}

bool Model::stop()
{
	return m_internal->stop();
}

Parser &Model::parser()
{
	return m_internal->parser();
}

const Parser &Model::getParser() const
{
	return m_internal->getParser();
}

bool Model::postEvent(const std::string & sEventName, Poco::JSON::Object::Ptr args)
{
	try {
		std::string sJson = Poco::Dynamic::Var(args).toString();
		m_internal->postEvent(sEventName, sJson);
	}
	catch (...) {
		return false;
	}
	return true;
}

void Model::setBoolean(const std::string & path, bool b)
{
	m_internal->setBoolean(path, b, uit::getTickCount());
}

void Model::setInteger(const std::string & path, int32_t n)
{
	m_internal->setInteger(path, n, uit::getTickCount());
}

void Model::setReal(const std::string & path, double d)
{
	m_internal->setReal(path, d, uit::getTickCount());
}

void Model::setString(const std::string & path, const std::string & str)
{
	m_internal->setString(path, str, uit::getTickCount());
}

void Model::setEnum(const std::string & path, int32_t e)
{
	m_internal->setEnum(path, e, uit::getTickCount());
}

void Model::setEnumStringValue(const std::string & path, const std::string sStringValue)
{
	m_internal->setEnumStringValue(path, sStringValue, uit::getTickCount());
}

bool Model::setImage(const std::string & path, const ImageData & image)
{
	if (image.bytes() > DATAFW_IMAGE_SIZE_LIMIT)
		return false;

	m_internal->setImage(path, Types::Image("", image.isFormatKnown(), image.format(), image.width(), image.height(), CommonAPI::ByteBuffer(image.getData(), image.getData() + image.bytes())), uit::getTickCount());
	return true;
}

void Model::updateListItems(const std::string &path, const std::set<uint32_t> &indexs, Poco::JSON::Array::Ptr items)
{
	if (indexs.empty() || !items || items->size() == 0)	return;

	//检查图片，分类存放
	std::vector<Types::Image> images;
	for (int32_t i = 0; i != items->size(); ++i)
	{
		Poco::JSON::Object::Ptr item = items->getObject(i);
		for (Poco::JSON::Object::ConstIterator iter = item->begin(); iter != item->end(); ++iter)
		{
			const std::string &sName = iter->first;
			Poco::Dynamic::Var v = item->get(sName);
			if (v.type() == typeid(ImageData))
			{
				const ImageData &imageData = v.extract<ImageData>();
				images.push_back(Types::Image("", imageData.isFormatKnown(), imageData.format(), imageData.width(), imageData.height(), CommonAPI::ByteBuffer(imageData.getData(), imageData.getData() + imageData.bytes())));
				item->set(sName, Parser::makeDefault(imageValue));
			}
		}
	}
	std::vector<uint32_t> ids(indexs.begin(), indexs.end());
	std::string sJson = Poco::Dynamic::Var(items).toString();
	m_internal->updateListItems(path, ids, sJson, images);
}

void Model::resizeList(const std::string & path, uint32_t size)
{
	m_internal->resizeList(path, size);
}

void Model::insertListItems(const std::string & path, uint32_t index, uint32_t count)
{
	m_internal->insertListItems(path, index, count);
}

void Model::removeListItems(const std::string & path, uint32_t index, uint32_t count)
{
	m_internal->removeListItems(path, index, count);
}

void Model::updateListViewport(const std::string & path, uint32_t first)
{
	m_internal->updateListViewport(path, first);
}

void Model::checkListItem(const std::string & path, int32_t index)
{
	m_internal->checkListItem(path, index);
}

void Model::focusListItem(const std::string & path, int32_t index)
{
	m_internal->focusListItem(path, index);
}

void Model::vkey(int32_t key, int32_t state)
{
	m_internal->vkey(key, state);
}
