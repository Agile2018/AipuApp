#include "Database.h"

Database::Database()
{
	ObserverError();
	MongoAccess::instance().ObserverError();
}

Database::~Database()
{
}

void Database::Configure() {
	mongocxx::uri uri(configuration->GetConnectString().c_str()); //"mongodb://localhost:27017"
	class NoopLogger : public mongocxx::logger {
	public:
		virtual void operator()(mongocxx::log_level,
			bsoncxx::stdx::string_view,
			bsoncxx::stdx::string_view) noexcept {}
	};

	auto instance =
		bsoncxx::stdx::make_unique<mongocxx::instance>(bsoncxx::stdx::make_unique<NoopLogger>());

	MongoAccess::instance().Configure(std::move(instance),
		bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(uri)));
	
}

void Database::InsertNewUser(User* user) {
	std::thread(&Database::AddImageUser, this, 
		user->GetPathImageTemp(), user->GetUserIdIFace()).join();
	std::thread(&Database::AddUser, this, user).join();
	BuildNewUser(user);
	//std::thread(&Database::AddRecordsUser, this, user).detach();
	//AddRecordsUser(user);
}

void Database::AddRecordsUser(User* user) {
	AddImageUser(user->GetPathImageTemp(),
		user->GetUserIdIFace());		
	AddUser(user);
	BuildNewUser(user);
}

void Database::AddUser(User* user) {
	
	auto clientConnection = MongoAccess::instance().GetConnection();
	mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
	
	mongocxx::collection collection = database[COLLECTION_USER.c_str()];
	
	lastUserId = user->GetUserIdIFace();

	bsoncxx::document::value builder = make_document(
		kvp("id_face", user->GetUserIdIFace()),
		kvp("name", user->GetNameUser().c_str()),
		kvp("address", user->GetAddressUser().c_str()));

	try
	{
		bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));
		
	}
	catch (const mongocxx::exception& e)
	{
		error->CheckError(ERROR_DATABASE,
			error->medium, e.what());
	}
	

}

void Database::AddImageUser(string pathImage, int idUser) {
	auto clientConnection = MongoAccess::instance().GetConnection();
	mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
	mongocxx::collection collection = database[COLLECTION_IMAGE.c_str()];
	
	string imageBase64 = FileImageToStringBase64(pathImage);	
	bsoncxx::document::value builder = make_document(
		kvp("id_face", idUser),
		kvp("data_64", imageBase64.c_str()));
	try
	{
	
		bsoncxx::stdx::optional<mongocxx::result::insert_one> result = collection.insert_one(std::move(builder));			
		
	}
	catch (const mongocxx::exception& e)
	{
		error->CheckError(ERROR_DATABASE,
			error->medium, e.what());
	}
	
}

void Database::BuildNewUser(User* user) {
	int idFace = user->GetUserIdIFace();
	std::vector<std::string> values;
	values.push_back(to_string(idFace));
	values.push_back(user->GetNameUser());
	values.push_back(user->GetAddressUser());
	values.push_back(QueryImageOfUser(idFace));

	BuildJSONUser(values);
}

string Database::FileImageToStringBase64(string path) {
	string encodedPng;
	vector<uchar> bufferImage;
	Mat img = imread(path, IMREAD_COLOR);
	if (!img.empty())
	{
		imencode(".png", img, bufferImage);
		auto base64Png = reinterpret_cast<const unsigned char*>(bufferImage.data());
		encodedPng = FORMAT_IMAGE_64 + base64->base64_encode(base64Png, (unsigned int)bufferImage.size());

	}
	return encodedPng;
}

//string Database::GetLastId(bsoncxx::stdx::optional<mongocxx::result::insert_one> result) {
//	std::string stringId;
//
//	if (result && result->inserted_id().type() == bsoncxx::type::k_oid)
//	{
//		bsoncxx::oid id = result->inserted_id().get_oid().value;
//		stringId = id.to_string();		 		
//	}
//
//	return stringId;
//}

void Database::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		shootError.on_next(either);
	});

	auto observerErrorDatabase = MongoAccess::instance().observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionErrorDatabase = observerErrorDatabase.subscribe([this](Either* either) {
		shootError.on_next(either);
	});
	auto observerConfiguration = configuration->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionConfiguration = observerConfiguration.subscribe([this](Either* either) {
		shootError.on_next(either);
	});



}

void Database::FindUserByIdFace(int idFaceUser) {
	std::thread(&Database::QueryUserByFace, this, idFaceUser).detach();
}

void Database::QueryUserByFace(int idFaceUser) {
	if (lastUserId != idFaceUser)
	{
		auto clientConnection = MongoAccess::instance().GetConnection();
		mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
		mongocxx::collection collection = database[COLLECTION_USER.c_str()];
		lastUserId = idFaceUser;	
		boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
			.find_one(make_document(kvp("id_face", idFaceUser)));
		
		auto view = (*cursor).view();
		int idFace = view["id_face"].get_int32().value;
		std::vector<std::string> values;
		values.push_back(to_string(idFace));
		values.push_back(view["name"].get_utf8().value.to_string());
		values.push_back(view["address"].get_utf8().value.to_string());
		values.push_back(QueryImageOfUser(idFaceUser));

		BuildJSONUser(values);

	}

}

void Database::BuildJSONUser(vector<std::string> values) {
	string stringJSON;
	Json::Value jsonParams;
	Json::Value jsonBody;

	std::map<std::string, std::string> params;
	params.insert(std::pair<std::string, std::string>(FIELD_USER_ID_FACE, values[0]));
	params.insert(std::pair<std::string, std::string>(FIELD_USER_NAME, values[1]));
	params.insert(std::pair<std::string, std::string>(FIELD_USER_ADDRESS, values[2]));
	params.insert(std::pair<std::string, std::string>(FIELD_IMAGE_DATA, values[3]));
	std::map<std::string, std::string>::const_iterator it = params.begin(),
		end = params.end();
	for (; it != end; ++it) {
		jsonParams[it->first] = it->second;

	}

	jsonBody[COLLECTION_USER] = DATASOURCE;
	jsonBody[PARAMS] = jsonParams;

	Json::StyledWriter writer;
	stringJSON = writer.write(jsonBody);
	shootUserJSON.on_next(stringJSON);

}

string Database::QueryImageOfUser(int idFaceUser) {
	auto clientConnection = MongoAccess::instance().GetConnection();
	mongocxx::database database = (*clientConnection)[configuration->GetNameDatabase().c_str()];
	mongocxx::collection collection = database[COLLECTION_IMAGE.c_str()];
	boost::optional<bsoncxx::v_noabi::document::value> cursor = collection
		.find_one(make_document(kvp("id_face", idFaceUser)));
	auto view = (*cursor).view();
	string image = view["data_64"].get_utf8().value.to_string();
	return image;
}


//void Database::FindUser(bsoncxx::stdx::optional<mongocxx::result::insert_one> result) {
//	auto clientConnection = MongoAccess::instance().GetConnection();
//	mongocxx::database database = (*clientConnection)[nameDatabase.c_str()];
//	mongocxx::collection collection = database[COLLECTION_USER.c_str()];
//
//	if (result && result->inserted_id().type() == bsoncxx::type::k_oid)
//	{
//		bsoncxx::oid id = result->inserted_id().get_oid().value;
//		auto cursor = collection.find(make_document(kvp(FIELD_USER_ID, id)));
//
//		for (auto&& doc : cursor) {
//			string jsonUser = bsoncxx::to_json(doc);
//			shootUserJSON.on_next(jsonUser);
//		}
//
//	}
//
//}