#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;

class Father{
private:
	int money;

protected:
	int room_key;
	
public:
	
	int address;
	
	void it_skill(void)
	{
		cout << "father is it skill" << endl;
	}

	int getMoney(void)
	{
		return money;
	}
	void setMoney(int money)
	{
		this->money = money;
	}
};

class Son_pub :public Father{
private:
	int toy;
	//using Father::it_skill;
public:	
	void play_game(void)
	{
		int m;

		//money -= 1;

		cout << "son play game" << endl;

		m = getMoney();
		m--;
		setMoney(m);

		room_key = 1;
		
	}
};

class Son_pro :protected Father{
private:
	int toy;
	//using Father::it_skill;
public:
	
	void play_game(void)
	{
		int m;

		//money -= 1;

		cout << "son play game" << endl;

		m = getMoney();
		m--;
		setMoney(m);

		room_key = 1;
		
	}
};


class Son_pri :private Father{
private:
	int toy;
	//using Father::it_skill;
public:
	
	void play_game(void)
	{
		int m;

		//money -= 1;

		cout << "son play game" << endl;

		m = getMoney();
		m--;
		setMoney(m);

		room_key = 1;
		
	}
};

class Grandson_pub:public Son_pub{

public:
	void test(void)
	{
		room_key = 1;
		address = 2;
	}

};

class Grandson_pro:public Son_pro{

public:
	void test(void)
	{
		room_key = 1;
		address = 2;
	}

};

class Grandson_pri:public Son_pri{

public:
	void test(void)
	{
//		room_key = 1;
//		address = 2;
	}

};






int main(int argc, char **argv)
{

	Son_pub s_pub;
	Son_pri s_pri;
	Son_pro s_pro;

	s_pub.play_game();
	s_pri.play_game();
	s_pro.play_game();

	s_pub.it_skill();
	//s_pri.it_skill();
	//s_pro.it_skill();

	Grandson_pub gs_pub;
	Grandson_pro gs_pro;

	gs_pub.address = 1;
//	gs_pro.address = 1;

	return 0;
}


