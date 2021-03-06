#ifndef Database_h
#define Database_h

#include <iostream>
#include "MongoAccess.h"
#include "User.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
//#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include "ErrorAipuLib.h"
#include "Base64.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <json\json.h>
#include "ConfigurationDatabase.h"

using namespace cv;
using namespace std;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::type;

class Database
{
public:
	Database();
	~Database();
	/*void SetNameDatabase(string name) {
		nameDatabase = name;
	}

	void SetConnection(string connect) {
		stringConnection = connect;
	}*/

	void Configure();
	void InsertNewUser(User* user);	
	void FindUserByIdFace(int idFaceUser);
	ConfigurationDatabase* configuration = new ConfigurationDatabase();
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	Rx::subject<string> userSubject;
	Rx::observable<string> observableUserJSON = userSubject.get_observable();

private:
	const int ERROR_DATABASE = 404;
	const string COLLECTION_USER = "user";
	const string COLLECTION_IMAGE = "image";
	const string FIELD_IMAGE_ID_USER = "id_user";
	const string FIELD_IMAGE_DATA = "data_64";
	const string FIELD_USER_ID_FACE = "id_face";
	const string FIELD_USER_ID = "_id";
	const string FIELD_USER_NAME = "name";
	const string FIELD_USER_ADDRESS = "address";
	const string FORMAT_IMAGE_64 = "data:image/jpeg;base64,";
	const string PARAMS = "params";
	const string DATASOURCE = "MongoDB";
	int lastUserId = 0;
	/*string nameDatabase;
	string stringConnection;*/
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<string> shootUserJSON = userSubject.get_subscriber();
	ErrorAipuLib* error = new ErrorAipuLib();
	Base64* base64 = new Base64();
	void AddUser(User* user);
	void AddRecordsUser(User* user);
	void AddImageUser(string pathImage, int idUser);
	void ObserverError();
	void QueryUserByFace(int idFaceUser);
	void BuildJSONUser(vector<std::string> values);
	void BuildNewUser(User* user);
	string QueryImageOfUser(int idFaceUser);
	//string GetLastId(bsoncxx::stdx::optional<mongocxx::result::insert_one> result);
	string FileImageToStringBase64(string path);
	//void FindUser(bsoncxx::stdx::optional<mongocxx::result::insert_one> result);
};


#endif // !Database_h

