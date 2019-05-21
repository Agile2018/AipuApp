#ifndef Molded_h
#define Molded_h

class Molded
{
public:
	Molded();
	~Molded();
	void SetMoldImage(char* mold) {
		moldImage = mold;
	}

	void SetMoldSize(int size) {
		moldSize = size;
	}

	char* GetMoldImage() {
		return moldImage;
	}

	int GetMoldSize() {
		return moldSize;
	}

private:
	char* moldImage;
	int moldSize;
};


#endif // !Molded_h

