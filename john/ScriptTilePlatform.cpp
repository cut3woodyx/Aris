#include "ScriptTilePlatform.h"

namespace FwEngine
{
	ScriptTilePlatform::ScriptTilePlatform(ComponentScript& comp)
		: IScript(STRING_SCRIPT_TILEPLATFORM, comp),
		_tileMap{ _components.getSystemManager()._map},
		_posX{ 0 },
		_posY{ 0 },
		_width{ 0 },
		_height{ 0 },
		_textureID{ 0 },
		_columns{}
	{
		AddIntVar("Start X", &_posX);
		AddIntVar("Start Y", &_posY);
		AddIntVar("Width", &_width);
		AddIntVar("Height", &_height);

		AddStringVar("Texture File Path", &_textureName);
	}

	ScriptTilePlatform::~ScriptTilePlatform()
	{
	}

	void ScriptTilePlatform::GenerateTiles()
	{
		_columns.clear();

		for (int i = _posX; i < _posX + _width; ++i)
		{
			std::vector<int> column;
			for (int j = _posY; j < _posY + _height; ++j)
			{
				int yAxis = j * _tileMap._width + i;
				_tileMap._mapArray[yAxis] = _textureID;
				_tileMap._mapArrayCol[yAxis] = 1;
				column.push_back(yAxis);
			}
			_columns.push_back(column);
		}
	}

	void ScriptTilePlatform::Init()
	{
		if (_posX < 0)
			_posX = 0;
		if (_posY < 0)
			_posY = 0;
		if (_width < 0)
			_width = 0;
		if (_height < 0)
			_height = 0;

		if (_tileMap._width < _width + _posX)
		{
			_width = _tileMap._width - _posX;
		}
		if (_tileMap._height < _height + _posY)
		{
			_height = _tileMap._width - _posY;
		}

		if (_textureName.size())
		{
			GLuint tex = RESOURCE->GetTexture(_textureName);

			for (int i = 0; i < _tileMap._tileTex.size(); ++i)
			{
				if (_tileMap._tileTex[i] == tex)
				{
					_textureID = i + 10;
				}
			}
			if (!_textureID)
			{
				_tileMap._tileTex.push_back(tex);
				_textureID = (int)_tileMap._tileTex.size() + 9;
			}
		}
		else
		{
			_textureID = _tileMap._mapArray[_posY * _tileMap._width + _posX];
		}

		GenerateTiles();
	}

	void ScriptTilePlatform::Update(float)
	{
	}

	void ScriptTilePlatform::Free()
	{
	}

	void ScriptTilePlatform::Destroy()
	{
	}

	void ScriptTilePlatform::OnCollisionEnter(size_t)
	{
	}

	void ScriptTilePlatform::OnCollisionExit(size_t)
	{
	}

	void ScriptTilePlatform::OnCollisionStay(size_t)
	{
	}

	void ScriptTilePlatform::PushVars()
	{
		if (_originalVars.size())
		{
			_posX = std::stoi(_originalVars[0]);
			_originalVars.erase(_originalVars.begin());
		}
		if (_originalVars.size())
		{
			_posY = std::stoi(_originalVars[0]);
			_originalVars.erase(_originalVars.begin());
		}
		if (_originalVars.size())
		{
			_width = std::stoi(_originalVars[0]);
			_originalVars.erase(_originalVars.begin());
		}
		if (_originalVars.size())
		{
			_height = std::stoi(_originalVars[0]);
			_originalVars.erase(_originalVars.begin());
		}

		if (_originalVars.size())
		{
			_textureName = _originalVars[0];
			_originalVars.erase(_originalVars.begin());
		}
	}

	void ScriptTilePlatform::BreakLeftColumn()
	{
		if (_columns.size())
		{
			_posX++;
			_width--;
			std::vector<int> coords = _columns[0];

			_columns.erase(std::begin(_columns));

			for (int i = 0; i < coords.size(); ++i)
			{
				_tileMap._mapArray[coords[i]] = 0;
				_tileMap._mapArrayCol[coords[i]] = 0;
			}
		}
	}
	void ScriptTilePlatform::BreakRightColumn()
	{
		if (_columns.size())
		{
			_width--;
			std::vector<int> coords = *(std::end(_columns) - 1);

			_columns.erase(std::end(_columns)-1);

			for (int i = 0; i < coords.size(); ++i)
			{
				_tileMap._mapArray[coords[i]] = 0;
				_tileMap._mapArrayCol[coords[i]] = 0;
			}
		}
	}
	void ScriptTilePlatform::BreakTopRow()
	{
		if (_columns.size())
		{
			_height--;

			for (int i = 0; i < _columns.size(); ++i)
			{
				int h = *(std::end(_columns[i]) - 1);
				_columns[i].erase(std::end(_columns[i]) - 1);
				_tileMap._mapArray[h] = 0;
				_tileMap._mapArrayCol[h] = 0;
			}
		}
	}
	void ScriptTilePlatform::BreakBottomRow()
	{
		if (_columns.size())
		{
			_height--;
			_posY++;

			for (int i = 0; i < _columns.size(); ++i)
			{
				int h = *(std::begin(_columns[i]));
				_columns[i].erase(std::begin(_columns[i]));
				_tileMap._mapArray[h] = 0;
				_tileMap._mapArrayCol[h] = 0;
			}
		}
	}

	void ScriptTilePlatform::MakeLeftColumn()
	{
		if (_posX)
		{
			_width++;
			_posX--;

			int i = _posX;
			std::vector<int> column;
			for (int j = _posY; j < _posY + _height; ++j)
			{
				int yAxis = j * _tileMap._width + i;
				_tileMap._mapArray[yAxis] = _textureID;
				_tileMap._mapArrayCol[yAxis] = 1;
				column.push_back(yAxis);
			}
			_columns.insert(std::begin(_columns), column);
		}
		else
			MakeRightColumn();
	}
	void ScriptTilePlatform::MakeRightColumn()
	{
		if (_tileMap._width >= _width + _posX + 1)
		{
			_width++;

			int i = _posX + _width - 1;
			std::vector<int> column;
			for (int j = _posY; j < _posY + _height; ++j)
			{
				int yAxis = j * _tileMap._width + i;
				_tileMap._mapArray[yAxis] = _textureID;
				_tileMap._mapArrayCol[yAxis] = 1;
				column.push_back(yAxis);
			}
			_columns.push_back(column);
		}
	}
	void ScriptTilePlatform::MakeTopRow()
	{
		if (_tileMap._height >= _height + _posY + 1)
		{
			_height++;

			for (int i = _posX; i < _posX + _width; ++i)
			{
				int yAxis = (_height - 1) * _tileMap._width + i;
				for (int j = 0; j < _columns.size(); ++j)
				{
					_columns[i].push_back(yAxis);
					_tileMap._mapArray[yAxis] = _textureID;
					_tileMap._mapArrayCol[yAxis] = 1;
				}
			}
		}
	}
	void ScriptTilePlatform::MakeBottomRow()
	{
		if (_posY)
		{
			_height++;
			_posY--;

			for (int i = _posX; i < _posX + _width; ++i)
			{
				int yAxis = (_posY) * _tileMap._width + i;
				for (int j = 0; j < _columns.size(); ++j)
				{
					_columns[i].insert(std::begin(_columns[i]), yAxis);
					_tileMap._mapArray[yAxis] = _textureID;
					_tileMap._mapArrayCol[yAxis] = 1;
				}
			}
		}
		else
			MakeTopRow();
	}

	int ScriptTilePlatform::GetPositionX()
	{
		return _posX;
	}
	int ScriptTilePlatform::GetPositionY()
	{
		return _posY;
	}
	int ScriptTilePlatform::GetHeight()
	{
		return _height;
	}
	int ScriptTilePlatform::GetWidth()
	{
		return _width;
	}
}