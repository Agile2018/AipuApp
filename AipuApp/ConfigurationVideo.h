#ifndef ConfigurationVideo_h
#define ConfigurationVideo_h

#include "Configuration.h"
#include "ErrorAipuLib.h"
#include "EnumError.h"
#include "LabelError.h"
#include <cctype>

class ConfigurationVideo: public Configuration
{
public:
	ConfigurationVideo();
	~ConfigurationVideo();
	void ParseJSONToObject();
	void ParseMapToJSON();
	void SetPath(string valuePath);
	void SetUser(string valueUser);
	void SetPassword(string valuePassword);
	void SetResolution(string valueResolution);
	void SetFPS(string valueFps);	
	void SetFPSIntToString(int intFPS);
	void SetImageTemp(string name);	
	string GetPath();
	string GetUser();
	string GetPassword();
	string GetResolution();
	string GetFPS();
	int GetFPSStringToInt();
	string GetImageTemp();
	int GetDeviceId();
	string GetURLVideo();
	bool BuildPathVideo();
	ErrorAipuLib* error = new ErrorAipuLib();
private:
	const string PATH = "path";
	const string PASSWORD = "password";
	const string USER = "user";
	const string RESOLUTION = "resolution";
	const string FRAMEXSECONDS = "fps";
	const string IMAGETEMP = "imagetemp";
	const string NAMEWINDOW = "namewindow";
	const string PARAMS = "params";
	const string CONFIGURATION = "configuration";
	const string VIDEO_CONFIGURATION = "video_configuration";
	const string HTTP = "http://";
	const string RTSP = "rtsp://";
	string path = "";
	string user = "";
	string password = "";
	string resolution = "";
	string fps = ""; //frames x second
	string imageTemp = "";
	int deviceId = NOT_DEVICE;
	string urlVideo;
	void SetValueJSONToConfiguration();
	string BuildUniformResourceLocator(string address);
	bool IsLocalDevice(string device);
	bool HasProtocol(string address);
};


#endif // !ConfigurationVideo_h

