#include <map>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>

using namespace std;


class asset
{
	const string path_;

public:

	asset(const string& path) : path_(path) {}

	const string& get_path() const { return path_; }

	virtual const char* read() const = 0;
	virtual ~asset() = default;
};


class text_asset : public asset
{
	string text_;

public:

	text_asset(const string& path) : asset(path) //конструктор текст ассета з файловими перевірками та операціями
	{
		ifstream file(path);

		if (!file.is_open())
		{
			text_ = "File not found";
			return;
		}

		string line;

		while (getline(file, line))
		{
			text_ += line + "\n";
		}

		if (text_.empty())
		{
			text_ = "Empty file";
		}
	}

	const char* read() const override //читання файлу (const бо лише читання)
	{
		return &text_[0];
	}

	~text_asset() override = default;
};

class asset_manager 
{
	map<string, weak_ptr<asset>> assets_; //строка - чи загружена вона

public:

	shared_ptr<asset> get(const string& path); //розумний показжчик який керує часом існування об'єкту
};

shared_ptr<asset> asset_manager::get(const string& path) //отримати ассети
{
	for (auto it = assets_.begin(); it != assets_.end();) //чи є "мертві" ассети у мапі
	{
		if (it->second.expired())
			it = assets_.erase(it);
		else
			++it;
	}
	weak_ptr<asset>& weak_asset = assets_[path];

	if (!weak_asset.expired()) //перевірка на "мертвий" ассет
	{
		return weak_asset.lock();
	}

	shared_ptr<asset> new_asset = make_shared<text_asset>(path);

	weak_asset = new_asset;

	return new_asset;
}

void PrintOpenAssets(const vector<shared_ptr<asset>>& open_assets)//виводить ассети
{
	if (open_assets.empty())
	{
		cout << "No open assets" << endl;
		return;
	}

	for (int i = 0; i < open_assets.size(); i++)
	{
		cout << i + 1 << ". " << open_assets[i]->get_path() << endl;
	}
}


int main()
{
	asset_manager asset_manager;
	vector<shared_ptr<asset>> open_assets;

	// 1. Load Asset
	// 2. Show loaded assets
	// 3. Read loaded asset
	// 4. Unload asset
	// 5. Exit


	bool isRunning = true;
	while (isRunning)
	{
		cout << endl;
		cout << "1. Load Asset" << endl;
		cout << "2. Show loaded assets" << endl;
		cout << "3. Read loaded asset" << endl;
		cout << "4. Unload asset" << endl;
		cout << "5. Exit" << endl;
		cout << "Choose an option: ";

		int choice;
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			string path;

			cout << "Enter path: ";
			cin >> path;

			shared_ptr<asset> loaded_asset = asset_manager.get(path);

			open_assets.push_back(loaded_asset);

			cout << "Asset loaded" << endl;
			break;
		}
		case 2:
		{
			PrintOpenAssets(open_assets);
			break;
		}
		case 3:
		{
			PrintOpenAssets(open_assets);

			if (open_assets.empty())
			{
				continue;
			}

			int index;
			cout << "Enter asset index: ";
			cin >> index;
			index--;

			if (index < 0 || index >= open_assets.size())
			{
				cout << "Wrong index" << endl;
				continue;
			}

			cout << open_assets[index]->read() << endl;
			break;
		}
		case 4:
		{
			PrintOpenAssets(open_assets);

			if (open_assets.empty())
			{
				continue;
			}

			int index;
			cout << "Enter asset index: ";
			cin >> index;
			index--;

			if (index < 0 || index >= open_assets.size())
			{
				cout << "Wrong index" << endl;
				continue;
			}

			open_assets.erase(open_assets.begin() + index);

			cout << "Asset unloaded" << endl;
			break;
		}
		case 5:
		{
			isRunning = false;
			cout << "Exiting program..." << endl;
			break;
		}
		default:
		{
			cout << "Error! Wrong choice" << endl;
			break;
		}
		}
	}

	return 0;
}