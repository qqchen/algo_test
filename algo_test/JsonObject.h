#ifndef JSON_OBJECT_H
#define JSON+OBJECT_H
#include "json/json.h"
#include <fstream>
#include <vector>
#include <string>

class Serializable
{
public:
	virtual Json::Value serialize() = 0;
	virtual bool deserialize(Json::Value root) = 0;
};

class JsonObject : public Serializable
{
public:
	JsonObject()
	{
		init_keys();
	}

	virtual void write(std::string& file)
	{
		Json::Value root = serialize();
		std::string json_file = writer.write(root);
		std::ofstream ofs;
		ofs.open(file);
		if (ofs.is_open())
		{
			ofs << json_file;
		}
		ofs.close();
	}

	virtual bool read(std::string& file)
	{
		std::ifstream ifs;
		ifs.open(file);
		Json::Value root;
		bool res = false;
		if (reader.parse(ifs, root, false))
		{
			res = deserialize(root);
		}
		ifs.close();
		return res;
	}

	virtual void init_keys() {}

	virtual bool parse_error(Json::Value root)
	{
		bool error = false;
		for (size_t i = 0; i < m_keys.size(); ++i)
		{
			if (root[m_keys[i]].isNull())
			{
				error = true;
				break;
			}
		}
		return error;
	};

protected:
	Json::FastWriter writer;
	Json::Reader reader;
	std::vector<std::string> m_keys;
};


#endif