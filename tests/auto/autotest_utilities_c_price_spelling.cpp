#include <string>
#include <iostream>
#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cstatistics.h"
#include "c_price_spelling.h"
#include "utilities.h"
#include "utilities_common.h"
#include "localy.h"

using namespace std;

bool RunTest1()
{
	auto							result = true;
	vector<pair<double, string>>	arr = {
		make_pair(0.01, "одна копейка"),
		make_pair(0.02, "две копейки"),
		make_pair(0.05, "пять копеек"),
		make_pair(0.15, "пятнадцать копеек"),
		make_pair(0.2, "двадцать копеек"),
		make_pair(0.21, "двадцать одна копейка"),
		make_pair(0.1, "десять копеек"),
		make_pair(0.2, "двадцать копеек"),
		make_pair(0.12, "двенадцать копеек"),
		make_pair(1, "один рубль"),
		make_pair(2, "два рубля"),
		make_pair(4, "четыре рубля"),
		make_pair(5, "пять рублей"),
		make_pair(10, "десять рублей"),
		make_pair(30, "тридцать рублей"),
		make_pair(31, "тридцать один рубль"),
		make_pair(300, "триста рублей"),
		make_pair(00000000123456.21, "сто двадцать три тысячи четыреста пятьдесят шесть рублей двадцать одна копейка"),
		make_pair(123456, "сто двадцать три тысячи четыреста пятьдесят шесть рублей"),
	};

	for(auto &item: arr)
	{
		C_Price_Spelling	num(item.first);
		auto				actual_output = num.Spelling();

		if(actual_output == item.second) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << actual_output << " != " << item.second << ")" << endl;
		}

	}

	return result;
}

vector<bool (*)(void)>	func_v = {
	RunTest1,
};


bool Test_ru_RU(CMysql *db)
{
	auto	result = true;

	setlocale(LC_ALL, "ru_RU.utf8");

	for(auto f: func_v)
	{
		result &= f();
	}

	return result;
}

bool Test_en_US(CMysql *db)
{
	auto	result = true;

	setlocale(LC_ALL, "en_US.utf8");

	for(auto f: func_v)
	{
		result &= f();
	}

	return result;
}

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be firts statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	CMysql			db;
	auto			testing_success = true;

	{ MESSAGE_DEBUG("", "", __FILE__); }
	signal(SIGSEGV, crash_handler);

	if(!Test_ru_RU(&db))
	{
		testing_success = false;
		cout << "Test_ru_RU failed" << endl;
	}

	if(!Test_en_US(&db))
	{
		testing_success = false;
		cout << "Test_en_US failed" << endl;
	}

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

