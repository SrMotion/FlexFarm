// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "bot.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

#include "bot.h"
#include "http.h"

#include "crypt.h"
#include "LoginForm.h"
#include <memory>
// ---------------------------------------------------------------------------
#include <System.IOUtils.hpp>
#include <System.Threading.hpp>
std::vector<BreakerBotV2*>bots;

#include <IdSync.hpp>
#include <Registry.hpp>

std::string gtver;
std::string ip;
int port;

UnicodeString GetModulePath() {
	wchar_t pBuf[MAX_PATH];
	if (GetModuleFileName(0, pBuf, MAX_PATH)) {
		PathRemoveFileSpec(pBuf);
		return pBuf;
	}
	return GetCurrentDir();
}
//static std::thread httpserver(http::run, "127.0.0.1", "17191");
std::unique_ptr<TIniFile>ini;
int appvers = 15;
bool pre = false;
std::string responseshit = "";
template<class _Fx, class ... _Types>
static void Synchronize(_Fx && _Func, _Types && ... _Args) {
	class Synchronization : public TIdSync {
	protected:
		virtual void __fastcall DoSynchronize() {
			func();
		};

	private:
		std::function < void() > func;

	public:
		Synchronization(std::function < void() > func) : func(func) {
		}
	};

	(new Synchronization(std::bind(_Func, _Args ...)))->Synchronize();
}

std::vector<BreakerBotV2*>getSelectedBots() {
	std::vector<BreakerBotV2*>buf;

	for (int i = 0; i < Form1->ListBox1->Items->Count; i++) {
		if (Form1->ListBox1->Selected[i] || Form1->CheckBox1->Checked) {
			for (auto ptr : bots) {
				if (ptr->loginInfo.getGrowID() == AnsiString
					(Form1->ListBox1->Items->Strings[i]).c_str()
					&& ptr->ingame && ptr->running) {
					buf.push_back(ptr);
				}
			}
		}
	}
	return buf;
}

BreakerBotV2* getselectedbot() {
	for (int i = 0; i < Form1->ListBox1->Items->Count; i++) {
		if (Form1->ListBox1->Selected[i]) {
			for (auto ptr : bots) {
				if (ptr->loginInfo.getGrowID() == AnsiString
					(Form1->ListBox1->Items->Strings[i]).c_str()) {
					return ptr;
				}
			}
		}
	}
	return NULL;
}

bool botExists(UnicodeString aga) {
	for (int i = 0; i < Form1->ListBox1->Items->Count; i++) {
		if (aga == Form1->ListBox1->Items->Strings[i]) {
			return true;
		}
	}
	return false;
}

__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_y -= 32;
		botptr->sendPlayerState();
	}
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void addBot(AnsiString gid, AnsiString pwd, bool* nothing = 0) {
    if (gid.IsEmpty() || pwd.IsEmpty()) {
        return;
    }

    LoginInfo login;
    login.setGrowID(AnsiString(gid).c_str());

    if (botExists(login.getGrowID().c_str())) {
        ShowMessage("Bot already exists!");
        return;
    }

    login.password = AnsiString(pwd).c_str();
    login.macAddress = GenerateMacAddress();
    login.version = gtver;
    login.serverIp = ip;
    login.serverPort = port;
    bots.emplace_back(new BreakerBotV2(login));
    bots.back()->startAsync();
	Form1->ListBox1->Items->Add(login.getGrowID().c_str());
	//DoStuffForBot(login);
}
void __fastcall TForm1::Button1Click(TObject *Sender) {
		//Form1->Edit7->Text = gtver.c_str();
	//Form1->Edit8->Text = ip.c_str();
	//Form1->Edit9->Text = std::to_string(port).c_str();
	gtver = "3.82";
	ip = "213.179.209.168";
	port =17249;
	pre = true;
	  addBot(Edit1->Text, Edit2->Text);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender) {
	if (Edit4->Text.IsEmpty()) {
		return;
	}
	for (auto botptr : getSelectedBots()) {
		botptr->setTargetWorld(AnsiString(Edit4->Text).c_str());
		if (botptr->world_to_be != botptr->world.name)
			SendString(botptr->peer, botptr->host, 3,
			"action|join_request\nname|" + botptr->world_to_be +
			"\ninvitedWorld|0");
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox3Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
        botptr->following = CheckBox3->Checked;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_x -= 32;
		botptr->sendPlayerState();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_x += 32;
		botptr->sendPlayerState();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_y += 32;
		botptr->sendPlayerState();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox2Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->auto_reconnect = CheckBox2->Checked;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender) {
	for (int i = 0; i < ListBox1->Count; i++) {
		if (ListBox1->Selected[i]) {
			bots.erase(std::remove_if(bots.begin(), bots.end(),
				[&](auto&ptr)->bool {
				if (ListBox1->Items->Strings[i] == ptr->loginInfo.getGrowID()
					.c_str())
				{ptr->running = false; return true;} return false;
			}), bots.end());
			ListBox1->Items->Delete(i);
			i--;
		}
	}
}
// ---------------------------------------------------------------------------

#include <Registry.hpp>

UnicodeString FindGrowtopiaDirPath() {
	UnicodeString pathBuffer;
	if (wchar_t pathLoc[MAX_PATH];
	SUCCEEDED(SHGetFolderPathW(0, CSIDL_PROFILE, 0, 0, pathLoc))) {
		pathBuffer = pathLoc;
		pathBuffer += L"\\AppData\\Local\\Growtopia";
		if (DirectoryExists(pathBuffer))
			return pathBuffer;
	}
	if (pathBuffer = ExtractFilePath(GetHomePath()) + L"\\Local\\Growtopia";
	DirectoryExists(pathBuffer))
		return pathBuffer;
	else {
		std::shared_ptr<TRegistry>registry(new TRegistry());
		if (registry->RootKey = HKEY_CURRENT_USER;
		registry->OpenKey("Software\\Growtopia", false)) {
			if (registry->ValueExists("path"))
				return registry->ReadString("path");
		}
	}
	return L"";
}

void loaditems(UnicodeString path) {
	Form1->Edit6->Text = path;
	if (!path.IsEmpty()) {
		World::itemDataContainer.LoadItemData(path);
		Form1->ListView1->Clear();
		for (auto item : World::itemDataContainer.item_map) {
			auto ptr = Form1->ListView1->Items->Add();
			ptr->Caption = item.second.name.c_str();
			ptr->SubItems->Insert(0, std::to_string(item.first).c_str());
		}
	}
}

void loadsettings() {
	//Form1->Edit7->Text = gtver.c_str();
	//Form1->Edit8->Text = ip.c_str();
	//Form1->Edit9->Text = std::to_string(port).c_str();
}

void savesettings() {
	ini->WriteString("mainform", "gtver", Form1->Edit7->Text);
	ini->WriteString("mainform", "ip", Form1->Edit8->Text);
	ini->WriteInteger("mainform", "port", StrToInt(Form1->Edit9->Text));
}

class SaveDecoder {
public:

	int gidOffset = -1, passOffset = -1, worldOffset = -1;
	std::string growid = "", password = "", lastworld = "", strContent = "";
	std::vector<BYTE>content;

	SaveDecoder(UnicodeString filePath) {
		if (ReadContent(filePath)) {
			LocateDataOffsets();
			if (gidOffset != -1 && passOffset != -1) {
				growid = strContent.substr(gidOffset + 4,
					int(content[gidOffset]));
				DecryptPassword();
				lastworld = strContent.substr(worldOffset + 4,
					int(content[worldOffset]));
			}
		}
	}

	bool ReadContent(UnicodeString filePath) {
		if (FileExists(filePath)) {
			if (std::shared_ptr<TFileStream>fileStream(new TFileStream(filePath,
				fmOpenRead)); fileStream->Size) {
				content.resize(fileStream->Size);
				fileStream->Position = 0;
				fileStream->ReadBuffer(content.data(), content.size());
			}
		}
		return content.size();
	}

	void LocateDataOffsets() {
		strContent.assign(std::string(reinterpret_cast<PCHAR>(content.data()),
			content.size()));
		gidOffset = (strContent.find("tankid_name") != -1 ?
			strContent.find("tankid_name") + 11 : -1);
		worldOffset = (strContent.find("lastworld") != -1 ?
			strContent.find("lastworld") + 9 : -1);
		int index1 = strContent.find("tankid_password"), index2 =
			strContent.rfind("tankid_password");
		passOffset = ((index1 > -1 || index2 > -1) && index1 != index2 ?
			(content[index1 + 15] == '_' ? index2 + 15 : index1 + 15) : -1);
	}

	void DecryptPassword() {
		int offset = (-(RetreiveKeyHash() + 2)) % 256;
		password.resize(int(strContent[passOffset]));
		for (int i = 0; i < password.size(); i++)
			password[i] = content[passOffset + i + 4] - i + offset;
	}

	uint32_t RetreiveKeyHash() {
		DWORD dwDiskSerial;
		if (!GetVolumeInformation(L"C:\\", 0, 0, &dwDiskSerial, 0, 0, 0, 0)) {
			if (!GetVolumeInformation(L"D:\\", 0, 0, &dwDiskSerial, 0, 0, 0, 0))
			{
				if (!GetVolumeInformation(L"E:\\", 0, 0, &dwDiskSerial, 0, 0,
					0, 0)) {
					if (!GetVolumeInformation(L"F:\\", 0, 0, &dwDiskSerial, 0,
						0, 0, 0)) {
						if (!GetVolumeInformation(L"G:\\", 0, 0, &dwDiskSerial,
							0, 0, 0, 0))
							return 0;
					}
				}
			}
		}

		if (std::string vol(std::to_string(dwDiskSerial)); vol.size()) {
			uint32_t buf = 0x55555555;
			for (int i : vol)
				buf = (buf >> 27) + (buf << 5) + i;
			return buf;
		}
		return 0;
	}

};

#include <IdBaseComponent.hpp>
#include <System.Classes.hpp>
#include <System.Threading.hpp>
#include <Iphlpapi.h>
#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <vector>

std::vector<std::string> CollectMac() {
	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
	std::vector<std::string>macAddr;
	if (std::shared_ptr<IP_ADAPTER_INFO>adapterInfo
		(reinterpret_cast<PIP_ADAPTER_INFO>(malloc(dwBufLen)));
	adapterInfo.get()) {
		if (GetAdaptersInfo(adapterInfo.get(),
			&dwBufLen) == ERROR_BUFFER_OVERFLOW)
			adapterInfo.reset(reinterpret_cast<PIP_ADAPTER_INFO>
			(malloc(dwBufLen)));
		if (GetAdaptersInfo(adapterInfo.get(), &dwBufLen) == NO_ERROR) {
			PIP_ADAPTER_INFO pAdapterInfo = adapterInfo.get();
			do {
				std::shared_ptr<char>buffer(new char[18]);
				sprintf_s(buffer.get(), 18, "%02X:%02X:%02X:%02X:%02X:%02X",
					pAdapterInfo->Address[0], pAdapterInfo->Address[1],
					pAdapterInfo->Address[2], pAdapterInfo->Address[3],
					pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
				macAddr.insert
					((std::string(pAdapterInfo->IpAddressList.IpAddress.String)
					!= "0.0.0.0" ? macAddr.begin() : macAddr.end()),
					buffer.get());
				pAdapterInfo = pAdapterInfo->Next;
			}
			while (pAdapterInfo);
		}
	}
	try {
		std::unique_ptr<TStrings>nameList(new TStringList());
		std::unique_ptr<TRegistry>Registry(new TRegistry(KEY_READ));
		Registry->RootKey = HKEY_CURRENT_USER;
		if (Registry->OpenKey("", false)) {
			Registry->GetKeyNames(nameList.get());
			if (Registry->OpenKey(nameList->Strings[0], false)) {
				Registry->GetValueNames(nameList.get());
				DWORD dwType = REG_BINARY, dwSize = 0;
				if (RegQueryValueEx(Registry->CurrentKey,
					nameList->Strings[0].c_str(), NULL, &dwType, NULL,
					&dwSize) == ERROR_SUCCESS) {
					std::vector<BYTE>dwBuffer;
					dwBuffer.resize(dwSize);
					if (RegQueryValueEx(Registry->CurrentKey,
						nameList->Strings[0].c_str(), NULL, &dwType,
						dwBuffer.data(), &dwSize) == ERROR_SUCCESS) {
						std::string buffer;
						for (auto chr : dwBuffer) {
							std::wstringstream stream;
							stream << std::hex << chr;
							std::wstring res(stream.str());
							res = res.size() == 1 ? L"0" + res : res;
							buffer += std::string(res.begin(), res.end());
						}
						if (buffer.size() == 36) {
							auto IntToHex = [](int INT)->std::string {
								std::stringstream stream;
								stream << std::hex << INT;
								return std::string(stream.str());
							};
							auto HexCharToInt = [](char HexChar)->int {
								std::string chars = "0123456789abcdefABCDEF";
								size_t posX = chars.find(HexChar);
								return posX == std::string::npos ? 0 :
									(posX > 15 ? posX - 6 : posX);
							};
							std::string result = "";
							for (int i = 0; i < 17; i++) {
								if (i % 3 == 2)
									continue;
								BYTE val = HexCharToInt(buffer[i * 2]) * 16 +
									HexCharToInt(buffer[(i * 2) + 1]);
								result +=
									IntToHex((unsigned char)
									((val >= 0x1f && val <= 0x34) ?
									(val - (21 + i)) : (val - (238 + i))));
							}
							std::vector<std::string>bytes;
							for (int i = 0; i < result.size(); i += 2)
								bytes.push_back(result.substr(i, 2));
							result = AnsiString(Join(bytes, ":").c_str())
								.UpperCase().c_str();
							if (auto it = std::find(macAddr.begin(),
								macAddr.end(), result); it != macAddr.end())
								macAddr.erase(it);
							macAddr.insert(macAddr.begin(), result);
						}
					}
				}
			}
		}
	}
	catch (...) {
	}
	return macAddr;
}

#include <sstream>

std::string Join(const std::vector<std::string>& strVector, std::string sep) {
	std::string buffer = "";
	for (int i = 0; i < strVector.size(); i++)
		buffer.append(strVector[i] + (i < strVector.size() - 1 ? sep : ""));
	return buffer;
}

UnicodeString GetGrowtopiaFilePath(AnsiString fileName) {
	if (UnicodeString dirPath(FindGrowtopiaDirPath()); DirectoryExists(dirPath))
		return (FileExists(dirPath + L"//" + fileName) ?
		dirPath + L"\\" + fileName : L"");
	return "";
}


void __fastcall TForm1::FormCreate(TObject *Sender) {

	try {
	if (AnsiString path = GetGrowtopiaFilePath("cache\\items.dat");
	FileExists(path)) {
		loaditems(path);
	}
	else {

		std::shared_ptr<TResourceStream>rStream
			(new TResourceStream((UINT)HInstance, L"items", RT_RCDATA));
		AnsiString path1 = GetModulePath() + "\\items.dat";
		rStream->SaveToFile(path1);
		loaditems(path1);
	}

	} catch (...) {
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox4Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->enabled = CheckBox4->Checked;
		if (Edit5->Text.IsEmpty()) {
			botptr->farmable_block_id = -1;
		}
		else
			botptr->farmable_block_id = StrToInt(Edit5->Text);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button15Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-2, -2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button14Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-1, -2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(0, -2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(1, -2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button24Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(2, -2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-2, -1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button16Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-1, -1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(0, -1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(1, -1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button29Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(2, -1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button22Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-2, 0);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button21Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-1, 0);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button18Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(1, 0);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button17Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(2, 0);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button26Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-2, 1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button25Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-1, 1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button20Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(0, 1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button19Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(1, 1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button30Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(2, 1);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button27Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-2, 2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button23Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(-1, 2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(0, 2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button28Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(1, 2);
		if (button->Caption == "") {
			botptr->thing.push_back(pair);
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button31Click(TObject *Sender) {
	auto button = static_cast<TButton*>(Sender);
	for (auto botptr : getSelectedBots()) {
		auto pair = std::make_pair<int, int>(2, 2);
		if (button->Caption == "") {
			if (std::find(botptr->thing.begin(), botptr->thing.end(),
				pair) == botptr->thing.end()) {
				botptr->thing.push_back(pair);
			}
			button->Caption = "Ok";
		}
		else if (button->Caption == "Ok") {
			botptr->thing.erase(std::remove(botptr->thing.begin(),
				botptr->thing.end(), pair), botptr->thing.end());
			button->Caption = "";
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit3Change(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->follow = AnsiString(Edit3->Text.LowerCase()).c_str();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::PageControl1Exit(TObject *Sender) {
	for (auto botptr : bots) {
		botptr->running = false;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit5Change(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		if (Edit5->Text.IsEmpty()) {
			botptr->farmable_block_id = -1;
		}
		else
			botptr->farmable_block_id = StrToInt(Edit5->Text);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Load1Click(TObject *Sender) {
	//
	if (!OpenDialog1->Execute())
		return;

	AnsiString content(TFile::ReadAllText(OpenDialog1->FileName));
	if (content.IsEmpty())
		return;

	std::vector<std::string>accs = ParseString(content.c_str(), "\n");

	if (!accs.size()) {
		return;
	}

	Load1->Enabled = false;

	TTask::Run([&, accs]() {
		for (int i = 0; i < accs.size(); i++)
		{auto parsed = ParseString(accs[i], ":");
			if (parsed.size() < 2) { continue;}

			auto gid = parsed[0]; auto pwd = parsed[1];

			if (gid.empty() || pwd.empty()) { continue;}

			bool nothing = false; Synchronize([&]()
			{addBot(gid.c_str(), pwd.c_str(), &nothing);});

			if (nothing) {break;}

			if (accs.size() - 1 != i)
				Sleep(30 * 1000);} Synchronize([&]()
		{Load1->Enabled = true;});});
	ShowMessage(
		"each bot will be executed within 30 seconds of delay to avoid shadow ban.");
}
// ---------------------------------------------------------------------------

#include <sstream>

void __fastcall TForm1::Save1Click(TObject *Sender) {

	if (!SaveDialog1->Execute()) {
		return;
	}

	std::stringstream ss;
	for (auto botptr : getSelectedBots()) {
		ss << botptr->loginInfo.getGrowID()
			<< ":" << botptr->loginInfo.password << std::endl;
	}

	if (ss.str().size()) {

		TFile::WriteAllText(SaveDialog1->FileName, ss.str().c_str());

	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox5Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->breakb = CheckBox5->Checked;
	}
	//
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox6Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->place_c = CheckBox6->Checked;
	}

	//
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::SearchBox1InvokeSearch(TObject *Sender) {
	SearchBox1->Text = SearchBox1->Text.Trim();
	for (int i = 0; i < ListView1->Items->Count; i++) {
		ListView1->Items->Item[i]->Selected = false;
	}
	for (int i = 0; i < ListView1->Items->Count; i++) {
		if (StartsWith(AnsiString(ListView1->Items->Item[i]->Caption.LowerCase()
			).c_str(), AnsiString(SearchBox1->Text.LowerCase()).c_str())) {
			ListView1->Items->Item[i]->Selected = true;
		}
	}

	for (int i = 0; i < ListView1->Items->Count; i++) {
		if (ListView1->Items->Item[i]->Selected) {
			ListView1->Items->Item[i]->MakeVisible(false);
			break;
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Button32Click(TObject *Sender) {
	if (Form1->OpenDialog1->Execute()) {
		loaditems(Form1->OpenDialog1->FileName);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Style(TObject *Sender) {
	if (TMenuItem * selected = dynamic_cast<TMenuItem*>(Sender); selected) {
		TStyleManager::TrySetStyle
			(ReplaceString(AnsiString(selected->Caption).c_str(), "&",
			"").c_str());
	}
}
// ---------------------------------------------------------------------------

void TForm1::itemchange() {

	auto shit = [&](BreakerBotV2 * ptr) {
		for (int i = 0; i < ListBox1->Count; i++) {
			if (ListBox1->Items->Strings[i] == ptr->loginInfo.getGrowID()
				.c_str()) {
				bots.erase(std::remove_if(bots.begin(), bots.end(),
					[&](auto&ptr)->bool {
					if (ListBox1->Items->Strings[i] == ptr->loginInfo.getGrowID
						().c_str())
					{ptr->running = false; return true;} return false;
				}), bots.end());
				ListBox1->Items->Delete(i);
				i--;
			}
		}
	};

	if (auto ptr = getselectedbot(); ptr) {
		if (!ptr->fail_message.empty()) {
			ShowMessage(ptr->fail_message.c_str());
			shit(ptr);
		PageControl2->Visible = false;
		Label16->Visible = true;
			return;
		}

		if (!ptr->running) {
			shit(ptr);
			return;
		}
		PageControl2->Visible = true;
		Label16->Visible = false;
		this->Caption = base64_decode("RmxleEZhcm0gdjEuMzIgW1ByZW1pdW1d").c_str();
		CheckBox2->Checked = ptr->auto_reconnect;
		CheckBox3->Checked = !ptr->follow.empty();
		Edit3->Text = ptr->follow.c_str();
		Edit4->Text = ptr->world.name.c_str();
		CheckBox4->Checked = ptr->enabled;
        CheckBox11->Checked = ptr->collecting;
		CheckBox5->Checked = ptr->breakb;
		CheckBox6->Checked = ptr->place_c;
		CheckBox8->Checked = ptr->spamming;
		Edit10->Text = ptr->spamtext.c_str();
		Edit5->Text = ptr->farmable_block_id == -1 ? "" :
			std::to_string(ptr->farmable_block_id).c_str();
		CheckBox7->Checked = ptr->auto_access;
		Label8->Caption = AnsiString("Name: ") + ptr->loginInfo.getGrowID()
			.c_str();
		if (ptr->local_player.level != -1)
			Label9->Caption = AnsiString("level: ") + std::to_string
				(ptr->local_player.level).c_str();
		if (ptr->local_player.gems_balance != -1) {
			Label10->Caption = AnsiString("Gems: ") + std::to_string
				(ptr->local_player.gems_balance).c_str();

		}
		if (ptr->local_player.userid != -1) {
			Label11->Caption = AnsiString("User id: ") + std::to_string
				(ptr->local_player.userid).c_str();
		}
		Label12->Caption = AnsiString("World player amount: ") +
			std::to_string(ptr->world.players.size() + 1).c_str();

		for (int i = 0; i < TabSheet3->ControlCount; ++i) {
			if (TabSheet3->Controls[i]->ClassType() == __classid(TButton)) {
				static_cast<TButton*>(TabSheet3->Controls[i])->Caption = "";
			}
		}

		for (auto pair : ptr->thing) {
			if (pair == std::make_pair<int, int>(-2, -2)) {
				Button15->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-1, -2)) {
				Button14->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(0, -2)) {
				Button9->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(1, -2)) {
				Button10->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(2, -2)) {
				Button24->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-2, -1)) {
				Button13->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-1, -1)) {
				Button16->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(0, -1)) {
				Button12->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(1, -1)) {
				Button11->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(2, -1)) {
				Button29->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-2, 0)) {
				Button22->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-1, 0)) {
				Button21->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(1, 0)) {
				Button18->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(2, 0)) {
				Button17->Caption = "Ok";
			} // hm
			else if (pair == std::make_pair<int, int>(-2, 1)) {
				Button26->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-1, 1)) {
				Button25->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(0, 1)) {
				Button20->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(1, 1)) {
				Button19->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(2, 1)) {
				Button30->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-2, 2)) {
				Button27->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(-1, 2)) {
				Button23->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(0, 2)) {
				Button7->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(1, 2)) {
				Button28->Caption = "Ok";
			}
			else if (pair == std::make_pair<int, int>(2, 2)) {
				Button31->Caption = "Ok";
			}
		}
	}
	else
	{
		PageControl2->Visible = false;
		Label16->Visible = true;
    }
}

void __fastcall TForm1::ListBox1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	itemchange();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1KeyUp(TObject *Sender, WORD &Key,
	TShiftState Shift) {
	itemchange();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox7Click(TObject *Sender) {
	for (auto botptr : getSelectedBots()) {
		botptr->auto_access = CheckBox7->Checked;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit7Change(TObject *Sender) {
	gtver = AnsiString(Edit7->Text).c_str();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit8Change(TObject *Sender) {
	ip = AnsiString(Edit8->Text).c_str();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit9Change(TObject *Sender) {
	if (Edit9->Text.IsEmpty()) {
		return;
	}
	port = StrToInt(Edit9->Text);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action) {
	//savesettings();
}
// ---------------------------------------------------------------------------
void __fastcall TForm1::CheckBox8Click(TObject *Sender)
{
	for (auto botptr : getSelectedBots()) {
	   botptr->spamming = CheckBox8->Checked;
	}
    Timer1->Interval = StrToInt(Edit11->Text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit10Change(TObject *Sender)
{
	for (auto botptr : getSelectedBots()) {
		if (Edit10->Text.IsEmpty()) {
		botptr->spamtext = "";
	}
	else
	{
	 botptr->spamtext = AnsiString(Edit10->Text).c_str();
	}
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	for (auto botptr : bots) {
	if(botptr->spamming)
	{
	if(botptr->spamtext != "")
	{
	 botptr->logMessage(botptr->spamtext);
	}
	}
}
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------


void __fastcall TForm1::Button34Click(TObject *Sender)
{
	if (Memo1->Lines->Text.IsEmpty()) {
		return;
	}
	for (auto botptr : getSelectedBots()) {
        SendString(botptr->peer, botptr->host, std::stoi(AnsiString(Edit12->Text).c_str()) , AnsiString(Memo1->Lines->Text).c_str());
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button33Click(TObject *Sender)
{
	for (auto botptr : getSelectedBots()) {
		botptr->Act7(int(botptr->local_player.pos.m_x / 32),int(botptr->local_player.pos.m_y / 32));
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Youtube1Click(TObject *Sender)
{
	   std::string readtr = "https://youtube.com/c/sahyuinew";
	   ShellExecuteA(NULL, "open", readtr.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Discord1Click(TObject *Sender)
{
	   std::string readtr = "https://discord.gg/YcaNFz3tkM";
	ShellExecuteA(NULL, "open", readtr.c_str(), NULL, NULL, SW_SHOWNORMAL);

}
void __fastcall TForm1::Update1Click(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------










void __fastcall TForm1::CheckBox10Click(TObject *Sender)
{
	for (auto botptr : getSelectedBots()) {
		botptr->Wrench(int(botptr->local_player.pos.m_x / 32) ,int(botptr->local_player.pos.m_y / 32) -1 );
		Sleep(50);
		botptr->Wrench(int(botptr->local_player.pos.m_x / 32) ,int(botptr->local_player.pos.m_y / 32) +1 );
		Sleep(50);
		botptr->Wrench(int(botptr->local_player.pos.m_x / 32) -1 ,int(botptr->local_player.pos.m_y / 32));
		Sleep(50);
		botptr->Wrench(int(botptr->local_player.pos.m_x / 32) + 1 ,int(botptr->local_player.pos.m_y / 32));
	}
}
//---------------------------------------------------------------------------



void __fastcall TForm1::CheckBox11Click(TObject *Sender)
{
for (auto botptr : getSelectedBots()) {
		botptr->collecting = CheckBox11->Checked;
}
}
//---------------------------------------------------------------------------







void __fastcall TForm1::Button35Click(TObject *Sender)
{
	bool verici = false;
	std::string osmancedi = AnsiString(TFile::ReadAllText("C:\\Windows\\System32\\drivers\\etc\\hosts")).c_str();
	if(osmancedi.find("growtopia1.com") != -1 || osmancedi.find("growtopia2.com") != -1)
	{
	   TFile::WriteAllText("C:\\Windows\\System32\\drivers\\etc\\hosts","");
       verici = true;
	}
	RESTRequest1->Execute();
	std::string ansi = AnsiString(RESTResponse1->Content).c_str();
	rtvar server = rtvar::parse(ansi);
	port = std::stoi(server.get("port").c_str());
	ip = server.get("server").c_str();
	loadsettings();
	if(verici)
	{
		TFile::WriteAllText("C:\\Windows\\System32\\drivers\\etc\\hosts",osmancedi.c_str());
    }
}
//---------------------------------------------------------------------------











void __fastcall TForm1::Edit13Change(TObject *Sender)
{

	for (auto botptr : getSelectedBots()) {
		if (Edit13->Text.IsEmpty()) {
		botptr->mega_txt = "";
	}
	else
	{
	 botptr->mega_txt = AnsiString(Edit13->Text).c_str();
	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button36Click(TObject *Sender)
{
	for (auto botptr : getSelectedBots()) {
		botptr->Mega(int(botptr->local_player.pos.m_x / 32),int(botptr->local_player.pos.m_y / 32));
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button37Click(TObject *Sender)
{
	for (auto botptr : getSelectedBots()) {
		botptr->setTargetWorld("BAKOENES");
			SendString(botptr->peer, botptr->host, 3,
			"action|join_request\nname|" + botptr->world_to_be +
			"\ninvitedWorld|0");
	}
    Sleep(1500);
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_x += 32;
		botptr->sendPlayerState();
	}
    Sleep(250);
	for (auto botptr : getSelectedBots()) {
		botptr->Act7(int(botptr->local_player.pos.m_x / 32),int(botptr->local_player.pos.m_y / 32));
	}
    Sleep(1000);
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_x += 32;
		botptr->sendPlayerState();
	}
    Sleep(250);
	for (auto botptr : getSelectedBots()) {
		botptr->local_player.pos.m_x -= 32;
		botptr->sendPlayerState();
	}
	Sleep(250);
	for (auto botptr : getSelectedBots()) {
		botptr->DropItem(2480,botptr->lastcollected-1);
	}
}
//---------------------------------------------------------------------------




